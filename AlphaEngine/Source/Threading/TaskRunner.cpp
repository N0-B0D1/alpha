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

#include <chrono>
#include <thread>

#include "Threading/TaskRunner.h"
#include "Threading/ATask.h"

#include "Toolbox/Logger.h"

namespace alpha
{
    TaskRunner::TaskRunner(const bool * const running, std::shared_ptr<ConcurrentQueue<ATask *> > pTaskQueue, std::shared_ptr<ConcurrentQueue<ATask *> > pReturnQueue)
        : m_running(running)
        , m_pTaskQueue(pTaskQueue)
        , m_pReturnQueue(pReturnQueue)
    { }
    TaskRunner::~TaskRunner() { }

    void TaskRunner::operator()()
    {
        LOG("Executing task runner thread.");
        while ((*m_running) == true)
        {
            // pickup tasks from threading system queue
            ATask * pTask;
            if (m_pTaskQueue->TryPop(pTask))
            {
                //LOG("Thread got new task to process.");
                // execute the task, all task logic should be self contained
                // exiting the execute method ammounts to completing the task.
                pTask->Execute();

                if (pTask->IsComplete())
                {
                    // once done, destroy the task
                    delete pTask;
                }
                else
                {
                    // return the task to the thread pool, so it can be queued
                    // up again to be processed on the next update tick.
                    m_pReturnQueue->Push(pTask);
                }
            }
            else
            {
                // wait for 1 millisecond, which should not effect threading
                // performance since this is much faster than the update
                // frequency of the threading sub-system.  However, this
                // prevents over-utilization of the cpu when there are not
                // enough tasks to operate on.
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
        LOG("Shutting down task runner thread.");
    }
}
