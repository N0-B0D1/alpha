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

#include "Events/EventData_ThreadTaskCreated.h"

namespace alpha
{
    const std::string EventData_ThreadTaskCreated::sk_name = "EventData_ThreadTaskCreated";

    EventData_ThreadTaskCreated::EventData_ThreadTaskCreated(std::shared_ptr<Task> task)
        : m_pTask(task)
    { }

    std::string EventData_ThreadTaskCreated::VGetTypeName() const
    {
        return EventData_ThreadTaskCreated::sk_name;
    }

    std::shared_ptr<Task> EventData_ThreadTaskCreated::GetTask() const
    {
        return m_pTask;
    }
}