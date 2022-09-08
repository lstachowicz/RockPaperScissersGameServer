//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#pragma once

#include "thread.hpp"
#include "inactive_reciver.hpp"

#include <map>
#include <mutex>
#include <queue>

namespace RPSGame
{

constexpr int64_t TIMEOUT = std::chrono::seconds(10).count();

template <class T>
class InactiveDetection : public Thread
{
public:
    InactiveDetection(InactiveReciver<T>* reciver)
        : m_reciver(reciver)
    {
        if (m_reciver == nullptr)
        {
            throw std::runtime_error("InactiveDetection<T>: Cannot notify nullptr reciver");
        }
    }
    
    ~InactiveDetection()
    {
        if (!stopped())
        {
            stop();
        }
    }

    inline void update(const T& object)
    {
        {
            std::scoped_lock<std::mutex> lock(m_queue_lock);
            m_queue.push(object);
        }
    }
    
protected:
    void run()
    {
        std::set<T> removed_objects;
        while(!stopped())
        {
            const auto time = std::chrono::system_clock::now();
            const auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
            const auto minimum_timestamp = timestamp - TIMEOUT;
            
            std::queue<T> objects;
            {
                std::scoped_lock<std::mutex> lock(m_queue_lock);
                m_queue.swap(objects);
            }
            
            while (!objects.empty())
            {
                updateInternal(objects.front(), timestamp);
                objects.pop();
            }
            
            {
                auto bound = m_times.lower_bound(minimum_timestamp);
                for (auto it = m_times.begin(); it != bound; ++it)
                {
                    m_objects.erase(it->second);
                    removed_objects.insert(it->second);
                }
                m_times.erase(m_times.begin(), bound);
            }

            m_reciver->inactive(removed_objects);
            removed_objects.clear();

            {
                std::unique_lock<std::mutex> lock(m_queue_lock);
                while (m_queue.empty() && !stopped())
                {
                    cv.wait_for(lock, std::chrono::seconds(10));
                }
                lock.unlock();
            }
        }
    }
    
    void updateInternal(const T& object, u_int64_t timestamp)
    {
        auto it = m_objects.find(object);
        if (it != m_objects.end())
        {
            auto range = m_times.equal_range(it->second);
            for (auto jt = range.first; jt != range.second; ++jt)
            {
                if (jt->second == object)
                {
                    m_times.erase(jt);
                    break;
                }
            }
        }
        
        m_times.insert({ timestamp, object });
        m_objects[object] = timestamp;
    }
    
private:
    InactiveReciver<T>* m_reciver;
    
    std::mutex m_queue_lock;
    std::condition_variable cv;
    std::queue<T> m_queue;

    std::map<T, uint64_t> m_objects;
    std::multimap<uint64_t, T> m_times;
};

}
