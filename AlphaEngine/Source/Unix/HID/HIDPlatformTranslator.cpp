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

#include <GLFW/glfw3.h>

#include "HID/HIDPlatformTranslator.h"
#include "HID/HIDTypes.h"

namespace alpha
{
    HIDPlatformTranslator::HIDPlatformTranslator()
    {
        // add some default states, ranges, and actions

        // control keys
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_BACKSPACE, "KB_BACKSPACE");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_TAB, "KB_TAB");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_ENTER, "KB_ENTER");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_LEFT_SHIFT, "KB_SHIFT");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_LEFT_CONTROL, "KB_CTRL");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_LEFT_ALT, "KB_ALT");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_CAPS_LOCK, "KB_CAPLOCK");

        // escape
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_ESCAPE, "KB_ESCAPE");

        // arrow keys
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_LEFT, "KB_LEFT");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_UP, "KB_UP");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_RIGHT, "KB_RIGHT");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_DOWN, "KB_DOWN");

        // word processor modifiers
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_SPACE, "KB_SPACE");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_PAGE_UP, "KB_PAGEUP");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_PAGE_DOWN, "KB_PAGEDOWN");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_END, "KB_END");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_HOME, "KB_HOME");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_INSERT, "KB_INSERT");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_DELETE, "KB_DELETE");

        // number row
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_0, "KB_0");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_1, "KB_1");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_2, "KB_2");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_3, "KB_3");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_4, "KB_4");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_5, "KB_5");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_6, "KB_6");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_7, "KB_7");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_8, "KB_8");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_9, "KB_9");

        // alphabet keys
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_A, "KB_A");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_B, "KB_B");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_C, "KB_C");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_D, "KB_D");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_E, "KB_E");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F, "KB_F");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_G, "KB_G");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_H, "KB_H");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_I, "KB_I");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_J, "KB_J");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_K, "KB_K");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_L, "KB_L");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_M, "KB_M");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_N, "KB_N");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_O, "KB_O");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_P, "KB_P");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_Q, "KB_Q");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_R, "KB_R");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_S, "KB_S");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_T, "KB_T");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_U, "KB_U");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_V, "KB_V");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_W, "KB_W");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_X, "KB_X");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_Y, "KB_Y");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_Z, "KB_Z");

        // function keys
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F1, "KB_F1");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F2, "KB_F2");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F3, "KB_F3");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F4, "KB_F4");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F5, "KB_F5");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F6, "KB_F6");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F7, "KB_F7");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F8, "KB_F8");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F9, "KB_F9");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F10, "KB_F10");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F11, "KB_F11");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_F12, "KB_F12");

        // special characters
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_SEMICOLON, "KB_SEMICOLON");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_EQUAL, "KB_EQUALS");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_COMMA, "KB_COMMA");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_MINUS, "KB_MINUS");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_PERIOD, "KB_PERIOD");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_SLASH, "KB_FORWARDSLASH");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_GRAVE_ACCENT, "KB_TICK");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_LEFT_BRACKET, "KB_LEFTBRACKET");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_BACKSLASH, "KB_BACKSLASH");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_RIGHT_BRACKET, "KB_RIGHTBRACKET");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_APOSTROPHE, "KB_APOSTROPHE");

        // numpad
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_ADD, "KB_NUMPLUS");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_0, "KB_NUM0");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_1, "KB_NUM1");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_2, "KB_NUM2");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_3, "KB_NUM3");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_4, "KB_NUM4");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_5, "KB_NUM5");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_6, "KB_NUM6");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_7, "KB_NUM7");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_8, "KB_NUM8");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_9, "KB_NUM9");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_MULTIPLY, "KB_NUMMULTIPLY");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_ENTER, "KB_NUMENTER");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_SUBTRACT, "KB_NUMMINUS");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_DECIMAL, "KB_NUMPERIOD");
        MapRawToCode(HID_KEYBOARD, GLFW_KEY_KP_DIVIDE, "KB_NUMDIVIDE");

        // mouse input mapping
        MapRawToCode(HID_MOUSE, MC_XAXIS, "MC_XAXIS");
        MapRawToCode(HID_MOUSE, MC_YAXIS, "MC_YAXIS");
        MapRawToCode(HID_MOUSE, MC_LEFT, "MC_LEFT");
        MapRawToCode(HID_MOUSE, MC_MIDDLE, "MC_MIDDLE");
        MapRawToCode(HID_MOUSE, MC_RIGHT, "MC_RIGHT");
        MapRawToCode(HID_MOUSE, MC_4, "MC_4");
        MapRawToCode(HID_MOUSE, MC_5, "MC_5");
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

    HIDAction * HIDPlatformTranslator::TranslateKeyboardCode(const unsigned short & code)
    {
        return TranslateCode(HID_KEYBOARD, code);
    }

    HIDAction * HIDPlatformTranslator::TranslateMouseCode(const unsigned short & code)
    {
        return TranslateCode(HID_MOUSE, code);
    }

    HIDAction * HIDPlatformTranslator::TranslateCode(const HID & device, const unsigned short & code)
    {
        auto it = m_actions[device].find(code);
        if (it != m_actions[device].end())
        {
            return it->second;
        }
        return nullptr;
    }

    void HIDPlatformTranslator::MapRawToCode(HID device, unsigned short raw, const std::string & code)
    {
        m_actions[device][raw] = new HIDAction(raw, code);
    }
}
