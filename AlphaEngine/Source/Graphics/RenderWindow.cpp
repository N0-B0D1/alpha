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

#include <SDL.h>
#include <SDL_video.h>

#include "Graphics/RenderWindow.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    RenderWindow::RenderWindow()
        : m_pWindow(nullptr)
    { }
    RenderWindow::~RenderWindow() { }

    bool RenderWindow::Initialize(int windowWidth, int windowHeight)
    {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        {
            LOG_ERR("Failed to initialize SDL video system! ", SDL_GetError());
            return false;
        }

        // Video sub-system must be initialize before we set attributes, but
        // attributes must be set before the window is created, thus we have to
        // define these here, rather than in the renderer class.
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_pWindow = SDL_CreateWindow(
            "ALPHA Engine",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_OPENGL
        );

        if (m_pWindow == nullptr)
        {
            LOG_ERR("Failed to create SDL window! ", SDL_GetError());
            return false;
        }

        return true;
    }

    bool RenderWindow::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        return true;
    }

    bool RenderWindow::Shutdown()
    {
        SDL_DestroyWindow(m_pWindow);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return true;
    }

    void RenderWindow::Render()
    {
        // XXX remove this? probably not needed
    }

    SDL_Window * RenderWindow::GetWindow() const
    {
        return m_pWindow;
    }

    SDL_SysWMinfo RenderWindow::GetWindowInfo() const
    {
        SDL_SysWMinfo info;

        SDL_VERSION(&info.version);
        if (!SDL_GetWindowWMInfo(m_pWindow, &info))
        {
            LOG_ERR("SDL could not retrieve window information. ", SDL_GetError());
        }

        return info;
    }
}
