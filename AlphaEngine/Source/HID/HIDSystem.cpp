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

#include "HID/HIDSystem.h"
#include "HID/HIDWindowListener.h"
#include "HID/HIDContextManager.h"

namespace alpha
{
    HIDSystem::HIDSystem()
        : AlphaSystem(60)
        , m_pWindowListener(nullptr)
    { }
    HIDSystem::~HIDSystem() { }

    bool HIDSystem::VInitialize()
    {
        m_pWindowListener = new HIDWindowListener(m_pubHIDKeyAction);

        return true;
    }

    bool HIDSystem::VShutdown()
    {
        if (m_pWindowListener != nullptr)
        {
            delete m_pWindowListener;
        }
        return true;
    }

    void HIDSystem::SubscribeToHIDKeyAction(std::shared_ptr<AEventDataSubscriber> pSubscriber)
    {
        m_pubHIDKeyAction.Subscribe(pSubscriber);
    }

    bool HIDSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        m_pWindowListener->Update();
        return true;
    }
}