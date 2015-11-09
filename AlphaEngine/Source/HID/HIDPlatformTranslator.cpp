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

#include "HID/HIDPlatformTranslator.h"
#include "HID/HIDTypes.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    HIDPlatformTranslator::HIDPlatformTranslator()
    {
        // add some default states, ranges, and actions

        // control keys
        CreateHIDAction(HID_KEYBOARD, KA_BACKSPACE, "KA_BACKSPACE");
        CreateHIDAction(HID_KEYBOARD, KA_TAB, "KA_TAB");
        CreateHIDAction(HID_KEYBOARD, KA_ENTER, "KA_ENTER");
        CreateHIDAction(HID_KEYBOARD, KA_LEFT_SHIFT, "KA_LEFT_SHIFT");
        CreateHIDAction(HID_KEYBOARD, KA_LEFT_CTRL, "KA_LEFT_CTRL");
        CreateHIDAction(HID_KEYBOARD, KA_LEFT_ALT, "KA_LEFT_ALT");
        CreateHIDAction(HID_KEYBOARD, KA_RIGHT_SHIFT, "KA_RIGHT_SHIFT");
        CreateHIDAction(HID_KEYBOARD, KA_RIGHT_CTRL, "KA_RIGHT_CTRL");
        CreateHIDAction(HID_KEYBOARD, KA_RIGHT_ALT, "KA_RIGHT_ALT");
        CreateHIDAction(HID_KEYBOARD, KA_CAPS_LOCK, "KA_CAPS_LOCK");
        CreateHIDAction(HID_KEYBOARD, KA_LEFT_SUPER, "KA_LEFT_SUPER"); // windows key

        // escape
        CreateHIDAction(HID_KEYBOARD, KA_ESCAPE, "KA_ESCAPE");

        // arrow keys
        CreateHIDAction(HID_KEYBOARD, KA_LEFT, "KA_LEFT");
        CreateHIDAction(HID_KEYBOARD, KA_UP, "KA_UP");
        CreateHIDAction(HID_KEYBOARD, KA_RIGHT, "KA_RIGHT");
        CreateHIDAction(HID_KEYBOARD, KA_DOWN, "KA_DOWN");

        // word processor modifiers
        CreateHIDAction(HID_KEYBOARD, KA_SPACE, "KA_SPACE");
        CreateHIDAction(HID_KEYBOARD, KA_INSERT, "KA_INSERT");
        CreateHIDAction(HID_KEYBOARD, KA_DELETE, "KA_DELETE");
        CreateHIDAction(HID_KEYBOARD, KA_HOME, "KA_HOME");
        CreateHIDAction(HID_KEYBOARD, KA_END, "KA_END");
        CreateHIDAction(HID_KEYBOARD, KA_PAGE_UP, "KA_PAGE_UP");
        CreateHIDAction(HID_KEYBOARD, KA_PAGE_DOWN, "KA_PAGE_DOWN");

        // number row
        CreateHIDAction(HID_KEYBOARD, KA_0, "KA_0");
        CreateHIDAction(HID_KEYBOARD, KA_1, "KA_1");
        CreateHIDAction(HID_KEYBOARD, KA_2, "KA_2");
        CreateHIDAction(HID_KEYBOARD, KA_3, "KA_3");
        CreateHIDAction(HID_KEYBOARD, KA_4, "KA_4");
        CreateHIDAction(HID_KEYBOARD, KA_5, "KA_5");
        CreateHIDAction(HID_KEYBOARD, KA_6, "KA_6");
        CreateHIDAction(HID_KEYBOARD, KA_7, "KA_7");
        CreateHIDAction(HID_KEYBOARD, KA_8, "KA_8");
        CreateHIDAction(HID_KEYBOARD, KA_9, "KA_9");

        // alphabet keys
        CreateHIDAction(HID_KEYBOARD, KA_A, "KA_A");
        CreateHIDAction(HID_KEYBOARD, KA_B, "KA_B");
        CreateHIDAction(HID_KEYBOARD, KA_C, "KA_C");
        CreateHIDAction(HID_KEYBOARD, KA_D, "KA_D");
        CreateHIDAction(HID_KEYBOARD, KA_E, "KA_E");
        CreateHIDAction(HID_KEYBOARD, KA_F, "KA_F");
        CreateHIDAction(HID_KEYBOARD, KA_G, "KA_G");
        CreateHIDAction(HID_KEYBOARD, KA_H, "KA_H");
        CreateHIDAction(HID_KEYBOARD, KA_I, "KA_I");
        CreateHIDAction(HID_KEYBOARD, KA_J, "KA_J");
        CreateHIDAction(HID_KEYBOARD, KA_K, "KA_K");
        CreateHIDAction(HID_KEYBOARD, KA_L, "KA_L");
        CreateHIDAction(HID_KEYBOARD, KA_M, "KA_M");
        CreateHIDAction(HID_KEYBOARD, KA_N, "KA_N");
        CreateHIDAction(HID_KEYBOARD, KA_O, "KA_O");
        CreateHIDAction(HID_KEYBOARD, KA_P, "KA_P");
        CreateHIDAction(HID_KEYBOARD, KA_Q, "KA_Q");
        CreateHIDAction(HID_KEYBOARD, KA_R, "KA_R");
        CreateHIDAction(HID_KEYBOARD, KA_S, "KA_S");
        CreateHIDAction(HID_KEYBOARD, KA_T, "KA_T");
        CreateHIDAction(HID_KEYBOARD, KA_U, "KA_U");
        CreateHIDAction(HID_KEYBOARD, KA_V, "KA_V");
        CreateHIDAction(HID_KEYBOARD, KA_W, "KA_W");
        CreateHIDAction(HID_KEYBOARD, KA_X, "KA_X");
        CreateHIDAction(HID_KEYBOARD, KA_Y, "KA_Y");
        CreateHIDAction(HID_KEYBOARD, KA_Z, "KA_Z");

        // function keys
        CreateHIDAction(HID_KEYBOARD, KA_F1, "KA_F1");
        CreateHIDAction(HID_KEYBOARD, KA_F2, "KA_F2");
        CreateHIDAction(HID_KEYBOARD, KA_F3, "KA_F3");
        CreateHIDAction(HID_KEYBOARD, KA_F4, "KA_F4");
        CreateHIDAction(HID_KEYBOARD, KA_F5, "KA_F5");
        CreateHIDAction(HID_KEYBOARD, KA_F6, "KA_F6");
        CreateHIDAction(HID_KEYBOARD, KA_F7, "KA_F7");
        CreateHIDAction(HID_KEYBOARD, KA_F8, "KA_F8");
        CreateHIDAction(HID_KEYBOARD, KA_F9, "KA_F9");
        CreateHIDAction(HID_KEYBOARD, KA_F10, "KA_F10");
        CreateHIDAction(HID_KEYBOARD, KA_F11, "KA_F11");
        CreateHIDAction(HID_KEYBOARD, KA_F12, "KA_F12");

        // special characters
        CreateHIDAction(HID_KEYBOARD, KA_SEMICOLON, "KA_SEMICOLON");
        CreateHIDAction(HID_KEYBOARD, KA_EQUAL, "KA_EQUAL");
        CreateHIDAction(HID_KEYBOARD, KA_COMMA, "KA_COMMA");
        CreateHIDAction(HID_KEYBOARD, KA_MINUS, "KA_MINUS");
        CreateHIDAction(HID_KEYBOARD, KA_PERIOD, "KA_PERIOD");
        CreateHIDAction(HID_KEYBOARD, KA_SLASH, "KA_SLASH");
        CreateHIDAction(HID_KEYBOARD, KA_GRAVE_ACCENT, "KA_GRAVE_ACCENT");
        CreateHIDAction(HID_KEYBOARD, KA_LEFT_BRACKET, "KA_LEFT_BRACKET");
        CreateHIDAction(HID_KEYBOARD, KA_BACKSLASH, "KA_BACKSLASH");
        CreateHIDAction(HID_KEYBOARD, KA_RIGHT_BRACKET, "KA_RIGHT_BRACKET");
        CreateHIDAction(HID_KEYBOARD, KA_APOSTROPHE, "KA_APOSTROPHE");

        // numpad
        CreateHIDAction(HID_KEYBOARD, KA_NUM_0, "KB_NUM_0");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_1, "KA_NUM_1");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_2, "KA_NUM_2");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_3, "KA_NUM_3");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_4, "KA_NUM_4");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_5, "KA_NUM_5");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_6, "KA_NUM_6");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_7, "KA_NUM_7");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_8, "KA_NUM_8");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_9, "KA_NUM_9");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_DECIMAL, "KA_NUM_DECIMAL");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_ENTER, "KA_NUM_ENTER");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_ADD, "KA_NUM_ADD");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_SUBTRACT, "KA_NUM_SUBTRACT");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_MULTIPLY, "KA_NUM_MULTIPLY");
        CreateHIDAction(HID_KEYBOARD, KA_NUM_DIVIDE, "KA_NUM_DIVIDE");

        // mouse input mapping
        CreateHIDAction(HID_MOUSE, MA_X_AXIS, "MA_X_AXIS");
        CreateHIDAction(HID_MOUSE, MA_Y_AXIS, "MA_Y_AXIS");
        CreateHIDAction(HID_MOUSE, MA_1, "MA_1"); // MA_LEFT
        CreateHIDAction(HID_MOUSE, MA_2, "MA_2"); // MA_MIDDLE
        CreateHIDAction(HID_MOUSE, MA_3, "MA_3"); // MA_RIGHT
        CreateHIDAction(HID_MOUSE, MA_4, "MA_4");
        CreateHIDAction(HID_MOUSE, MA_5, "MA_5");
        //CreateHIDAction(HID_MOUSE, MA_6, "MA_6");
        //CreateHIDAction(HID_MOUSE, MA_7, "MA_7");
        //CreateHIDAction(HID_MOUSE, MA_8, "MA_8");
        CreateHIDAction(HID_MOUSE, MA_WHEEL_FORWARD, "MA_WHEEL_FORWARD");
        CreateHIDAction(HID_MOUSE, MA_WHEEL_BACK, "MA_WHEEL_BACK");
        CreateHIDAction(HID_MOUSE, MA_WHEEL_LEFT, "MA_WHEEL_LEFT");
        CreateHIDAction(HID_MOUSE, MA_WHEEL_RIGHT, "MA_WHEEL_RIGHT");
    }

    HIDPlatformTranslator::~HIDPlatformTranslator()
    {
        for (auto actionMap : m_actions)
        {
            for (auto pair : actionMap)
            {
                delete pair.second;
            }
            actionMap.clear();
        }
    }

    HIDAction * HIDPlatformTranslator::TranslateKeyboardCode(const unsigned int & code)
    {
        return TranslateCode(HID_KEYBOARD, code);
    }

    HIDAction * HIDPlatformTranslator::TranslateMouseCode(const unsigned int & code)
    {
        return TranslateCode(HID_MOUSE, code);
    }

    HIDAction * HIDPlatformTranslator::TranslateCode(const HID & device, const unsigned int & code)
    {
        auto it = m_actions[device].find(code);
        if (it != m_actions[device].end())
        {
            //LOG_WARN("action -> ", it->second->name);
            return it->second;
        }
        return nullptr;
    }

    void HIDPlatformTranslator::CreateHIDAction(HID device, unsigned int code, const std::string & name)
    {
        m_actions[device][code] = new HIDAction(code, name);
    }
}
