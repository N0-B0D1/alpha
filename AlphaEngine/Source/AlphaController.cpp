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
#include "Logic/LogicSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Assets/AssetSystem.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    AlphaController::AlphaController()
        : m_pLogic(nullptr)
        , m_pGraphics(nullptr)
        , m_pAssets(nullptr)
    {
        LOG("<AlphaController> Constructed.");
    }
    AlphaController::~AlphaController() { }


    void AlphaController::SetLogic(LogicSystem *pLogic)
    {
        m_pLogic = pLogic;
    }

    void AlphaController::Execute()
    {
        LOG("<AlphaController> Execution start.");

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
            LOG_ERR("<AlphaController> Initialization failed!");
        }

        this->Shutdown();

        LOG("<AlphaController> Execution complete.");
    }

    bool AlphaController::Initialize()
    {
        // Initialize asset repository
        m_pAssets = std::make_shared<AssetSystem>();
        if (!m_pAssets->VInitialize())
        {
            LOG_ERR("<AssetSystem> Initialization failed!");
            return false;
        }

        // create graphcs
        m_pGraphics = new GraphicsSystem();
        if (!m_pGraphics->VInitialize())
        {
            LOG_ERR("<GraphicsSystem> Initialization failed!");
            return false;
        }

        // create input device manager

        // create game logic
        m_pLogic->SetAssetSystem(m_pAssets);
        if (!m_pLogic->VInitialize())
        {
            LOG_ERR("<LogicSystem> Initialization failed!");
            return false;
        }
        
        // wire up pub-sub relations
        m_pLogic->SubscribeToEntityCreated(m_pGraphics->GetEntityCreatedSubscriber());

        // setup timer/clock
        m_start = std::chrono::high_resolution_clock::now();

        LOG("<AlphaController> Initialization complete.");
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
            LOG("<LogicSystem> Disposed.");
        }
        if (m_pGraphics)
        {
            m_pGraphics->VShutdown();
            delete m_pGraphics;
            LOG("<GraphicsSystem> Disposed.");
        }
        if (m_pAssets)
        {
            m_pAssets->VShutdown();
            LOG("<AssetSystem> Disposed.");
        }

        LOG("<AlphaController> Shutdown complete.");
        return true;
    }
}
