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

#include <functional>

#include <SDL.h>

#include "HID/HIDWindowListener.h"
#include "HID/HIDPlatformTranslator.h"
#include "Graphics/RenderWindow.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    static HIDWindowListener * g_pWindowListener = nullptr;

    HIDWindowListener::HIDWindowListener(std::function<void(HID, const HIDAction &, bool)> dispatchHIDActionKey, std::function<void(HID, const HIDAction &, float, float)> dispatchHIDActionAxis)
        : m_fDispatchHIDActionKey(dispatchHIDActionKey)
        , m_fDispatchHIDActionAxis(dispatchHIDActionAxis)
        , m_pPlatformTranslator(nullptr)
    {
        g_pWindowListener = this;

        // set platform context, to translate to engine input codes
        m_pPlatformTranslator = new HIDPlatformTranslator();

        m_lastMousePosition = m_mousePosition;
    }
    HIDWindowListener::~HIDWindowListener()
    {
        g_pWindowListener = nullptr;
    }

    bool HIDWindowListener::Update()
    {
        bool not_closing = true;
        HIDAction * pAction = nullptr;
        int x, y;
        float xoffset, yoffset;
        SDL_Event e;

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                // quit game
                LOG_WARN("Close requested on game window, shutting down game.");
                not_closing = false;
            }

            switch (e.type)
            {
                case SDL_KEYDOWN:
                    // keyboard press, translate key
                    pAction = m_pPlatformTranslator->TranslateKeyboardCode(e.key.keysym.scancode);
                    if (pAction)
                    {
                        m_fDispatchHIDActionKey(HID_KEYBOARD, *pAction, true);
                    }
                    break;

                case SDL_KEYUP:
                    // keyboard release, translate key
                    pAction = m_pPlatformTranslator->TranslateKeyboardCode(e.key.keysym.scancode);
                    if (pAction)
                    {
                        m_fDispatchHIDActionKey(HID_KEYBOARD, *pAction, false);
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    pAction = m_pPlatformTranslator->TranslateMouseCode(e.button.button);
                    if (pAction)
                    {
                        m_fDispatchHIDActionKey(HID_MOUSE, *pAction, true);
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    pAction = m_pPlatformTranslator->TranslateMouseCode(e.button.button);
                    if (pAction)
                    {
                        m_fDispatchHIDActionKey(HID_MOUSE, *pAction, false);
                    }
                    break;

                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&x, &y);
                    m_mousePosition.xAbsolutePos = static_cast<float>(x);
                    m_mousePosition.yAbsolutePos = static_cast<float>(y);
                    break;

                case SDL_MOUSEWHEEL:
                    xoffset = static_cast<float>(e.wheel.x);
                    yoffset = static_cast<float>(e.wheel.y);

                    if (yoffset > 0)
                    {
                        // forward
                        pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_FORWARD);
                        m_fDispatchHIDActionAxis(HID_MOUSE, *pAction, yoffset, 120);
                    }
                    else if (yoffset < 0)
                    {
                        // back scroll
                        pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_BACK);
                        m_fDispatchHIDActionAxis(HID_MOUSE, *pAction, yoffset, -120);
                    }

                    if (xoffset > 0)
                    {
                        // left
                        pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_RIGHT);
                        m_fDispatchHIDActionKey(HID_MOUSE, *pAction, true);
                    }
                    else if (xoffset < 0)
                    {
                        // right
                        pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_LEFT);
                        m_fDispatchHIDActionKey(HID_MOUSE, *pAction, true);
                    }

                    break;

                default:
                    break;
            }
        }

        // publish mouse movement
        if (m_lastMousePosition.xAbsolutePos != m_mousePosition.xAbsolutePos)
        {
            m_mousePosition.xRelativePos = m_lastMousePosition.xAbsolutePos - m_mousePosition.xAbsolutePos;
            HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(MA_X_AXIS);
            m_fDispatchHIDActionAxis(HID_MOUSE, *pAction, m_mousePosition.xRelativePos, m_mousePosition.xAbsolutePos);
        }
        if (m_lastMousePosition.yAbsolutePos != m_mousePosition.yAbsolutePos)
        {
            m_mousePosition.yRelativePos = m_lastMousePosition.yAbsolutePos - m_mousePosition.yAbsolutePos;
            HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(MA_Y_AXIS);
            m_fDispatchHIDActionAxis(HID_MOUSE, *pAction, m_mousePosition.yRelativePos, m_mousePosition.yAbsolutePos);
        }

        // store last position
        m_lastMousePosition = m_mousePosition;

        return not_closing;
    }

    /*
    void HIDWindowListener::GLFWMouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
    {
        if (yoffset > 0)
        {
            // forward
            auto pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_FORWARD);
            m_fDispatchHIDActionAxis(HID_MOUSE, *pAction, yoffset, 120);
        }
        else
        {
            // back scroll
            auto pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_BACK);
            m_fDispatchHIDActionAxis(HID_MOUSE, *pAction, yoffset, -120);
        }

        if (xoffset > 0)
        {
            // left
            auto pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_LEFT);
            m_fDispatchHIDActionKey(HID_MOUSE, *pAction, true);
        }
        else
        {
            // right
            auto pAction = m_pPlatformTranslator->TranslateMouseCode(MA_WHEEL_RIGHT);
            m_fDispatchHIDActionKey(HID_MOUSE, *pAction, true);
        }
    }
    */
}
