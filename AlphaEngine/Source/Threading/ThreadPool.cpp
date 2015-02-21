/**
Copyright 2014-2015 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <thread>
#include <vector>

#include "Threading/ThreadPool.h"
#include "Threading/TaskRunner.h"
#include "Threading/Task.h"
#include "Toolbox/ConcurrentQueue.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    ThreadPool::ThreadPool()
        : m_pTaskQueue(nullptr)
        , m_running(true)
    { }
    ThreadPool::~ThreadPool() { }

    bool ThreadPool::Initialize()
    {
        // set the max number of hardware threads for the current machine
        // if zero/not computable, make a minimum of 1 thread.
        m_maxThreads = std::thread::hardware_concurrency();
        if (m_maxThreads == 0)
        {
            m_maxThreads = 1;
        }
        LOG("  ThreadPool > Detected ", m_maxThreads, " max possible hardware threads.");

        // create the task queue
        m_pTaskQueue = std::make_shared<ConcurrentQueue<Task *> >();

        // Create a TaskRunner thread for each hardware thread available.
        for (unsigned i = 0; i < m_maxThreads; ++i)
        {
            m_threads.push_back(std::thread(TaskRunner(&m_running, m_pTaskQueue)));
        }

        return true;
    }

    bool ThreadPool::Shutdown()
    {
        // simply set the shared runnning boolean to false
        // and join on all threads.  Since all TaskRunner's
        // have a reference to this boolean, setting this to
        // false amounts to telling all threads to stop execution.
        m_running = false;

        // join on each thread
        while (m_threads.size() > 0)
        {
            std::thread &t = m_threads.back();
            
            if (t.joinable())
            {
                t.join();
            }
            else
            {
                LOG_ERR("Thread not joinable!");
            }

            // pop last, so we don't destruct the thread before joining
            m_threads.pop_back();
        }

        return true;
    }

    void ThreadPool::QueueTask(Task * pTask)
    {
        m_pTaskQueue->Push(pTask);
    }
}
