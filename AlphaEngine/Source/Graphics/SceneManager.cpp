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
#include "Graphics/RenderData.h"
#include "Graphics/RenderDataTask.h"
#include "Assets/AssetSystem.h"
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Entities/MeshComponent.h"
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

        // walk the tree, and create render data for renderable scene nodes.
        // make a task for each entity scene node set, and publish it
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

        return true;
    }

    std::vector<RenderData *> & SceneManager::GetRenderData()
    {
        // get the latest set of render data to be rendered.
        return m_vRenderData;
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

            // do a depth first creation, so the list of child nodes can be passed into the scene node creation.
            std::map<unsigned int, SceneNode *> child_nodes = this->CreateNodes(component.second->GetComponents(), node);
            node->SetChildren(child_nodes);

            nodes[component.first] = node;
        }

        return nodes;
    }

    void SceneManager::BuildRenderData(unsigned int entity_id, std::map<unsigned int, SceneNode *> nodes, std::vector<RenderData *> & renderables) const
    {
        // XXX not sure if the entity id is neede at this point ... refactor as needed.

        // for each node
        for (auto iter : nodes)
        {
            // check this node
            if (iter.second->IsRenderable())
            {
                // make render data for this node
                renderables.push_back(iter.second->GetRenderData());
            }

            // recurse each child node
            this->BuildRenderData(entity_id, iter.second->GetChildren(), renderables);
        }
    }
}
