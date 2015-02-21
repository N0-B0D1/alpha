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
#include "Threading/Task.h"

#include "Toolbox/Logger.h"

namespace alpha
{
    TaskRunner::TaskRunner(const bool * const running, std::shared_ptr<ConcurrentQueue<Task *> > pTaskQueue)
        : m_running(running)
        , m_pTaskQueue(pTaskQueue)
    { }
    TaskRunner::~TaskRunner() { }

    void TaskRunner::operator()()
    {
        LOG("Executing task runner thread.");
        while ((*m_running) == true)
        {
            // pickup tasks from threading system queue
            Task * pTask;
            if (m_pTaskQueue->TryPop(pTask))
            {
                //LOG("Thread got new task to process.");
                // execute the task, all task logic should be self contained
                // exiting the execute method ammounts to completing the task.
                pTask->VExecute();

                // once done, destroy the task
                if (pTask) { delete pTask; }
            }
            else
            {
                //LOG("Thread waiting ....");
                // when no task is available to process, sleep for 1 second
                // so that we don't hog all the system resources
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        LOG("Shutting down task runner thread.");
    }
}
