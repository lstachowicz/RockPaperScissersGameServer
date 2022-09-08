//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#include "rank.hpp"
#include "paths.hpp"

#include <fstream>
#include <sstream>

using namespace RPSGame;

Rank::~Rank()
{
    if (!stopped())
    {
        stop();
    }
}

void Rank::rank(std::multimap<uint64_t, std::string>& rank)
{
    std::scoped_lock<std::mutex> lock(m_rank_lock);
    rank.insert(m_rank.begin(), m_rank.end());
}

void Rank::update(std::pair<PlayerId, uint64_t> entry)
{
    std::unique_lock<std::mutex> lock(m_queue_lock);
    m_queue.push(std::move(entry));
    cv.notify_all();
}

void Rank::run()
{
    load();

    auto fill_rank = [this](std::multimap<uint64_t, std::string>& rank) {
        for (auto it = m_rank_score.begin(); it != m_rank_score.end(); ++it)
        {
            rank.emplace(it->first, it->second.toString());
        }
    };

    std::multimap<uint64_t, std::string> rank;
    std::queue<std::pair<PlayerId, uint64_t> > queue;
    while(!stopped())
    {
        rank.clear();

        {
            std::scoped_lock<std::mutex> lock(m_queue_lock);
            queue.swap(m_queue);
        }

        update(queue);

        {
            fill_rank(rank);

            std::scoped_lock<std::mutex> lock(m_queue_lock);
            rank.swap(m_rank);
        }

        save();

        {
            std::unique_lock<std::mutex> lock(m_queue_lock);
            while (m_queue.empty() && !stopped())
            {
                cv.wait_for(lock, std::chrono::milliseconds(100));
            }
            lock.unlock();
        }
    }

    {
        std::scoped_lock<std::mutex> lock(m_queue_lock);
        queue.swap(m_queue);
    }
    update(queue);
    save();
}

void Rank::update(std::queue<std::pair<PlayerId, uint64_t>>& queue)
{
    while (!queue.empty())
    {
        auto el = queue.front();
        if (m_rank_score.size() < 20 || el.second > m_rank_score.begin()->first)
        {
            for(auto it = m_rank_score.begin(); it != m_rank_score.end(); ++it)
            {
                if (it->second == el.first)
                {
                    m_rank_score.erase(it);
                    break;
                }
            }
            m_rank_score.insert({ el.second, el.first});

            if (m_rank_score.size() > 20)
            {
                m_rank_score.erase(m_rank_score.begin());
            }
        }

        queue.pop();
    }
}

void Rank::load()
{
    m_rank_score.clear();

    std::ifstream rank_file(RANK_FILE);
    if (rank_file.is_open())
    {
        std::string line;
        while (std::getline(rank_file, line))
        {
            std::istringstream iss(line);
            uint64_t rank;
            std::string player_hash;
            if (!(iss >> player_hash >> rank))
            {
                m_rank_score.clear();
                break;
            };
            m_rank_score.insert({rank, PlayerId{player_hash}});
        }
    }
}

void Rank::save()
{
    std::ofstream rank_file(RANK_FILE);
    if (rank_file.is_open())
    {
        for (auto it = m_rank_score.begin(); it != m_rank_score.end(); ++it)
        {
            rank_file << it->second.toString() << " " << it->first << "\n";
        }
    }
}
