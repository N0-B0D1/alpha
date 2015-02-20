#ifndef ALPHA_TASK_RUNNER_H
#define ALPHA_TASK_RUNNER_H

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

#include <memory>

#include "Toolbox/ConcurrentQueue.h"

namespace alpha
{
    class Task;

    /**
     * \brief TaskRunner defines the threaded task processer
     *
     * A task runner is created in a thread by the thread pool, and will begin
     * requesting tasks from the threading system.  When a task is received the
     * runner will execute the task until it is complete, then request another task.
     */
    class TaskRunner
    {
    public:
        /**
         * \constructor
         * \param running A reference to the main thread pool running variable.
         * \param taskQueue A reference to the main thread pools task queue.  Task runners can safely grab new tasks from this thread safe queue.
         */
        explicit TaskRunner(const bool * const running, std::shared_ptr<ConcurrentQueue<std::shared_ptr<Task> > > pTaskQueue);
        virtual ~TaskRunner();

        /**
         * \brief allow this classed to be called like a function, so a thread can be created from an instance
         */
        void operator()();

    private:
        TaskRunner & operator=(const TaskRunner &);

        const bool * const m_running;
        std::shared_ptr<ConcurrentQueue<std::shared_ptr<Task> > > m_pTaskQueue;
    };
}

#endif // ALPHA_TASK_RUNNER_H
