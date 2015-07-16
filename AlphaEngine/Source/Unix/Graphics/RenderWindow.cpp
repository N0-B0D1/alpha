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

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Graphics/RenderWindow.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    /** Initialize global window to nullptr */
    GLFWwindow * g_pWindow = nullptr;

    RenderWindow::RenderWindow() { }
    RenderWindow::~RenderWindow() { }

    bool RenderWindow::Initialize(int windowWidth, int windowHeight)
    {
        g_pWindow = glfwCreateWindow(windowWidth, windowHeight, "ALPHA Engine", nullptr, nullptr);
        glfwMakeContextCurrent(g_pWindow);
        if (g_pWindow == NULL)
        {
            LOG_ERR("Failed to create GLFW window.");
            glfwTerminate();
            return false;
        }

        return true;
    }

    bool RenderWindow::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        // play nicely with the existing window system (X11)
        if (glfwWindowShouldClose(g_pWindow))
        {
            return false;
        }
        
        glfwPollEvents();
        return true;
    }

    bool RenderWindow::Shutdown()
    {
        return true;
    }

    void RenderWindow::Render()
    {
        // XXX remove this? probably not needed
    }

    GLFWwindow * RenderWindow::GetWindow() const
    {
        return g_pWindow;
    }
}
