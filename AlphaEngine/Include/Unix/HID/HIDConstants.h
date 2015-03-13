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

#include <GLFW/glfw3.h>

namespace alpha
{
    /** Keyboard Actions, includes all keys including numpad */
    typedef enum
    {
        KA_BACKSPACE = GLFW_KEY_BACKSPACE,
        KA_TAB = GLFW_KEY_TAB,

        KA_ENTER = GLFW_KEY_ENTER,
        KA_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
        KA_LEFT_CTRL = GLFW_KEY_LEFT_CONTROL,
        KA_LEFT_ALT = =GLFW_KEY_LEFT_ALT,

        KA_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,

        KA_ESCAPE = GLFW_KEY_ESCAPE,

        KA_SPACE = GLFW_KEY_SPACE,
        KA_PAGE_UP = GLFW_KEY_PAGE_UP,
        KA_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
        KA_END = GLFW_KEY_END,
        KA_HOME = GLFW_KEY_HOME,
        KA_LEFT = GLFW_KEY_LEFT,
        KA_UP = GLFW_KEY_UP,
        KA_RIGHT = GLFW_KEY_RIGHT,
        KA_DOWN = GLFW_KEY_DOWN,

        KA_INSERT = GLFW_KEY_INSERT,
        KA_DELETE = GLFW_KEY_DELETE,

        KA_0 = GLFW_KEY_0,
        KA_1 = GLFW_KEY_1,
        KA_2 = GLFW_KEY_2,
        KA_3 = GLFW_KEY_3,
        KA_4 = GLFW_KEY_4,
        KA_5 = GLFW_KEY_5,
        KA_6 = GLFW_KEY_6,
        KA_7 = GLFW_KEY_7,
        KA_8 = GLFW_KEY_8,
        KA_9 = GLFW_KEY_9,

        KA_A = GLFW_KEY_A,
        KA_B = GLFW_KEY_B,
        KA_C = GLFW_KEY_C,
        KA_D = GLFW_KEY_D,
        KA_E = GLFW_KEY_E,
        KA_F = GLFW_KEY_F,
        KA_G = GLFW_KEY_G,
        KA_H = GLFW_KEY_H,
        KA_I = GLFW_KEY_I,
        KA_J = GLFW_KEY_J,
        KA_K = GLFW_KEY_K,
        KA_L = GLFW_KEY_L,
        KA_M = GLFW_KEY_M,
        KA_N = GLFW_KEY_N,
        KA_O = GLFW_KEY_O,
        KA_P = GLFW_KEY_P,
        KA_Q = GLFW_KEY_Q,
        KA_R = GLFW_KEY_R,
        KA_S = GLFW_KEY_S,
        KA_T = GLFW_KEY_T,
        KA_U = GLFW_KEY_U,
        KA_V = GLFW_KEY_V,
        KA_W = GLFW_KEY_W,
        KA_X = GLFW_KEY_X,
        KA_Y = GLFW_KEY_Y,
        KA_Z = GLFW_KEY_Z,
        KA_LEFT_SUPER = GLFW_KEY_LEFT_SUPER, // windows key
        //KA_RIGHT_SUPER = 93, // right menu button

        KA_NUM_0 = GLFW_KEY_KP_0,
        KA_NUM_1 = GLFW_KEY_KP_1,
        KA_NUM_2 = GLFW_KEY_KP_2,
        KA_NUM_3 = GLFW_KEY_KP_3,
        KA_NUM_4 = GLFW_KEY_KP_5,
        KA_NUM_5 = GLFW_KEY_KP_5,
        KA_NUM_6 = GLFW_KEY_KP_6,
        KA_NUM_7 = GLFW_KEY_KP_7,
        KA_NUM_8 = GLFW_KEY_KP_8,
        KA_NUM_9 = GLFW_KEY_KP_9,
        KA_NUM_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
        KA_NUM_ADD = GLFW_KEY_KP_ADD,

        KA_NUM_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
        KA_NUM_DECIMAL = GLFW_KEY_KP_DECIMAL,
        KA_NUM_DIVIDE = GLFW_KEY_KP_DIVIDE,
        KA_F1 = GLFW_KEY_F1,
        KA_F2 = GLFW_KEY_F2,
        KA_F3 = GLFW_KEY_F3,
        KA_F4 = GLFW_KEY_F4,
        KA_F5 = GLFW_KEY_F5,
        KA_F6 = GLFW_KEY_F6,
        KA_F7 = GLFW_KEY_F7,
        KA_F8 = GLFW_KEY_F8,
        KA_F9 = GLFW_KEY_F9,
        KA_F10 = GLFW_KEY_F10,
        KA_F11 = GLFW_KEY_F11,
        KA_F12 = GLFW_KEY_F12,

        KA_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,

        KA_SEMICOLON = GLFW_KEY_SEMICOLON,
        KA_EQUAL = GLFW_KEY_EQUAL,
        KA_COMMA = GLFW_KEY_COMMA,
        KA_MINUS = GLFW_KEY_MINUS,
        KA_PERIOD = GLFW_KEY_PERIOD,
        KA_SLASH = GLFW_KEY_SLASH,
        KA_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,

        KA_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
        KA_BACKSLASH = GLFW_KEY_BACKSLASH,
        KA_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
        KA_APOSTROPHE = GLFW_KEY_APOSTROPHE,

        // special keys, right-alt, num-enter, etc.
        KA_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
        KA_RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
        KA_NUM_ENTER = GLFW_KEY_ENTER,
    } KeyboardAction;

    /** Mouse Actions, includes each axis as a separate action, and up to 5 buttons. */
    typedef enum
    {
        MA_X_AXIS,
        MA_Y_AXIS,
        MA_1,
        MA_2,
        MA_3,
        MA_4,
        MA_5,
        MA_LEFT = MA_1,
        MA_MIDDLE = MA_2,
        MA_RIGHT = MA_3,
    } MouseAction;
}

#endif // ALPHA_HID_CONSTANTS_H
