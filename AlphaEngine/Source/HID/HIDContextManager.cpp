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

#include "HID/HIDContextManager.h"
#include "HID/HIDContext.h"
#include "HID/HIDTypes.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    HIDContextManager::HIDContextManager()
        : m_pActiveContext(nullptr)
    { }
    HIDContextManager::~HIDContextManager() { }

    void HIDContextManager::SetActiveContext(HIDContext * context)
    {
        m_pActiveContext = context;
    }

    void HIDContextManager::KeyboardButtonUp(const HIDAction * const action)
    {
        //LOG("KEY STATE RELEASED: ", action->name);

        if (m_pActiveContext)
        {
            auto state = m_pActiveContext->TranslateState(action);
            if (state)
            {
                // dispatch action state released
                auto it = m_stateDelegates.find(state->name);
                if (it != m_stateDelegates.end())
                {
                    for (auto func : it->second)
                    {
                        // send key up event to delegate
                        func(false);
                    }
                }
            }
        }
    }

    void HIDContextManager::KeyboardButtonDown(const HIDAction * const action)
    {
        //LOG("KEY STATE INITIATED: ", action->name);

        if (m_pActiveContext)
        {
            //LOG("Key pressed - ", code);
            auto contextAction = m_pActiveContext->TranslateAction(action);
            if (contextAction)
            {
                // dispatch key action
                auto it = m_actionDelegates.find(contextAction->name);
                if (it != m_actionDelegates.end())
                {
                    for (auto func : it->second)
                    {
                        // send key action event to delegate
                        func();
                    }
                }
            }

            // translate any states associated with this action
            auto state = m_pActiveContext->TranslateState(action);
            if (state)
            {
                // dispatch state pressed
                auto it = m_stateDelegates.find(state->name);
                if (it != m_stateDelegates.end())
                {
                    for (auto func : it->second)
                    {
                        // send key action event to delegate
                        func(true);
                    }
                }
            }
        }
    }

    void HIDContextManager::MouseButtonDown(const HIDAction * const /*action*/)
    {
        //LOG("KEY STATE INITIATED: ", action->name);
    }

    void HIDContextManager::MouseButtonUp(const HIDAction * const /*action*/)
    {
        //LOG("KEY STATE RELEASED: ", action->name);
    }

    void HIDContextManager::MouseMoved(const HIDAction * const /*action*/, float /*relative*/, float /*absolute*/)
    {
        //LOG("Mouse: ", action->name, " ", relative, " - ", absolute);
    }

    void HIDContextManager::BindAction(std::string action, std::function<void()> delegate)
    {
        m_actionDelegates[action].push_back(delegate);
    }

    void HIDContextManager::BindState(std::string state, std::function<void(bool)> delegate)
    {
        m_stateDelegates[state].push_back(delegate);
    }

    void HIDContextManager::BindRange(std::string range, std::function<void(long, float)> delegate)
    {
        m_rangeDelegates[range].push_back(delegate);
    }
}