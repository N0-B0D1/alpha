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
#include "HID/HIDSystemEvents.h"

namespace alpha
{
    HIDSystem::HIDSystem()
        : AlphaSystem(60)
        , m_pWindowListener(nullptr)
    { }
    HIDSystem::~HIDSystem() { }

    bool HIDSystem::VInitialize()
    {
        //m_pWindowListener = new HIDWindowListener(m_pubHIDKeyAction);
        m_pWindowListener = new HIDWindowListener([this](HID device, const HIDAction & action, bool pressed) { this->DispatchHIDActionKeyEvent(device, action, pressed); },
                                                  [this](HID device, const HIDAction & action, float relative, float absolute) { this->DispatchHIDActionAxisEvent(device, action, relative, absolute); });

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

    bool HIDSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        return m_pWindowListener->Update();
    }

    void HIDSystem::DispatchHIDActionKeyEvent(HID device, const HIDAction & action, bool pressed)
    {
        this->PublishEvent(new Event_HIDKeyAction(device, action, pressed));
    }

    void HIDSystem::DispatchHIDActionAxisEvent(HID device, const HIDAction & action, float relative, float absolute)
    {
        this->PublishEvent(new Event_HIDKeyAction(device, action, false, relative, absolute));
    }
}
