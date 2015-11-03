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

#include <SDL_events.h>

namespace alpha
{
    /** Keyboard Actions, includes all keys including numpad */
    typedef enum
    {
        KA_BACKSPACE = SDL_SCANCODE_BACKSPACE,
        KA_TAB = SDL_SCANCODE_TAB,

        KA_ENTER = SDL_SCANCODE_RETURN,
        KA_LEFT_SHIFT = SDL_SCANCODE_LSHIFT,
        KA_LEFT_CTRL = SDL_SCANCODE_LCTRL,
        KA_LEFT_ALT = SDL_SCANCODE_LALT,

        KA_CAPS_LOCK = SDL_SCANCODE_CAPSLOCK,

        KA_ESCAPE = SDL_SCANCODE_ESCAPE,

        KA_SPACE = SDL_SCANCODE_SPACE,
        KA_PAGE_UP = SDL_SCANCODE_PAGEUP,
        KA_PAGE_DOWN = SDL_SCANCODE_PAGEDOWN,
        KA_END = SDL_SCANCODE_END,
        KA_HOME = SDL_SCANCODE_HOME,
        KA_LEFT = SDL_SCANCODE_LEFT,
        KA_UP = SDL_SCANCODE_UP,
        KA_RIGHT = SDL_SCANCODE_RIGHT,
        KA_DOWN = SDL_SCANCODE_DOWN,

        KA_INSERT = SDL_SCANCODE_INSERT,
        KA_DELETE = SDL_SCANCODE_DELETE,

        KA_0 = SDL_SCANCODE_0,
        KA_1 = SDL_SCANCODE_1,
        KA_2 = SDL_SCANCODE_2,
        KA_3 = SDL_SCANCODE_3,
        KA_4 = SDL_SCANCODE_4,
        KA_5 = SDL_SCANCODE_5,
        KA_6 = SDL_SCANCODE_6,
        KA_7 = SDL_SCANCODE_7,
        KA_8 = SDL_SCANCODE_8,
        KA_9 = SDL_SCANCODE_9,

        KA_A = SDL_SCANCODE_A,
        KA_B = SDL_SCANCODE_B,
        KA_C = SDL_SCANCODE_C,
        KA_D = SDL_SCANCODE_D,
        KA_E = SDL_SCANCODE_E,
        KA_F = SDL_SCANCODE_F,
        KA_G = SDL_SCANCODE_G,
        KA_H = SDL_SCANCODE_H,
        KA_I = SDL_SCANCODE_I,
        KA_J = SDL_SCANCODE_J,
        KA_K = SDL_SCANCODE_K,
        KA_L = SDL_SCANCODE_L,
        KA_M = SDL_SCANCODE_M,
        KA_N = SDL_SCANCODE_N,
        KA_O = SDL_SCANCODE_O,
        KA_P = SDL_SCANCODE_P,
        KA_Q = SDL_SCANCODE_Q,
        KA_R = SDL_SCANCODE_R,
        KA_S = SDL_SCANCODE_S,
        KA_T = SDL_SCANCODE_T,
        KA_U = SDL_SCANCODE_U,
        KA_V = SDL_SCANCODE_V,
        KA_W = SDL_SCANCODE_W,
        KA_X = SDL_SCANCODE_X,
        KA_Y = SDL_SCANCODE_Y,
        KA_Z = SDL_SCANCODE_Z,
        KA_LEFT_SUPER = SDL_SCANCODE_LGUI, // windows key
        //KA_RIGHT_SUPER = SLDK_RGUI, // right windows key

        KA_NUM_0 = SDL_SCANCODE_KP_0,
        KA_NUM_1 = SDL_SCANCODE_KP_1,
        KA_NUM_2 = SDL_SCANCODE_KP_2,
        KA_NUM_3 = SDL_SCANCODE_KP_3,
        KA_NUM_4 = SDL_SCANCODE_KP_4,
        KA_NUM_5 = SDL_SCANCODE_KP_5,
        KA_NUM_6 = SDL_SCANCODE_KP_6,
        KA_NUM_7 = SDL_SCANCODE_KP_7,
        KA_NUM_8 = SDL_SCANCODE_KP_8,
        KA_NUM_9 = SDL_SCANCODE_KP_9,
        KA_NUM_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
        KA_NUM_ADD = SDL_SCANCODE_KP_PLUS,

        KA_NUM_SUBTRACT = SDL_SCANCODE_KP_MINUS,
        KA_NUM_DECIMAL = SDL_SCANCODE_KP_PERIOD,
        KA_NUM_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
        KA_NUM_ENTER = SDL_SCANCODE_KP_ENTER,

        KA_F1 = SDL_SCANCODE_F1,
        KA_F2 = SDL_SCANCODE_F2,
        KA_F3 = SDL_SCANCODE_F3,
        KA_F4 = SDL_SCANCODE_F4,
        KA_F5 = SDL_SCANCODE_F5,
        KA_F6 = SDL_SCANCODE_F6,
        KA_F7 = SDL_SCANCODE_F7,
        KA_F8 = SDL_SCANCODE_F8,
        KA_F9 = SDL_SCANCODE_F9,
        KA_F10 = SDL_SCANCODE_F10,
        KA_F11 = SDL_SCANCODE_F11,
        KA_F12 = SDL_SCANCODE_F12,

        KA_RIGHT_SHIFT = SDL_SCANCODE_RSHIFT,

        KA_SEMICOLON = SDL_SCANCODE_SEMICOLON,
        KA_EQUAL = SDL_SCANCODE_EQUALS,
        KA_COMMA = SDL_SCANCODE_COMMA,
        KA_MINUS = SDL_SCANCODE_MINUS,
        KA_PERIOD = SDL_SCANCODE_PERIOD,
        KA_SLASH = SDL_SCANCODE_SLASH,
        KA_GRAVE_ACCENT = SDL_SCANCODE_GRAVE,

        KA_LEFT_BRACKET = SDL_SCANCODE_LEFTBRACKET,
        KA_BACKSLASH = SDL_SCANCODE_BACKSLASH,
        KA_RIGHT_BRACKET = SDL_SCANCODE_RIGHTBRACKET,
        KA_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,

        // special keys, right-alt, num-enter, etc.
        KA_RIGHT_ALT = SDL_SCANCODE_RALT,
        KA_RIGHT_CTRL = SDL_SCANCODE_RCTRL,
    } KeyboardAction;

    /** Mouse Actions, includes each axis as a separate action, and up to 5 buttons. */
    typedef enum
    {
        MA_1 = SDL_BUTTON_LEFT,
        MA_2 = SDL_BUTTON_MIDDLE,
        MA_3 = SDL_BUTTON_RIGHT,
        MA_4 = SDL_BUTTON_X1,
        MA_5 = SDL_BUTTON_X2,
        //MA_6 = SDL_BUTTON_6,
        //MA_7 = SDL_BUTTON_7,
        //MA_8 = SDL_BUTTON_8,

        MA_LEFT = MA_1,
        MA_MIDDLE = MA_2,
        MA_RIGHT = MA_3,

        MA_WHEEL_FORWARD = 15,
        MA_WHEEL_BACK = 16,
        MA_WHEEL_LEFT = 17,
        MA_WHEEL_RIGHT = 18,

        MA_X_AXIS = 10,
        MA_Y_AXIS = 11,
    } MouseAction;
}

#endif // ALPHA_HID_CONSTANTS_H
