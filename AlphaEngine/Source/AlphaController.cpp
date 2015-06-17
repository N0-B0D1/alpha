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

#include <memory>

#include "AlphaController.h"
#include "Audio/AudioSystem.h"
#include "Logic/LogicSystem.h"
#include "Graphics/GraphicsSystem.h"
#include "Assets/AssetSystem.h"
#include "Toolbox/Logger.h"
#include "FSA/StateMachine.h"
#include "FSA/GameState.h"
#include "Threading/ThreadSystem.h"
#include "HID/HIDSystem.h"
#include "Events/EventManager.h"

namespace alpha
{
    AlphaController::AlphaController()
        : m_pThreads(nullptr)
        , m_pLogic(nullptr)
        , m_pGraphics(nullptr)
        , m_pAssets(nullptr)
        , m_pAudio(nullptr)
        , m_pInput(nullptr)
        , m_pGameStateMachine(nullptr)
    { }
    AlphaController::~AlphaController() { }

    void AlphaController::SetGameState(std::shared_ptr<AGameState> state)
    {
        if (m_pGameStateMachine == nullptr)
        {
            state->SetLogic(m_pLogic);
            m_pGameStateMachine = new StateMachine(state);
        }
        else
        {
            LOG_WARN("<AlphaController>", " Attempted to attach game state after it has already been attached.");
        }
    }

    void AlphaController::Execute(std::shared_ptr<AGameState> state)
    {
        LOG("<AlphaController> Execution start.");

        if (this->Initialize(state))
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

    bool AlphaController::Initialize(std::shared_ptr<AGameState> state)
    {
        // Create the event manager first
        // the event manager is responsible for all communication between
        // sub-systems, thus it must exist for any systems.
        m_pEventManager = new EventManager();
        if (!m_pEventManager->Initialize())
        {
            LOG_ERR("--EventManager-- Initialization failed!");
            return false;
        }

        // Initialize asset repository
        m_pAssets = new AssetSystem();
        if (!InitializeSystem(m_pAssets)) { LOG_ERR("<AssetSystem> Initialization failed!"); return false; }

        // create graphics system, for platform specific rendering
        m_pGraphics = new GraphicsSystem();
        m_pGraphics->SetAssetSystem(m_pAssets); // attach asset system to graphics system
        if (!InitializeSystem(m_pGraphics)) { LOG_ERR("<GraphicsSystem> Initialization failed!"); return false; }

        // create human input device system
        m_pInput = new HIDSystem();
        if (!InitializeSystem(m_pInput)) { LOG_ERR("<HIDSystem> Initialization failed!"); return false; }

        // create audio system
        m_pAudio = new AudioSystem();
        if (!InitializeSystem(m_pAudio)) { LOG_ERR("<AudioSystem> Initialization failed!"); return false; }

        // create logic system to manage game layer
        m_pLogic = new LogicSystem();
        m_pLogic->SetAudioSystem(m_pAudio);     // attach audio system to logic layer
        m_pLogic->SetAssetSystem(m_pAssets);    // initialize the game logic
        this->SetGameState(state);              // set starting state
        if (!InitializeSystem(m_pLogic)) { LOG_ERR("<LogicSystem> Initialization failed!"); return false; }

        // wire up pub-sub relations
        m_pLogic->SubscribeToEntityCreated(m_pGraphics->GetEntityCreatedSubscriber());
        m_pLogic->SubscribeToSetActiveCamera(m_pGraphics->GetSetActiveCameraSubscriber());
        m_pInput->SubscribeToHIDKeyAction(m_pLogic->GetHIDKeyActionSubscriber());

        // initialize the specified game state
        // if no state has been specified, then fail to startup
        if (m_pGameStateMachine == nullptr)
        {
            LOG_ERR("<AlphaController> No Game State specified.");
            return false;
        }
        if (!InitializeSystem(m_pGameStateMachine)) { LOG_ERR("<AlphaController> Game State failed to initialize."); return false; }

        // prep threading system last, so tasks can't be processed until
        // the whole engine is up and running.
        m_pThreads = new ThreadSystem();
        if (!InitializeSystem(m_pThreads)) { LOG_ERR("<ThreadSystem> Initialization failed!"); return false; }

        // subscribe the threading system to any new task publishers
        auto taskSubscriber = m_pThreads->GetThreadTaskCreatedSubscriber();
        m_pGraphics->SubscribeToThreadTaskCreated(taskSubscriber);

        // setup timer/clock
        m_start = std::chrono::high_resolution_clock::now();

        LOG("<AlphaController> Initialization complete.");
        return true;
    }

    bool AlphaController::InitializeSystem(AlphaSystem * pSystem)
    {
        if (!pSystem->Initialize(m_pEventManager))
        {
            return false;
        }
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
            // update thread sub-system, allow threads access to any new tasks.
            m_pThreads->Update(currentTime, sk_maxUpdateTime);

            // update audio sub-system
            m_pAudio->Update(currentTime, sk_maxUpdateTime);

            // update asset system, which should cull least recently used items from memory
            m_pAssets->Update(currentTime, sk_maxUpdateTime);

            // update human input, so the latest input state can be passed to the logic system
            m_pInput->Update(currentTime, sk_maxUpdateTime);

            // update logic which will udpate entities
            m_pLogic->Update(currentTime, sk_maxUpdateTime);

            // update current game state
            success = m_pGameStateMachine->Update(currentTime, sk_maxUpdateTime);

            // update graphics, which will create any new elements for the scene, or remove them as needed.
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

        return success;
    }

    bool AlphaController::Shutdown()
    {
        // destroy systems in reverse order
        // shutdown needs to be smart about deleting things that might not exist
        // in case something failed during initialization, and we are only half built

        // dispose of threading system first, so any running threads will be joined
        // and tasks wont be left in a bad state as the engine shuts down.
        if (ShutdownSystem(m_pThreads)) { LOG("<ThreadSystem> Disposed."); }
        if (ShutdownSystem(m_pGameStateMachine)) { LOG("<GameStateMachine> Disposed."); } // XXX game state machine probably shouldn't be a system ...
        if (ShutdownSystem(m_pLogic)) { LOG("<LogicSystem> Disposed."); }
        if (ShutdownSystem(m_pAudio)) { LOG("<AudioSystem> Disposed."); }
        if (ShutdownSystem(m_pInput)) { LOG("<HIDSystem> Disposed."); }
        if (ShutdownSystem(m_pGraphics)) { LOG("<GraphicsSystem> Disposed."); }
        if (ShutdownSystem(m_pAssets)) { LOG("<AssetSystem> Disposed."); }

        LOG("<AlphaController> Shutdown complete.");
        return true;
    }

    bool AlphaController::ShutdownSystem(AlphaSystem * pSystem)
    {
        if (pSystem)
        {
            pSystem->Shutdown(m_pEventManager);
            delete pSystem;
            return true;
        }
        return false;
    }
}
