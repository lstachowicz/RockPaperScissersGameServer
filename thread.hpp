//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#pragma once

#include <thread>

namespace RPSGame
{

class Thread
{
public:
    virtual ~Thread();
    
    void start();
    void stop();
    
    inline bool stopped() const { return m_stop; }
    
protected:
    virtual void run() = 0;
    
private:
    inline void runInternal() { this->run(); }

    bool m_stop;
    std::thread m_thread;
};

}
