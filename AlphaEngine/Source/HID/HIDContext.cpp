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

#include "HID/HIDContext.h"
#include "HID/HIDTypes.h"

namespace alpha
{
    //HIDActionState::HIDActionState(std::string actionName, bool actionPressed /*= false*/)
    //    : HIDAction(actionName)
    //    , pressed(actionPressed)
    //{ }
    //HIDActionRange::HIDActionRange(std::string actionName, float actionRange /*= 0.f*/)
    //    : HIDAction(actionName)
    //    , range(actionRange)
    //{ }

    HIDContext::HIDContext() { }
    HIDContext::~HIDContext()
    {
        for (auto pair : m_actions)
        {
            delete pair.second;
        }
        /*
        for (auto pair : m_states)
        {
            delete pair.second;
        }
        for (auto pair : m_ranges)
        {
            delete pair.second;
        }
        */
    }

    HIDAction * HIDContext::TranslateKeyboardAction(const unsigned short & code)
    {
        auto it = m_actions.find(code);
        if (it != m_actions.end())
        {
            return it->second;
        }
        return nullptr;
    }

    /*
    HIDActionState * HIDContext::TranslateKeyboardState(const unsigned short & code)
    {
        auto it = m_states.find(code);
        if (it != m_states.end())
        {
            return it->second;
        }
        return nullptr;
    }
    */

    HIDAction * HIDContext::TranslateMouseAction(const unsigned short & /*code*/)
    {
        // XXX not implemented
        return nullptr;
    }

    /*
    HIDActionState * HIDContext::TranslateMouseState(const unsigned short & code)
    {
        // XXX not implemented
        return nullptr;
    }
    */
}