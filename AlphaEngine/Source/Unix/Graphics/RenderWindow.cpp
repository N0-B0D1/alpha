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
    RenderWindow::RenderWindow() { }
    RenderWindow::~RenderWindow() { }

    bool RenderWindow::Initialize()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

        m_pWindow = glfwCreateWindow(800, 600, "ALPHA Engine", nullptr, nullptr);
        glfwMakeContextCurrent(m_pWindow);
        if (m_pWindow == NULL)
        {
            LOG_ERR("Failed to create GLFW window.");
            glfwTerminate();
            return false;
        }

        // register key event
        glfwSetKeyCallback(m_pWindow, RenderWindow::key_callback);

        return true;
    }

    bool RenderWindow::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        // play nicely with X11, or possibly other system being used?
        if (glfwWindowShouldClose(m_pWindow))
        {
            return false;
        }
        else
        {
            glfwPollEvents();
        }

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
        return m_pWindow;
    }

    void RenderWindow::key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
    {
        // When a user presses the escape key, we set the WindowShouldClose property to true, 
        // closing the application
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
