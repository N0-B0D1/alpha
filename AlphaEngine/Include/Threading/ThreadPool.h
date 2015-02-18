#ifndef ALPHA_THREAD_POOL_H
#define ALPHA_THREAD_POOL_H

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

namespace alpha
{
    class TaskRunner;

    /**
     * \brief The ThreadPool maintains a pre-defined number of threads and passes Tasks to them as needed.
     */
    class ThreadPool
    {
    public:
        ThreadPool();
        virtual ~ThreadPool();

        /** Create pool of available task runner threads. */
        bool Initialize();
        /** Join all threads and dispose of them. */
        bool Shutdown();

    private:
        /** Number of supported hardware threads. */
        unsigned m_maxThreads;

        /** Array of TaskRunner threads. */
        std::vector<std::thread> m_threads;
        /** Array of all TaskRunners that are executing in threads. */
        std::vector<TaskRunner> m_runners;

        /** Thread running state, setting to false will stop all task runner activity. */
        bool m_running;
    };
}

#endif
