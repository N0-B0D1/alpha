#ifndef ALPHA_EVENTDATA_THREAD_TASK_CREATED_H
#define ALPHA_EVENTDATA_THREAD_TASK_CREATED_H

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

#include "Events/EventData.h"
#include "Threading/Task.h"

namespace alpha
{
    class EventData_ThreadTaskCreated : public EventData
    {
        /** The type name that defines this event type */
        static const std::string sk_name;

    public:
        explicit EventData_ThreadTaskCreated(Task * task);
        virtual std::string VGetTypeName() const;
        Task * GetTask() const;

    private:
        Task * m_pTask;
    };
}

#endif // ALPHA_EVENTDATA_THREAD_TASK_CREATED_H