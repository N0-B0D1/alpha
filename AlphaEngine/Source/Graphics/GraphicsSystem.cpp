/**
Copyright 2014 Jason R. Wendlandt

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

#include "Graphics/GraphicsSystem.h"

#include "Graphics/GraphicsRenderer.h"

namespace alpha
{
    GraphicsSystem::GraphicsSystem()
        : AlphaSystem(30)
        , m_pRenderer(0)
    { }
    GraphicsSystem::~GraphicsSystem() { }

    bool GraphicsSystem::VInitialize()
    {
        m_pRenderer = new GraphicsRenderer();
        if (!m_pRenderer->Initialize())
        {
            return false;
        }
        return true;
    }

    bool GraphicsSystem::VShutdown()
    {
        if (m_pRenderer)
        {
            m_pRenderer->Shutdown();
            delete m_pRenderer;
        }
        return true;
    }

    bool GraphicsSystem::VUpdate(double currentTime, double elapsedTime)
    {
        return m_pRenderer->Update(currentTime, elapsedTime);
    }

    void GraphicsSystem::Render()
    {
        m_pRenderer->Render();
    }
}