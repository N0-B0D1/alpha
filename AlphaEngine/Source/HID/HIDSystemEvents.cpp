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

#include "HID/HIDSystemEvents.h"

namespace alpha
{
    const std::string Event_HIDKeyAction::sk_name = "EventData_HIDKeyDown";

    Event_HIDKeyAction::Event_HIDKeyAction(HID device, const HIDAction & action, bool pressed, float relative /*= 0*/, float absolute /*= 0.f*/)
        : m_device(device)
        , m_action(action)
        , m_pressed(pressed)
        , m_relative(relative)
        , m_absolute(absolute)
    { }

    std::string Event_HIDKeyAction::VGetTypeName() const
    {
        return Event_HIDKeyAction::sk_name;
    }

    AEvent * Event_HIDKeyAction::VCopy()
    {
        return new Event_HIDKeyAction(m_device, m_action, m_pressed, m_relative, m_absolute);
    }

    HID Event_HIDKeyAction::GetDevice() const
    {
        return m_device;
    }

    const HIDAction & Event_HIDKeyAction::GetAction() const
    {
        return m_action;
    }

    bool Event_HIDKeyAction::GetPressed() const
    {
        return m_pressed;
    }

    float Event_HIDKeyAction::GetRelative() const
    {
        return m_relative;
    }

    float Event_HIDKeyAction::GetAbsolute() const
    {
        return m_absolute;
    }
}