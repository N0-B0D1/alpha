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
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    SceneManager::~SceneManager() { }

    bool SceneManager::Add(const std::shared_ptr<Entity> & entity)
    {
        unsigned int entity_id = entity->GetId();
        auto search = m_nodes.find(entity_id);
        if (search == m_nodes.end())
        {
            auto components = entity->GetComponents();
            m_nodes[entity_id] = this->CreateNodes(components);
            return true;
        }
        return false;
    }

    bool SceneManager::Update(const std::shared_ptr<Entity> & entity)
    {
        auto search = m_nodes.find(entity->GetId());
        if (search != m_nodes.end())
        {
            return true;
        }
        return false;
    }

    bool SceneManager::Remove(const std::shared_ptr<Entity> & entity)
    {
        auto search = m_nodes.find(entity->GetId());
        if (search != m_nodes.end())
        {
            return true;
        }
        return false;
    }

    std::map<unsigned int, std::shared_ptr<SceneNode> > SceneManager::CreateNodes(const std::map<unsigned int, std::shared_ptr<EntityComponent> > components)
    {
        std::map<unsigned int, std::shared_ptr<SceneNode> > nodes;

        for (auto component : components)
        {
            LOG("SceneManager ", "Creating SceneNode for entity.");

            // do a depth first creation, so the list of child nodes can be passed into the scene node creation.
            std::map<unsigned int, std::shared_ptr<SceneNode> > child_nodes = this->CreateNodes(component.second->GetComponents());

            std::shared_ptr<SceneNode> node = nullptr;
            if (component.second->VIsRenderable())
            {
                // make a scene node for this component
                std::shared_ptr<SceneComponent> scene_component = std::dynamic_pointer_cast<SceneComponent>(component.second);
                node = std::make_shared<SceneNode>(scene_component, child_nodes);
            }
            else
            {
                // XXX if this component is not renderable, create a space node for it
                // For now this is just the easy approach, so that the scene node 
                // hierarchy mirrors the component hierarchy
                // We could potentially add debug/editor render data for these nodes later?
                node = std::make_shared<SceneNode>(nullptr, child_nodes);
            }

            nodes[component.first] = node;
        }

        return nodes;
    }
}
