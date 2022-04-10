#include "FixedThreadPool.h"

namespace crystal
{

    FixedThreadPool::FixedThreadPool(int size)
    {
        m_terminated = false;
        for (int i = 0; i < size; i++)
        {
            m_threadPool.push_back(std::thread([this, i]() {
                Task task;
                while (true)
                {
                    {
                        std::unique_lock<std::mutex> lock(m_queueLock);

                        m_condition.wait(lock, [this]() {return !m_taskQueue.empty() || m_terminated; });
                        if (m_terminated)
                        {
                            return;
                        }
                        task = m_taskQueue.front();
                        m_taskQueue.pop();
                    }
                    task.Job();
                }
            }));
        }
    }
    FixedThreadPool::~FixedThreadPool()
    {
        {
            std::lock_guard<std::mutex> lockGuard(m_queueLock);
            m_terminated = true;
        }

        m_condition.notify_all();
        for (auto& thread : m_threadPool)
        {
            thread.join();
        }
    }
    void FixedThreadPool::AddTask(const Task & task)
    {
        {
            std::lock_guard<std::mutex> lockGuard(m_queueLock);
            m_taskQueue.push(task);
        }
        m_condition.notify_one();
    }
}