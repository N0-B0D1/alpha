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
    {

    }
    HIDContextManager::~HIDContextManager() { }

    void HIDContextManager::KeyboardButtonUp(const HIDAction * const action)
    {
        /*
        //LOG("Key release - ", code);
        auto state = m_pDefaultContext->TranslateKeyboardState(code);
        if (state)
        {
            // dispatch state released
            LOG("KEY STATE RELEASED: ", state->name);
        }
        */

        LOG("KEY STATE RELEASED: ", action->name);
    }

    void HIDContextManager::KeyboardButtonDown(const HIDAction * const action)
    {
        /*
        //LOG("Key pressed - ", code);
        auto action = m_pDefaultContext->TranslateKeyboardAction(code);
        if (action)
        {
            // dispatch state released
            LOG("KEY ACTION: ", action->name);
        }
        auto state = m_pDefaultContext->TranslateKeyboardState(code);
        if (state)
        {
            // dispatch state released
            LOG("KEY STATE INITIATED: ", state->name);
        }
        */

        LOG("KEY STATE INITIATED: ", action->name);
    }

    void HIDContextManager::MouseButtonDown(const HIDAction * const action)
    {
        LOG("KEY STATE INITIATED: ", action->name);
    }

    void HIDContextManager::MouseButtonUp(const HIDAction * const action)
    {
        LOG("KEY STATE RELEASED: ", action->name);
    }

    void HIDContextManager::MouseMoved(const HIDAction * const action, long relative, float absolute)
    {
        LOG("Mouse: ", action->name, " ", relative, " - ", absolute);
    }
}