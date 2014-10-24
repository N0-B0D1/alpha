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

#include "AlphaController.h"

#include "Events/EventSystem.h"
#include "Logic/LogicSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Assets/AssetSystem.h"

namespace alpha
{
    AlphaController::AlphaController()
        : m_pEvents(nullptr)
        , m_pGraphics(nullptr)
        , m_pLogic(nullptr)
        , m_pAssets(nullptr)
    { }
    AlphaController::~AlphaController() { }

    void AlphaController::Execute()
    {
        if (this->Initialize())
        {
            for (;;)
            {
                if (!this->Update())
                {
                    break;
                }
            }
        }

        this->Shutdown();
    }

    bool AlphaController::Initialize()
    {
        // Event management system is allways first
        m_pEvents = new EventSystem;
        if (!m_pEvents->VInitialize())
        {
            return false;
        }

        // Initialize asset repository
        m_pAssets = new AssetSystem();
        if (!m_pAssets->VInitialize())
        {
            return false;
        }

        // create graphcs
        m_pGraphics = new GraphicsSystem();
        if (!m_pGraphics->VInitialize())
        {
            return false;
        }
        // create input device manager
        // create game logic
        m_pLogic = new LogicSystem();
        if (!m_pLogic->VInitialize())
        {
            return false;
        }

        // setup timer/clock
        m_start = std::chrono::high_resolution_clock::now();

        return true;
    }

    bool AlphaController::Update()
    {
        bool success = true;
        //static int loops = 1000;
        //static const double sk_maxUpdateTime = 1.0f / 60.0f;

        // update event manager
        // process any events for me

        // calculate loop time total and elapsed
        std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - m_start;
        double currentTime = (double)(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 100000.0f);
        double elapsedTime = currentTime - m_timeLastFrame;

        m_timeAccumulator += elapsedTime;

        // update systems in discrete chunks of time
        while (m_timeAccumulator >= sk_maxUpdateTime)
        {
            // pass around events first
            m_pEvents->Update(currentTime, sk_maxUpdateTime);

            m_pAssets->Update(currentTime, sk_maxUpdateTime);
            m_pLogic->Update(currentTime, sk_maxUpdateTime);
            success = m_pGraphics->Update(currentTime, sk_maxUpdateTime);
            if (!success)
            {
                return false;
            }

            m_timeAccumulator -= sk_maxUpdateTime;
        }

        // update the time last frame to now
        // We want to essentially ignore the time taken to update
        // and let the renderer "produce" time that can be "consumed" on the next update
        now = std::chrono::high_resolution_clock::now();
        elapsed = now - m_start;
        m_timeLastFrame = (double)(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 100000.0f);

        // render after updates complete
        m_pGraphics->Render();

        //loops -= 1;
        //return loops > 0;
        return success;
    }

    bool AlphaController::Shutdown()
    {
        // destroy systems in reverse order
        // shutdown needs to be smart about deleting things that might not exist
        // in case something failed during initialization, and we are only half built
        if (m_pLogic)
        {
            m_pLogic->VShutdown();
            delete m_pLogic;
        }
        if (m_pGraphics)
        {
            m_pGraphics->VShutdown();
            delete m_pGraphics;
        }
        if (m_pAssets)
        {
            m_pAssets->VShutdown();
            delete m_pAssets;
        }
        if (m_pEvents)
        {
            m_pEvents->VShutdown();
            delete m_pEvents;
        }

        return true;
    }

    int InitiateAlpha()
    {
        AlphaController controller;
        controller.Execute();
        return 0;
    }
}