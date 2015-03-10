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
    HIDContext::HIDContext() { }
    HIDContext::~HIDContext()
    {
        for (auto action : m_actions)
        {
            if (action.second)
            {
                delete action.second;
            }
        }
        m_actions.clear();

        for (auto action : m_states)
        {
            if (action.second)
            {
                delete action.second;
            }
        }
        m_states.clear();

        for (auto action : m_ranges)
        {
            if (action.second)
            {
                delete action.second;
            }
        }
        m_ranges.clear();
    }

    HIDContextAction * HIDContext::TranslateAction(const HIDAction * action) const
    {
        auto it = m_actions.find(action->name);
        if (it != m_actions.end())
        {
            return it->second;
        }
        return nullptr;
    }

    HIDContextAction * HIDContext::TranslateState(const HIDAction * action) const
    {
        auto it = m_states.find(action->name);
        if (it != m_states.end())
        {
            return it->second;
        }
        return nullptr;
    }

    HIDContextAction * HIDContext::TranslateRange(const HIDAction * action) const
    {
        auto it = m_ranges.find(action->name);
        if (it != m_ranges.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void HIDContext::MapAction(const std::string & action, std::vector<std::string> mappings)
    {
        for (auto actionMap : mappings)
        {
            HIDContextAction * contextualAction = new HIDContextAction();
            contextualAction->name = action;

            auto it = m_actions.find(actionMap);
            if (it == m_actions.end())
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
        for (auto actionMap : mappings)
        {
            HIDContextAction * contextualAction = new HIDContextAction();
            contextualAction->name = action;

            auto it = m_states.find(actionMap);
            if (it == m_states.end())
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
        for (auto actionMap : mappings)
        {
            HIDContextAction * contextualAction = new HIDContextAction();
            contextualAction->name = action;

            auto it = m_ranges.find(actionMap);
            if (it == m_ranges.end())
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
}