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

#include "Threading/ThreadSystemEvents.h"
#include "Threading/ATask.h"

namespace alpha
{
    const std::string Event_NewThreadTask::sk_name = "Event_NewThreadTask";

    Event_NewThreadTask::Event_NewThreadTask(ATask * pTask)
        : m_pTask(pTask)
    { }

    std::string Event_NewThreadTask::VGetTypeName() const
    {
        return Event_NewThreadTask::sk_name;
    }

    AEvent * Event_NewThreadTask::VCopy()
    {
        return new Event_NewThreadTask(m_pTask);
    }

    ATask * Event_NewThreadTask::GetTask() const
    {
        return m_pTask;
    }
}