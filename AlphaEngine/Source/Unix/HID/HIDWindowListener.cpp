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

#include "HID/HIDWindowListener.h"
#include "HID/HIDPlatformTranslator.h"
#include "Graphics/RenderWindow.h"

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
    static void StaticGLFWMousePositionCallback(GLFWwindow * window, double xpos, double ypos)
    {
        if (g_pWindowListener != nullptr)
        {
            g_pWindowListener->GLFWMousePositionCallback(window, xpos, ypos);
        }
    }

    HIDWindowListener::HIDWindowListener(EventDataPublisher<EventData_HIDKeyAction> & pubHIDKeyAction)
        : m_pPlatformTranslator(nullptr)
        , m_pubHIDKeyAction(pubHIDKeyAction)
    {
        g_pWindowListener = this;

        // hook up GLFW listeners callbacks
        glfwSetKeyCallback(g_pWindow, StaticGLFWKeyCallback);
        glfwSetMouseButtonCallback(g_pWindow, StaticGLFWMouseKeyCallback);
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
            HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(MC_XAXIS);
            DispatchHIDActionAxisEvent(HID_MOUSE, *pAction, m_mousePosition.xRelativePos, m_mousePosition.xAbsolutePos);
        }
        if (m_lastMousePosition.yAbsolutePos != m_mousePosition.yAbsolutePos)
        {
            m_mousePosition.yRelativePos = m_lastMousePosition.yAbsolutePos - m_mousePosition.yAbsolutePos;
            HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(MC_YAXIS);
            DispatchHIDActionAxisEvent(HID_MOUSE, *pAction, m_mousePosition.yRelativePos, m_mousePosition.yAbsolutePos);
        }

        // store last position
        m_lastMousePosition = m_mousePosition;
    }

    void HIDWindowListener::GLFWKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
    {
        // do cool stuff with key presses!
        auto pAction = m_pPlatformTranslator->TranslateKeyboardCode(key);
        if (pAction)
        {
            LOG("action = ", pAction->name);
        }

        // When a user presses the escape key, we set the WindowShouldClose property to true, 
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
    
    void HIDWindowListener::GLFWMouseKeyCallback(GLFWwindow * /*window*/, int button, int action, int mods)
    {
        LOG("Mouse Key Input: ", button, action, mods);
    }

    void HIDWindowListener::GLFWMousePositionCallback(GLFWwindow * /*window*/, double xpos, double ypos)
    {
        m_mousePosition.xAbsolutePos = xpos;
        m_mousePosition.yAbsolutePos = ypos;
    }

    void HIDWindowListener::DispatchHIDActionKeyEvent(HID device, const HIDAction & action, bool pressed)
    {
        std::shared_ptr<EventData_HIDKeyAction> event = std::make_shared<EventData_HIDKeyAction>(device, action, pressed);
        m_pubHIDKeyAction.Publish(event);

    }

    void HIDWindowListener::DispatchHIDActionAxisEvent(HID device, const HIDAction & action, long relative, float absolute)
    {
        std::shared_ptr<EventData_HIDKeyAction> event = std::make_shared<EventData_HIDKeyAction>(device, action, false, relative, absolute);
        m_pubHIDKeyAction.Publish(event);
    }
}
