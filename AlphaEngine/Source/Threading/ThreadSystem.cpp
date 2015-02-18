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

#include "Threading/ThreadSystem.h"
#include "Threading/ThreadPool.h"

namespace alpha
{
    ThreadSystem::ThreadSystem() : AlphaSystem(60) { }
    ThreadSystem::~ThreadSystem() { }

    bool ThreadSystem::VInitialize()
    {
        // create a thread pool to manage threads as resources
        m_pThreadPool = new ThreadPool();
        if (!m_pThreadPool->Initialize())
        {
            return false;
        }
        return true;
    }

    bool ThreadSystem::VShutdown()
    {
        // join/exit any running threads, and delete the thread pool
        if (m_pThreadPool)
        {
            m_pThreadPool->Shutdown();
            delete m_pThreadPool;
        }
        return true;
    }

    bool ThreadSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        // queue up new tasks for the thread pool
        return true;
    }
}
