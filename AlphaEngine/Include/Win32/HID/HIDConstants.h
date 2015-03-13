#ifndef ALPHA_HID_CONSTANTS_H
#define ALPHA_HID_CONSTANTS_H

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

namespace alpha
{
    /** Keyboard Actions, includes all keys including numpad */
    typedef enum
    {
        KA_BACKSPACE = 8,
        KA_TAB = 9,

        KA_ENTER = 13,
        KA_LEFT_SHIFT = 160,
        KA_LEFT_CTRL = 17,
        KA_LEFT_ALT = 18,

        KA_CAPS_LOCK = 20,

        KA_ESCAPE = 27,

        KA_SPACE = 32,
        KA_PAGE_UP = 33,
        KA_PAGE_DOWN = 34,
        KA_END = 35,
        KA_HOME = 36,
        KA_LEFT = 37,
        KA_UP = 38,
        KA_RIGHT = 39,
        KA_DOWN = 40,

        KA_INSERT = 45,
        KA_DELETE = 46,

        KA_0 = 48,
        KA_1 = 49,
        KA_2 = 50,
        KA_3 = 51,
        KA_4 = 52,
        KA_5 = 53,
        KA_6 = 54,
        KA_7 = 55,
        KA_8 = 56,
        KA_9 = 57,

        KA_A = 65,
        KA_B = 66,
        KA_C = 67,
        KA_D = 68,
        KA_E = 69,
        KA_F = 70,
        KA_G = 71,
        KA_H = 72,
        KA_I = 73,
        KA_J = 74,
        KA_K = 75,
        KA_L = 76,
        KA_M = 77,
        KA_N = 78,
        KA_O = 79,
        KA_P = 80,
        KA_Q = 81,
        KA_R = 82,
        KA_S = 83,
        KA_T = 84,
        KA_U = 85,
        KA_V = 86,
        KA_W = 87,
        KA_X = 88,
        KA_Y = 89,
        KA_Z = 90,
        KA_LEFT_SUPER = 91, // windows key
        //KA_RIGHT_SUPER = 93, // right menu button

        KA_NUM_0 = 96,
        KA_NUM_1 = 97,
        KA_NUM_2 = 98,
        KA_NUM_3 = 99,
        KA_NUM_4 = 100,
        KA_NUM_5 = 101,
        KA_NUM_6 = 102,
        KA_NUM_7 = 103,
        KA_NUM_8 = 104,
        KA_NUM_9 = 105,
        KA_NUM_MULTIPLY = 106,
        KA_NUM_ADD = 107,

        KA_NUM_SUBTRACT = 109,
        KA_NUM_DECIMAL = 110,
        KA_NUM_DIVIDE = 111,
        KA_F1 = 112,
        KA_F2 = 113,
        KA_F3 = 114,
        KA_F4 = 115,
        KA_F5 = 116,
        KA_F6 = 117,
        KA_F7 = 118,
        KA_F8 = 119,
        KA_F9 = 120,
        KA_F10 = 121,
        KA_F11 = 122,
        KA_F12 = 123,

        KA_RIGHT_SHIFT = 161,

        KA_SEMICOLON = 186,
        KA_EQUAL = 187,
        KA_COMMA = 188,
        KA_MINUS = 189,
        KA_PERIOD = 190,
        KA_SLASH = 191,
        KA_GRAVE_ACCENT = 192,

        KA_LEFT_BRACKET = 219,
        KA_BACKSLASH = 220,
        KA_RIGHT_BRACKET = 221,
        KA_APOSTROPHE = 222,

        // special keys, right-alt, num-enter, etc.
        KA_RIGHT_ALT = 301,
        KA_RIGHT_CTRL = 302,
        KA_NUM_ENTER = 303,
    } KeyboardAction;

    /** Mouse Actions, includes each axis as a separate action, and up to 5 buttons. */
    typedef enum
    {
        MA_1 = 1,
        MA_2 = 2,
        MA_3 = 3,
        MA_4 = 4,
        MA_5 = 5,
        MA_6 = 6,
        MA_7 = 7,
        MA_8 = 8,

        MA_LEFT = MA_1,
        MA_MIDDLE = MA_2,
        MA_RIGHT = MA_3,

        MA_WHEEL_FORWARD = 15,
        MA_WHEEL_BACK = 16,
        MA_WHEEL_LEFT = 17,
        MA_WHEEL_RIGHT = 18,

        MA_X_AXIS = 20,
        MA_Y_AXIS = 21,
    } MouseAction;
}

#endif // ALPHA_HID_CONSTANTS_H