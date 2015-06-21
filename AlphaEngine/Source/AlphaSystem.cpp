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

#include "AlphaSystem.h"
#include "Events/EventManager.h"
#include "Events/EventInterface.h"
#include "Events/AEvent.h"

namespace alpha
{
    AlphaSystem::AlphaSystem(uint8_t hertz)
        : m_hertz(hertz)
        , m_pEventInterface(nullptr)
    {
        m_updateFrequency = 1.0f / m_hertz;
    }
    AlphaSystem::~AlphaSystem() { }

    bool AlphaSystem::Initialize(EventManager * pEventManager)
    {
        m_pEventInterface = new EventInterface();
        pEventManager->RegisterEventInterface(m_pEventInterface);
        return VInitialize();
    }

    bool AlphaSystem::Update(double currentTime, double elapsedTime)
    {
        bool success = true;
        m_elapsedTime += elapsedTime;
        if (m_elapsedTime > m_updateFrequency)
        {
            // Allow any event handler to be called
            HandleEvents();

            // let the sub-system update
            success = this->VUpdate(currentTime, m_updateFrequency);
            m_elapsedTime = m_elapsedTime - m_updateFrequency;
        }
        return success;
    }

    bool AlphaSystem::Shutdown(EventManager * pEventManager)
    {
        bool success = VShutdown();
        if (m_pEventInterface)
        {
            pEventManager->UnregisterEventInterface(m_pEventInterface);
            delete m_pEventInterface;
        }
        return success;
    }

    void AlphaSystem::PublishEvent(AEvent * pEvent)
    {
        if (pEvent)
        {
            m_pEventInterface->PublishEvent(pEvent);
        }
    }

    void AlphaSystem::AddEventHandler(unsigned int event_id, std::function<void(AEvent * const)> handler)
    {
        m_mEventHandlers[event_id] = handler;
    }

    void AlphaSystem::RemoveEventHandler(unsigned int event_id)
    {
        auto it = m_mEventHandlers.find(event_id);
        if (it != m_mEventHandlers.end())
        {
            m_mEventHandlers.erase(it);
        }
    }

    void AlphaSystem::HandleEvents()
    {
        while (auto pEvent = m_pEventInterface->GetNextEvent())
        {
            auto it = m_mEventHandlers.find(pEvent->GetTypeID());
            if (it != m_mEventHandlers.end())
            {
                it->second(pEvent);
            }
            delete pEvent;
        }
    }
}
