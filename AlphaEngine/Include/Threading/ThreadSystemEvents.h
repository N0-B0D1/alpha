#ifndef ALPHA_THREAD_SYSTEM_EVENTS_H
#define ALPHA_THREAD_SYSTEM_EVENTS_H

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

#include "Events/AEvent.h"

namespace alpha
{
    class ATask;

    /**
    * Event_NewThreadTask
    */
    class Event_NewThreadTask : public AEvent
    {
    public:
        static const std::string sk_name;

        explicit Event_NewThreadTask(ATask * pTask);

        virtual std::string VGetTypeName() const;
        virtual AEvent * VCopy();

        /** Retrieve the task to be executed in a thread. */
        ATask * GetTask() const;

    private:
        ATask * m_pTask;
    };
}

#endif // ALPHA_THREAD_SYSTEM_EVENTS_H