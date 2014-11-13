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

#include "Events/EventManager.h"
#include "Logic/LogicSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Assets/AssetSystem.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    AlphaController::AlphaController()
        : m_pEvents(nullptr)
        , m_pLogic(nullptr)
        , m_pGraphics(nullptr)
        , m_pAssets(nullptr)
    { }
    AlphaController::~AlphaController() { }

    void AlphaController::SetLogic(LogicSystem *pLogic)
    {
        m_pLogic = pLogic;
    }

    void AlphaController::Execute()
    {
        LOG("Alpha Controller beginning execution");

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
        else
        {
            LOG_ERR("Alpha Controller initialization failed");
        }

        this->Shutdown();

        LOG("Alpha Controller end of execution");
    }

    bool AlphaController::Initialize()
    {
        // Event management system is allways first
        m_pEvents = new EventManager;
        if (!m_pEvents->Initialize())
        {
            LOG_ERR("Event Manager initialization failed");
            return false;
        }

        // Initialize asset repository
        m_pAssets = new AssetSystem();
        if (!m_pAssets->VInitialize())
        {
            LOG_ERR("Asset System initialization failed");
            return false;
        }

        // create graphcs
        m_pGraphics = new GraphicsSystem();
        if (!m_pGraphics->VInitialize())
        {
            LOG_ERR("Graphics System initialization failed");
            return false;
        }

        // create input device manager

        // create game logic
        //m_pLogic = new LogicSystem();
        //m_pLogic = this->CreateGameLogic();
        if (!m_pLogic->VInitialize())
        {
            LOG_ERR("Logic System initialization failed");
            return false;
        }

        // setup timer/clock
        m_start = std::chrono::high_resolution_clock::now();

        LOG("Alpha Controller initialization completed successfully");
        return true;
    }

    bool AlphaController::Update()
    {
        bool success = true;

        // calculate elapsed time since last frame started rendering
        std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - m_start;
        double currentTime = (double)(std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 100000.0f);
        double elapsedTime = currentTime - m_timeLastFrame;

        m_timeAccumulator += elapsedTime;

		// pass around events first, so that each system is
		// prepared for this update phase to begin.
		m_pEvents->Update(currentTime, sk_maxUpdateTime);

        // update systems in discrete chunks of time
        while (m_timeAccumulator >= sk_maxUpdateTime)
        {

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
            m_pEvents->Shutdown();
            delete m_pEvents;
        }

        LOG("Alpha Controller shutdown complete");
        return true;
    }
/*
    template <class GameLogic>
    int InitiateAlpha()
    {
        GameLogic logic;
        AlphaController controller;
        controller.Execute();
        return 0;
    }
    */
}
