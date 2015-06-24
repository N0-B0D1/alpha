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

#include "Graphics/GraphicsSystem.h"
#include "Graphics/IRenderer.h"
#include "Graphics/GraphicsRenderer.h"
#include "Graphics/SceneManager.h"
#include "Graphics/Light.h"
#include "Graphics/Camera.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"
#include "Logic/LogicSystemEvents.h"

namespace alpha
{
    GraphicsSystem::GraphicsSystem()
        : AlphaSystem(30)
        , m_pAssets(nullptr)
        , m_pRenderer(nullptr)
        , m_pSceneManager(nullptr)
        , m_pCamera(nullptr)
    { }
    GraphicsSystem::~GraphicsSystem() { }

    bool GraphicsSystem::VInitialize()
    {
        // OS specific renderer
        m_pRenderer = new GraphicsRenderer();
        if (!m_pRenderer->Initialize(m_pAssets))
        {
            return false;
        }

        // Scene renerable manager
        m_pSceneManager = new SceneManager(m_pAssets);

        // register event handlers
        this->AddEventHandler(AEvent::GetIDFromName(Event_EntityCreated::sk_name), [this](AEvent * pEvent) { this->HandleEntityCreatedEvent(pEvent); });
        this->AddEventHandler(AEvent::GetIDFromName(Event_EntityUpdated::sk_name), [this](AEvent * pEvent) { this->HandleEntityUpdatedEvent(pEvent); });
        this->AddEventHandler(AEvent::GetIDFromName(Event_SetActiveCamera::sk_name), [this](AEvent * pEvent) { this->HandleSetActiveCameraEvent(pEvent); });

        // create a default camera for the scene
        m_pCamera = std::make_shared<Camera>(Vector3(0.f, 0.f, 20.f));

        return true;
    }

    bool GraphicsSystem::VShutdown()
    {
        if (m_pSceneManager) { delete m_pSceneManager; }

        if (m_pRenderer)
        {
            m_pRenderer->Shutdown();
            delete m_pRenderer;
        }
        return true;
    }

    void GraphicsSystem::Render()
    {
        // prepare the scene data for rendering
        m_pSceneManager->PreRender();

        // udpate the camera pre-render, so it can adjust to any game logic changes
        m_pCamera->Update(800, 600);

        // XXX TODO - pass camera into scene manager so the renderables can be frustum culled ... maybe ?
        const std::vector<RenderSet *> renderables = m_pSceneManager->GetRenderData();
        const std::vector<Light *> lights = m_pSceneManager->GetLightData();

        // Prep current set of renderables
        for (auto rs : renderables)
        {
            m_pRenderer->PreRender(rs);
        }

        // Render the array of renderables from the given camera viewpoint
        m_pRenderer->Render(m_pCamera, renderables, lights);
    }

    bool GraphicsSystem::VUpdate(double currentTime, double elapsedTime)
    {
        // udpate scene manager, produces the current
        // set of objects that should be rendered in the
        // scene.
        m_pSceneManager->Update(currentTime, elapsedTime);

        // Prep current set of renderables, make sure that
        // the objects to be rendered are ready, and contain
        // and renderer specific data variables
        const std::vector<RenderSet *> renderables = m_pSceneManager->GetRenderData();
        for (auto rs : renderables)
        {
            m_pRenderer->PreRender(rs);
        }

        return m_pRenderer->Update(currentTime, elapsedTime);
    }

    void GraphicsSystem::SetAssetSystem(AssetSystem * const pAssets)
    {
        m_pAssets = pAssets;
    }

    void GraphicsSystem::HandleEntityCreatedEvent(AEvent * pEvent)
    {
        LOG("Graphics system received Event_EntityCreated");
        if (auto pEntityCreatedEvent = dynamic_cast<Event_EntityCreated *>(pEvent))
        {
            this->m_pSceneManager->Add(pEntityCreatedEvent->GetEntity());
        }
    }

    void GraphicsSystem::HandleEntityUpdatedEvent(AEvent * pEvent)
    {
        //LOG("Graphics system received Event_EntityUpdated");
        if (auto pUpdateEvent = dynamic_cast<Event_EntityUpdated *>(pEvent))
        {
            this->m_pSceneManager->Update(pUpdateEvent->GetEntity());
        }
    }

    void GraphicsSystem::HandleSetActiveCameraEvent(AEvent * pEvent)
    {
        LOG("Graphics system received Event_SetActiveCamera.");
        if (auto pSetActiveCameraEvent = dynamic_cast<Event_SetActiveCamera *>(pEvent))
        {
            this->m_pCamera->SetCameraComponent(pSetActiveCameraEvent->GetCameraComponent());
        }
    }
}
