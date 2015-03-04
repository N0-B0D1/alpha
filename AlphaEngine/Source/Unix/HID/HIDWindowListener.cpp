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

    HIDWindowListener::HIDWindowListener()
    {
        g_pWindowListener = this;

        // hook up key listener to glfw callback
        glfwSetKeyCallback(g_pWindow, StaticGLFWKeyCallback);
    }
    HIDWindowListener::~HIDWindowListener()
    {
        g_pWindowListener = nullptr;
    }

    void HIDWindowListener::GLFWKeyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
    {
        // do cool stuff with key presses!

        // When a user presses the escape key, we set the WindowShouldClose property to true, 
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}
