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

#include "HID/HIDWindowListener.h"
#include "HID/HIDPlatformTranslator.h"
#include "Graphics/RenderWindow.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    static HIDWindowListener * g_pWindowListener = nullptr;
    static void StaticGLFWKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
    {
        if (g_pWindowListener != nullptr)
        {
            g_pWindowListener->GLFWKeyCallback(window, key, scancode, action, mode);
        }
    }
    static void StaticGLFWMouseKeyCallback(GLFWwindow * window, int button, int action, int mods)
    {
        if (g_pWindowListener != nullptr)
        {
            g_pWindowListener->GLFWMouseKeyCallback(window, button, action, mods);
        }
    }
    static void StaticGLFWMouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
    {
        if (g_pWindowListener != nullptr)
        {
            g_pWindowListener->GLFWMouseScrollCallback(window, xoffset, yoffset);
        }
    }
    static void StaticGLFWMousePositionCallback(GLFWwindow * window, double xpos, double ypos)
    {
        if (g_pWindowListener != nullptr)
        {
            g_pWindowListener->GLFWMousePositionCallback(window, xpos, ypos);
        }
    }

    HIDWindowListener::HIDWindowListener(std::function<void(HID, const HIDAction &, bool)> dispatchHIDActionKey, std::function<void(HID, const HIDAction &, long, float)> dispatchHIDActionAxis)
        : m_fDispatchHIDActionKey(dispatchHIDActionKey)
        , m_fDispatchHIDActionAxis(dispatchHIDActionAxis)
        , m_pPlatformTranslator(nullptr)
    {
        g_pWindowListener = this;

        // hook up GLFW listeners callbacks
        glfwSetKeyCallback(g_pWindow, StaticGLFWKeyCallback);
        glfwSetMouseButtonCallback(g_pWindow, StaticGLFWMouseKeyCallback);
        glfwSetScrollCallback(g_pWindow, StaticGLFWMouseScrollCallback);
        glfwSetCursorPosCallback(g_pWindow, StaticGLFWMousePositionCallback);

        // set platform context, to translate to engine input codes
        m_pPlatformTranslator = new HIDPlatformTranslator();

        m_lastMousePosition = m_mousePosition;
    }
    HIDWindowListener::~HIDWindowListener()
    {
        g_pWindowListener = nullptr;
    }

    void HIDWindowListener::Update()
    {
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
    }

    void HIDWindowListener::GLFWKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
    {
        if (action != GLFW_REPEAT)
        {
            // do cool stuff with key presses!
            auto pAction = m_pPlatformTranslator->TranslateKeyboardCode(key);
            if (pAction)
            {
                m_fDispatchHIDActionKey(HID_KEYBOARD, *pAction, action == GLFW_PRESS);
            }

            // When a user presses the escape key, we set the WindowShouldClose property to true, 
            if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
        }
    }
    
    void HIDWindowListener::GLFWMouseKeyCallback(GLFWwindow * /*window*/, int button, int action, int mods)
    {
        auto pAction = m_pPlatformTranslator->TranslateMouseCode(button);
        if (pAction)
        {
            m_fDispatchHIDActionKey(HID_MOUSE, *pAction, action == GLFW_PRESS);
        }
        else
        {
            LOG_WARN("Unknown mouse key click: ", button, ", ", action, ", ", mods);
        }
    }

    void HIDWindowListener::GLFWMouseScrollCallback(GLFWwindow * /*window*/, double xoffset, double yoffset)
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

    void HIDWindowListener::GLFWMousePositionCallback(GLFWwindow * /*window*/, double xpos, double ypos)
    {
        m_mousePosition.xAbsolutePos = xpos;
        m_mousePosition.yAbsolutePos = ypos;
    }
}
