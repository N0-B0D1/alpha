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

#include "Graphics/SceneManager.h"
#include "Graphics/SceneNode.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Light.h"
#include "Assets/AssetSystem.h"
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Entities/MeshComponent.h"
#include "Entities/LightComponent.h"
#include "Events/EventDataPublisher.h"
#include "Events/EventData_ThreadTaskCreated.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    SceneManager::SceneManager(std::weak_ptr<EventDataPublisher<EventData_ThreadTaskCreated>> pTaskPublisher, std::weak_ptr<AssetSystem> pAssets)
        : m_pAssets(pAssets)
        , m_pTaskPublisher(pTaskPublisher)
    { }
    SceneManager::~SceneManager()
    {
        for (auto pair : m_nodes)
        {
            // delete all entity scene nodes properly
            for (auto nodepair : pair.second)
            {
                delete nodepair.second;
            }
            pair.second.clear();
        }
        m_nodes.clear();
    }

    bool SceneManager::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        // clean the list, so it can be rebuilt.
        m_vRenderData.clear();
        m_vLightData.clear();

        // walk the tree, and create render data for renderable scene nodes.
        // make a task for each entity scene node set, and publish it

        /*
        // XXX This doesn't actually work ... animations are extremely clunky
        if (auto spPublisher = m_pTaskPublisher.lock())
        {
            for (auto pair : m_nodes)
            {
                Task * pTask = new RenderDataTask(pair.second);
                auto pEvent = std::make_shared<EventData_ThreadTaskCreated>(pTask);
                spPublisher->Publish(pEvent);
                this->BuildRenderData(pair.first, pair.second, m_vRenderData);
            }
        }
        */

        for (auto pair : m_nodes)
        {
            this->BuildRenderData(pair.first, pair.second, m_vRenderData, m_vLightData);
        }

        return true;
    }

    const std::vector<RenderSet *> & SceneManager::GetRenderData()
    {
        // get the latest set of render data to be rendered.
        return m_vRenderData;
    }

    const std::vector<Light *> & SceneManager::GetLightData()
    {
        return m_vLightData;
    }

    bool SceneManager::Add(const std::shared_ptr<Entity> & entity)
    {
        unsigned int entity_id = entity->GetId();
        auto search = m_nodes.find(entity_id);
        if (search == m_nodes.end())
        {
            auto components = entity->GetComponents();
            m_nodes[entity_id] = this->CreateNodes(components, nullptr);
            return true;
        }
        return false;
    }

    bool SceneManager::Update(const std::shared_ptr<Entity> & /*entity*/)
    {
        /*
        auto search = m_nodes.find(entity->GetId());
        if (search != m_nodes.end())
        {
            return true;
        }
        return false;
        */
        return true;
    }

    bool SceneManager::Remove(const std::shared_ptr<Entity> & /*entity*/)
    {
        /*
        auto search = m_nodes.find(entity->GetId());
        if (search != m_nodes.end())
        {
            return true;
        }
        return false;
        */
        return false;
    }

    std::map<unsigned int, SceneNode *> SceneManager::CreateNodes(const std::map<unsigned int, std::shared_ptr<EntityComponent> > components, SceneNode * pParent)
    {
        std::map<unsigned int, SceneNode *> nodes;

        for (auto component : components)
        {
            // creat this node
            std::shared_ptr<SceneComponent> scene_component = std::dynamic_pointer_cast<SceneComponent>(component.second);
            auto node = new SceneNode(pParent, scene_component);

            // add any necessary assets to the scene node
            // XXX this should also happen during update incase the model asset is changed anytime during 
            // the scene components life cycle
            if (auto mesh_component = std::dynamic_pointer_cast<MeshComponent>(scene_component))
            {
                auto path = mesh_component->GetMeshPath();
                if (auto pAssets = m_pAssets.lock())
                {
                    auto asset = pAssets->GetAsset(path.c_str());
                    node->SetMesh(asset);
                }
            }

            // get the material path, load as an asset, and set it on the node.
            auto material_path = scene_component->GetMaterialPath();
            std::shared_ptr<Material> pMaterial;
            if (auto pAssets = m_pAssets.lock())
            {
                auto pAsset = pAssets->GetAsset(material_path.c_str());

                // XXX TODO - pass asset through a material manager, so that only one
                // material every exists for a given material script.
                pMaterial = std::make_shared<Material>(pAsset);
                node->SetMaterial(pMaterial);
            }

            if (auto light_component = std::dynamic_pointer_cast<LightComponent>(scene_component))
            {
                // create a light based on the light component and set it on the scene node
                Light * pLight = new Light(light_component);
                node->SetLight(pLight);
            }

            // do a depth first creation, so the list of child nodes can be passed into the scene node creation.
            std::map<unsigned int, SceneNode *> child_nodes = this->CreateNodes(component.second->GetComponents(), node);
            node->SetChildren(child_nodes);

            nodes[component.first] = node;
        }

        return nodes;
    }

    void SceneManager::BuildRenderData(unsigned int entity_id, std::map<unsigned int, SceneNode *> nodes, std::vector<RenderSet *> & renderables, std::vector<Light *> & lights) const
    {
        // XXX not sure if the entity id is needed at this point ... refactor as needed.

        // for each node
        for (auto iter : nodes)
        {
            // check this node
            auto node = iter.second;
            
            // prep the world transform for this node
            Matrix world_transform = node->GetWorldTransform();

            Light * pLight = node->GetLight();

            // make render data for this node
            if (RenderSet * rs = node->GetRenderSet())
            {
                rs->worldTransform = world_transform;

                // if this renderable has a light attached
                // then it emits light and should use a different
                // shader
                rs->emitsLight = node->EmitsLight();

                // set the render sets material
                rs->material = node->GetMaterial();

                renderables.push_back(rs);
            }

            // see if it is a light
            if (pLight != nullptr)
            {
                pLight->worldTransform = world_transform;
                lights.push_back(pLight);
            }

            // recurse each child node
            this->BuildRenderData(entity_id, node->GetChildren(), renderables, lights);
        }
    }
}
