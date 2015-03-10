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
#include "Toolbox/Logger.h"

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
        /*
        for (auto pair : m_actions)
        {
            delete pair.second;
        }
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

    HIDContextAction * HIDContext::TranslateAction(const HIDAction * /*action*/) const
    {
        return nullptr;
    }

    HIDContextAction * HIDContext::TranslateState(const HIDAction * /*action*/) const
    {
        return nullptr;
    }

    HIDContextAction * HIDContext::TranslateRange(const HIDAction * /*action*/) const
    {
        return nullptr;
    }

    void HIDContext::MapAction(const std::string & action, std::vector<std::string> mappings)
    {
        HIDContextAction contextualAction;
        contextualAction.name = action;

        for (auto actionMap : mappings)
        {
            auto it = m_actions.find(actionMap);
            if (it != m_actions.end())
            {
                // map action
                m_actions[actionMap] = contextualAction;
            }
            else
            {
                // log warning and don't remap
                LOG_WARN("Attempt to re-map ACTION [", actionMap, "] to [", action, "]");
            }
        }
    }

    void HIDContext::MapState(const std::string & action, std::vector<std::string> mappings)
    {
        HIDContextAction contextualAction;
        contextualAction.name = action;

        for (auto actionMap : mappings)
        {
            auto it = m_states.find(actionMap);
            if (it != m_states.end())
            {
                // map action
                m_states[actionMap] = contextualAction;
            }
            else
            {
                // log warning and don't remap
                LOG_WARN("Attempt to re-map STATE [", actionMap, "] to [", action, "]");
            }
        }
    }

    void HIDContext::MapRange(const std::string & action, std::vector<std::string> mappings)
    {

        HIDContextAction contextualAction;
        contextualAction.name = action;

        for (auto actionMap : mappings)
        {
            auto it = m_ranges.find(actionMap);
            if (it != m_ranges.end())
            {
                // map action
                m_ranges[actionMap] = contextualAction;
            }
            else
            {
                // log warning and don't remap
                LOG_WARN("Attempt to re-map RANGE [", actionMap, "] to [", action, "]");
            }
        }
    }

    /*
    HIDAction * HIDContext::TranslateKeyboardAction(const unsigned short & code)
    {
        auto it = m_actions.find(code);
        if (it != m_actions.end())
        {
            return it->second;
        }
        return nullptr;
    }

    HIDActionState * HIDContext::TranslateKeyboardState(const unsigned short & code)
    {
        auto it = m_states.find(code);
        if (it != m_states.end())
        {
            return it->second;
        }
        return nullptr;
    }

    HIDAction * HIDContext::TranslateMouseAction(const unsigned short & code)
    {
        // XXX not implemented
        return nullptr;
    }

    HIDActionState * HIDContext::TranslateMouseState(const unsigned short & code)
    {
        // XXX not implemented
        return nullptr;
    }
    */
}