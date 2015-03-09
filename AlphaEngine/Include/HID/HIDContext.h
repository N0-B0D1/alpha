#ifndef ALPHA_HID_CONTEXT_H
#define ALPHA_HID_CONTEXT_H

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

#include <map>
#include <string>

namespace alpha
{
    struct HIDAction;

    /**
     * Defines an action state
     * e.g.: strafe_left released
     */
    /*
    struct HIDActionState : public HIDAction
    {
        HIDActionState(std::string actionName, bool actionPressed = false);
        bool pressed;
    };
    */

    /**
     * Defines an action and its range (0.f - 1.f)
     * e.g.: strafe_left @ 0.5f
     */
    /*
    struct HIDActionRange : public HIDAction
    {
        HIDActionRange(std::string actionName, float actionRange = 0.f);
        float range;
    };
    */

    /**
     * HIDContext abstract base class for translating hardware input into an input state or range.
     */
    class HIDContext
    {
    public:
        HIDContext();
        virtual ~HIDContext();

        HIDAction * TranslateKeyboardAction(const unsigned short & code);
        //HIDActionState * TranslateKeyboardState(const unsigned short & code);

        HIDAction * TranslateMouseAction(const unsigned short & code);
        //HIDActionState * TranslateMouseState(const unsigned short & code);
        //HIDActionRange TranslateMouseRange();

    protected:
        std::map<unsigned short, HIDAction *> m_actions;
        //std::map<unsigned short, HIDActionState *> m_states;
        //std::map<unsigned short, HIDActionRange *> m_ranges;
    };
}

#endif // ALPHA_HID_CONTEXT_H