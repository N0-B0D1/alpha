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
    { }
    GraphicsSystem::~GraphicsSystem() { }

    bool GraphicsSystem::VInitialize()
    {
        // XXX temporary, load some basic shaders to use for everything until proper load pipeline is implemented
        auto ps_shader = this->LoadShaderFile("Shaders/Basic_PS");
        auto vs_shader = this->LoadShaderFile("Shaders/Basic_VS");

        // OS specific renderer
        m_pRenderer = new GraphicsRenderer();
        if (!m_pRenderer->Initialize())
        {
            return false;
        }
        m_pRenderer->SetBasicShaders(ps_shader, vs_shader);

        // Scene renerable manager
        m_pSceneManager = std::unique_ptr<SceneManager>(new SceneManager());

        // create event subscribers
        m_subEntityCreated = std::make_shared<EventDataSubscriber<EventData_EntityCreated> >();

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

    void GraphicsSystem::Render()
    {
        m_pRenderer->Render();
    }

    bool GraphicsSystem::VUpdate(double currentTime, double elapsedTime)
    {
        // add any new entities to the graphics system
        // update any existing entities in the scene
        // remove and destroyed entities from the scene.
        this->ReadSubscription();

        return m_pRenderer->Update(currentTime, elapsedTime);
    }

    void GraphicsSystem::SetAssetSystem(std::shared_ptr<AssetSystem> pAssets)
    {
        m_pAssets = pAssets;
    }

    void GraphicsSystem::ReadSubscription()
    {
        // read any published EventData_EntityCreated events that may have occured since the last update.
        while(std::shared_ptr<const EventData_EntityCreated> data = m_subEntityCreated->GetNextEvent())
        {
            LOG("Graphics system received EntityCreated event");
            m_pSceneManager->Add(data->GetEntity());
        }
    }

    std::shared_ptr<AEventDataSubscriber> GraphicsSystem::GetEntityCreatedSubscriber() const
    {
        return m_subEntityCreated;
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
