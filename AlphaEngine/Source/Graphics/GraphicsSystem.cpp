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
#include "Graphics/GraphicsRenderer.h"
#include "Graphics/SceneManager.h"
#include "Graphics/RenderData.h"
#include "Graphics/Camera.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    GraphicsSystem::GraphicsSystem()
        : AlphaSystem(30)
        , m_pAssets(nullptr)
        , m_pRenderer(nullptr)
        , m_pSceneManager(nullptr)
        , m_pCamera(nullptr)
        , m_subEntityCreated(nullptr)
        , m_subSetActiveCamera(nullptr)
        , m_pubThreadTaskCreated(nullptr)
    { }
    GraphicsSystem::~GraphicsSystem() { }

    bool GraphicsSystem::VInitialize()
    {
        // load some basic shaders to use for everything until proper load pipeline is implemented
        auto ps_shader = this->LoadShaderFile("Shaders/Basic_PS");
        auto vs_shader = this->LoadShaderFile("Shaders/Basic_VS");

        // OS specific renderer
        m_pRenderer = new GraphicsRenderer();
        m_pRenderer->SetBasicShaders(ps_shader, vs_shader);
        if (!m_pRenderer->Initialize())
        {
            return false;
        }

        // make task publisher, and shared with sub-systems
        m_pubThreadTaskCreated = std::make_shared<EventDataPublisher<EventData_ThreadTaskCreated>>();

        // Scene renerable manager
        m_pSceneManager = new SceneManager(m_pubThreadTaskCreated, m_pAssets);

        // create event subscribers
        m_subEntityCreated = std::make_shared<EventDataSubscriber<EventData_EntityCreated>>();
        // create set active camera subscription
        m_subSetActiveCamera = std::make_shared<EventDataSubscriber<EventData_SetActiveCamera>>();

        // create our player camera view of the scene
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
        // udpate the camera pre-render, so it can adjust to any game logic changes
        m_pCamera->Update(800, 600);

        // XXX TODO - pass camera into scene manager so the renderables can be frustum culled ... maybe ?
        std::vector<RenderSet *> renderables = m_pSceneManager->GetRenderData();

        // Render the array of renderables from the given camera viewpoint
        m_pRenderer->Render(m_pCamera, renderables);
    }

    void GraphicsSystem::SubscribeToThreadTaskCreated(std::shared_ptr<AEventDataSubscriber> pSubscriber)
    {
        m_pubThreadTaskCreated->Subscribe(pSubscriber);
    }

    bool GraphicsSystem::VUpdate(double currentTime, double elapsedTime)
    {
        // add any new entities to the graphics system
        // update any existing entities in the scene
        // remove and destroyed entities from the scene.
        this->ReadSubscriptions();

        // udpate scene manager
        m_pSceneManager->Update(currentTime, elapsedTime);

        return m_pRenderer->Update(currentTime, elapsedTime);
    }

    void GraphicsSystem::SetAssetSystem(std::shared_ptr<AssetSystem> pAssets)
    {
        m_pAssets = pAssets;
    }

    void GraphicsSystem::ReadSubscriptions()
    {
        // read any published EventData_EntityCreated events that may have occured since the last update.
        while(std::shared_ptr<const EventData_EntityCreated> data = m_subEntityCreated->GetNextEvent())
        {
            LOG("Graphics system received EntityCreated event");
            m_pSceneManager->Add(data->GetEntity());
        }

        while(auto data = m_subSetActiveCamera->GetNextEvent())
        {
            LOG("Graphics system received SetActiveCamera event.");
            m_pCamera->SetCameraComponent(data->GetCameraComponent());
        }
    }

    std::shared_ptr<AEventDataSubscriber> GraphicsSystem::GetEntityCreatedSubscriber() const
    {
        return m_subEntityCreated;
    }

    std::shared_ptr<AEventDataSubscriber> GraphicsSystem::GetSetActiveCameraSubscriber() const
    {
        return m_subSetActiveCamera;
    }

    std::shared_ptr<Asset> GraphicsSystem::LoadShaderFile(const std::string & name)
    {
        std::string shader_path = std::string(name.c_str());
        shader_path.append(".");
        shader_path.append(GraphicsRenderer::sk_shader_extension.c_str());
        std::shared_ptr<Asset> asset = m_pAssets->GetAsset(shader_path.c_str());
        return asset;
    }
}
