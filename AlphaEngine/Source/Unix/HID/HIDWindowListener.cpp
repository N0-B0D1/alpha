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

    HIDWindowListener::HIDWindowListener(EventDataPublisher<EventData_HIDKeyAction> & pubHIDKeyAction)
        : m_pPlatformTranslator(nullptr)
        , m_pubHIDKeyAction(pubHIDKeyAction)
    {
        g_pWindowListener = this;

        // hook up key listener to glfw callback
        glfwSetKeyCallback(g_pWindow, StaticGLFWKeyCallback);

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
