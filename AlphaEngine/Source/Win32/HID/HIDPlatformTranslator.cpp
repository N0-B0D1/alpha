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

#include <Windows.h>

#include "HID/HIDPlatformTranslator.h"
#include "HID/HIDTypes.h"

namespace alpha
{
    HIDPlatformTranslator::HIDPlatformTranslator()
    {
        // add some default states, ranges, and actions

        // control keys
        MapRawToCode(HID_KEYBOARD, 8, "KB_BACKSPACE");
        MapRawToCode(HID_KEYBOARD, 9, "KB_TAB");
        MapRawToCode(HID_KEYBOARD, 13, "KB_ENTER");
        MapRawToCode(HID_KEYBOARD, 16, "KB_SHIFT");
        MapRawToCode(HID_KEYBOARD, 17, "KB_CTRL");
        MapRawToCode(HID_KEYBOARD, 18, "KB_ALT");
        MapRawToCode(HID_KEYBOARD, 20, "KB_CAPLOCK");

        // escape
        MapRawToCode(HID_KEYBOARD, 27, "KB_ESCAPE");

        // arrow keys
        MapRawToCode(HID_KEYBOARD, 37, "KB_LEFT");
        MapRawToCode(HID_KEYBOARD, 38, "KB_UP");
        MapRawToCode(HID_KEYBOARD, 39, "KB_RIGHT");
        MapRawToCode(HID_KEYBOARD, 40, "KB_DOWN");

        // word processor modifiers
        MapRawToCode(HID_KEYBOARD, 32, "KB_SPACE");
        MapRawToCode(HID_KEYBOARD, 33, "KB_PAGEUP");
        MapRawToCode(HID_KEYBOARD, 34, "KB_PAGEDOWN");
        MapRawToCode(HID_KEYBOARD, 35, "KB_END");
        MapRawToCode(HID_KEYBOARD, 36, "KB_HOME");
        MapRawToCode(HID_KEYBOARD, 45, "KB_INSERT");
        MapRawToCode(HID_KEYBOARD, 46, "KB_DELETE");

        // number row
        MapRawToCode(HID_KEYBOARD, 48, "KB_0");
        MapRawToCode(HID_KEYBOARD, 49, "KB_1");
        MapRawToCode(HID_KEYBOARD, 50, "KB_2");
        MapRawToCode(HID_KEYBOARD, 51, "KB_3");
        MapRawToCode(HID_KEYBOARD, 52, "KB_4");
        MapRawToCode(HID_KEYBOARD, 53, "KB_5");
        MapRawToCode(HID_KEYBOARD, 54, "KB_6");
        MapRawToCode(HID_KEYBOARD, 55, "KB_7");
        MapRawToCode(HID_KEYBOARD, 56, "KB_8");
        MapRawToCode(HID_KEYBOARD, 57, "KB_9");

        // alphabet keys
        MapRawToCode(HID_KEYBOARD, 65, "KB_A");
        MapRawToCode(HID_KEYBOARD, 66, "KB_B");
        MapRawToCode(HID_KEYBOARD, 67, "KB_C");
        MapRawToCode(HID_KEYBOARD, 68, "KB_D");
        MapRawToCode(HID_KEYBOARD, 69, "KB_E");
        MapRawToCode(HID_KEYBOARD, 70, "KB_F");
        MapRawToCode(HID_KEYBOARD, 71, "KB_G");
        MapRawToCode(HID_KEYBOARD, 72, "KB_H");
        MapRawToCode(HID_KEYBOARD, 73, "KB_I");
        MapRawToCode(HID_KEYBOARD, 74, "KB_J");
        MapRawToCode(HID_KEYBOARD, 75, "KB_K");
        MapRawToCode(HID_KEYBOARD, 76, "KB_L");
        MapRawToCode(HID_KEYBOARD, 77, "KB_M");
        MapRawToCode(HID_KEYBOARD, 78, "KB_N");
        MapRawToCode(HID_KEYBOARD, 79, "KB_O");
        MapRawToCode(HID_KEYBOARD, 80, "KB_P");
        MapRawToCode(HID_KEYBOARD, 81, "KB_Q");
        MapRawToCode(HID_KEYBOARD, 82, "KB_R");
        MapRawToCode(HID_KEYBOARD, 83, "KB_S");
        MapRawToCode(HID_KEYBOARD, 84, "KB_T");
        MapRawToCode(HID_KEYBOARD, 85, "KB_U");
        MapRawToCode(HID_KEYBOARD, 86, "KB_V");
        MapRawToCode(HID_KEYBOARD, 87, "KB_W");
        MapRawToCode(HID_KEYBOARD, 88, "KB_X");
        MapRawToCode(HID_KEYBOARD, 89, "KB_Y");
        MapRawToCode(HID_KEYBOARD, 90, "KB_Z");

        // function keys
        MapRawToCode(HID_KEYBOARD, 112, "KB_F1");
        MapRawToCode(HID_KEYBOARD, 113, "KB_F2");
        MapRawToCode(HID_KEYBOARD, 114, "KB_F3");
        MapRawToCode(HID_KEYBOARD, 115, "KB_F4");
        MapRawToCode(HID_KEYBOARD, 116, "KB_F5");
        MapRawToCode(HID_KEYBOARD, 117, "KB_F6");
        MapRawToCode(HID_KEYBOARD, 118, "KB_F7");
        MapRawToCode(HID_KEYBOARD, 119, "KB_F8");
        MapRawToCode(HID_KEYBOARD, 120, "KB_F9");
        MapRawToCode(HID_KEYBOARD, 121, "KB_F10");
        MapRawToCode(HID_KEYBOARD, 122, "KB_F11");
        MapRawToCode(HID_KEYBOARD, 123, "KB_F12");

        // special characters
        MapRawToCode(HID_KEYBOARD, 186, "KB_SEMICOLON");
        MapRawToCode(HID_KEYBOARD, 187, "KB_EQUALS");
        MapRawToCode(HID_KEYBOARD, 188, "KB_COMMA");
        MapRawToCode(HID_KEYBOARD, 189, "KB_MINUS");
        MapRawToCode(HID_KEYBOARD, 190, "KB_PERIOD");
        MapRawToCode(HID_KEYBOARD, 191, "KB_FORWARDSLASH");
        MapRawToCode(HID_KEYBOARD, 192, "KB_TICK");
        MapRawToCode(HID_KEYBOARD, 219, "KB_LEFTBRACKET");
        MapRawToCode(HID_KEYBOARD, 220, "KB_BACKSLASH");
        MapRawToCode(HID_KEYBOARD, 221, "KB_RIGHTBRACKET");
        MapRawToCode(HID_KEYBOARD, 222, "KB_APOSTRAPHE");

        // numpad
        MapRawToCode(HID_KEYBOARD, 13, "KB_NUMPLUS");
        MapRawToCode(HID_KEYBOARD, 96, "KB_NUM0");
        MapRawToCode(HID_KEYBOARD, 97, "KB_NUM1");
        MapRawToCode(HID_KEYBOARD, 98, "KB_NUM2");
        MapRawToCode(HID_KEYBOARD, 99, "KB_NUM3");
        MapRawToCode(HID_KEYBOARD, 100, "KB_NUM4");
        MapRawToCode(HID_KEYBOARD, 101, "KB_NUM5");
        MapRawToCode(HID_KEYBOARD, 102, "KB_NUM6");
        MapRawToCode(HID_KEYBOARD, 103, "KB_NUM7");
        MapRawToCode(HID_KEYBOARD, 104, "KB_NUM8");
        MapRawToCode(HID_KEYBOARD, 105, "KB_NUM9");
        MapRawToCode(HID_KEYBOARD, 106, "KB_NUMMULTIPLY");
        MapRawToCode(HID_KEYBOARD, 107, "KB_NUMENTER");
        MapRawToCode(HID_KEYBOARD, 109, "KB_NUMMINUS");
        MapRawToCode(HID_KEYBOARD, 110, "KB_NUMPERIOD");
        MapRawToCode(HID_KEYBOARD, 111, "KB_NUMDIVIDE");

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