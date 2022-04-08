#pragma once

#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>

namespace crystal
{

    class FixedThreadPool
    {
    public:
        struct Task
        {
            std::function<void()>   Job;
        };

        FixedThreadPool(int size);
        ~FixedThreadPool();

        void AddTask(const Task& task);

    private:

        std::vector<std::thread>    m_threadPool;
        std::queue<Task>            m_taskQueue;
        std::mutex                  m_queueLock;
        std::condition_variable     m_condition;
        bool                        m_terminated;
    };
}
