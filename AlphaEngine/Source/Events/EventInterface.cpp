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

#include "Events/EventInterface.h"
#include "Events/AEvent.h"

namespace alpha
{
    EventInterface::~EventInterface() { }

    void EventInterface::PublishEvent(AEvent * pEvent)
    {
        m_qOutgoingEvents.Push(pEvent);
    }

    AEvent * EventInterface::GetNextEvent()
    {
        AEvent * pEvent;
        if (m_qIncomingEvents.TryPop(pEvent))
        {
            return pEvent;
        }
        return nullptr;
    }
}
