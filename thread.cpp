//
//  Created by Łukasz Stachowicz on 03/09/2022.
//

#include "thread.hpp"

using namespace RPSGame;

Thread::~Thread()
{
    if (!stopped())
    {
        this->stop();
    }
}

void Thread::start()
{
    m_stop = false;
    m_thread = std::thread(&Thread::runInternal, this);
}

void Thread::stop()
{
    m_stop = true;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}
