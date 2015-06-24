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
    TaskRunner::TaskRunner(const bool * const running, const int * const currentQueue, std::shared_ptr<ConcurrentQueue<ATask *> > pTaskQueue[2])
        : m_running(running)
        , m_currentQueue(currentQueue)
    {
        m_pTaskQueue[0] = pTaskQueue[0];
        m_pTaskQueue[1] = pTaskQueue[1];
    }
    TaskRunner::~TaskRunner() { }

    void TaskRunner::operator()()
    {
        LOG("Executing task runner thread.");
        while ((*m_running) == true)
        {
            // store the original value of the current queue
            // up front so that we can put the task on the next
            // queue.
            int current_queue = (*m_currentQueue);

            // pickup tasks from threading system queue
            ATask * pTask;
            if (m_pTaskQueue[current_queue]->TryPop(pTask))
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
                    // if not complete after this iteration, push the task onto the next 
                    // queue, so that it can be processed again on the next update loop.
                    m_pTaskQueue[(current_queue + 1) % 2]->Push(pTask);
                }
            }
            else
            {
                // when no task is available to process, sleep for 1 second
                // so that we don't hog all the system resources
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        LOG("Shutting down task runner thread.");
    }
}
