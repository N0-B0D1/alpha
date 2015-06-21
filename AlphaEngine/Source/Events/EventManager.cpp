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

#include <algorithm>

#include "Events/EventManager.h"
#include "Events/EventInterface.h"
#include "Events/AEvent.h"

namespace alpha
{
    EventManager::~EventManager() { }


        
    bool EventManager::Initialize()
    {
        return true;
    }

    bool EventManager::Update()
    {
        std::vector<AEvent *> events;

        // for each interface, gather all new outgoing events
        for (auto pEventInterface : m_vInterfaces)
        {
            AEvent * pEvent;
            while (pEventInterface->m_qOutgoingEvents.TryPop(pEvent))
            {
                events.push_back(pEvent);
            }
        }

        // once all events are gathered, publish each event too all interfaces
        while (events.size() > 0)
        {
            auto pEvent = events.back();
            for (auto pEventInterface : m_vInterfaces)
            {
                pEventInterface->m_qIncomingEvents.Push(pEvent->VCopy());
            }

            events.pop_back();
            delete pEvent;
        }

        return true;
    }

    bool EventManager::Shutdown()
    {
        return true;
    }

    /** Register an interface so that it can receive events. */
    void EventManager::RegisterEventInterface(EventInterface * const pEventInterface)
    {
        auto it = std::find(m_vInterfaces.begin(), m_vInterfaces.end(), pEventInterface);
        if (it == m_vInterfaces.end())
        {
            m_vInterfaces.push_back(pEventInterface);
        }
    }
    
    /** Remove the event interface from the manager so that it no longer recieves events */
    void EventManager::UnregisterEventInterface(EventInterface * const pEventInterface)
    {
        auto it = std::find(m_vInterfaces.begin(), m_vInterfaces.end(), pEventInterface);
        if (it != m_vInterfaces.end())
        {
            m_vInterfaces.erase(it);
        }
    }
}
