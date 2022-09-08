//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#pragma once

#include "player_id.hpp"
#include "thread.hpp"

#include <map>
#include <mutex>
#include <queue>
#include <list>

namespace RPSGame
{

class Rank : public Thread
{
public:
    explicit Rank() = default;
    ~Rank();
    
    void rank(std::multimap<uint64_t, std::string>& rank);
    void update(std::pair<PlayerId, uint64_t> entry);

protected:
    void run() override;

    void update(std::queue<std::pair<PlayerId, uint64_t>>& queue);

    void load();
    void save();

private:
    std::multimap<uint64_t, PlayerId> m_rank_score;
    
    std::mutex m_rank_lock;
    std::multimap<uint64_t, std::string> m_rank;
    
    std::mutex m_queue_lock;
    std::condition_variable cv;
    std::queue<std::pair<PlayerId, uint64_t> > m_queue;
};

}
