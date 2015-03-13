#ifndef ALPHA_EVENT_DATA_HID_KEY_DOWN_H
#define ALPHA_EVENT_DATA_HID_KEY_DOWN_H

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

#include "Events/EventData.h"
#include "HID/HIDTypes.h"

namespace alpha
{
    struct HIDAction;

    class EventData_HIDKeyAction : public EventData
    {
        /** The type name that defines this event type */
        static const std::string sk_name;

    public:
        explicit EventData_HIDKeyAction(HID device, const HIDAction & action, bool pressed, long relative = 0, float absolute = 0.f);
        virtual std::string VGetTypeName() const;

        HID GetDevice() const;
        const HIDAction & GetAction() const;
        bool GetPressed() const;

        long GetRelative() const;
        float GetAbsolute() const;

    private:
        // non-copyable
        EventData_HIDKeyAction(const EventData_HIDKeyAction&);
        EventData_HIDKeyAction & operator=(const EventData_HIDKeyAction&);

        // normal key up/down values
        HID m_device;
        const HIDAction & m_action;
        bool m_pressed;

        // axis range values
        long m_relative;
        float m_absolute;
    };
}

#endif // ALPHA_EVENT_DATA_HID_KEY_DOWN_H