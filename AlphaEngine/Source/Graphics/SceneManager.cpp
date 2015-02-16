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
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    SceneManager::~SceneManager() { }

    bool SceneManager::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        // clean the list, so it can be rebuilt.
        while (m_vRenderData.size() > 0)
        {
            m_vRenderData.pop_back();
        }

        // walk the tree, and create render data for renderable scene nodes.
        for (auto iter : m_nodes)
        {
            this->BuildRenderData(iter.first, iter.second, m_vRenderData);
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

            //std::shared_ptr<SceneNode> node = nullptr;
            std::shared_ptr<SceneComponent> scene_component = std::dynamic_pointer_cast<SceneComponent>(component.second);
            // at this point we either have nullptr or an actual scene component
            // either way make a scene node for it, and the nullptr will just
            // exist as a spacer in the tree, so we don't have to reconstruct the
            // complex component hierarchy minus none scene components.
            nodes[component.first] = std::make_shared<SceneNode>(scene_component, child_nodes);
        }

        return nodes;
    }

    void SceneManager::BuildRenderData(unsigned int entity_id, std::map<unsigned int, std::shared_ptr<SceneNode> > nodes, std::vector<RenderData *> & renderables) const
    {
        // XXX not sure if the entity id is neede at this point ... refactor as needed.

        // for each node
        for (auto iter : nodes)
        {
            // check this node
            if (iter.second->IsRenderable())
            {
                // make render data for this node
                //RenderData * rd = new RenderData();
                //renderables.push_back(rd);
                renderables.push_back(iter.second->GetRenderData());
            }

            // recurse each child 
            this->BuildRenderData(entity_id, iter.second->GetChildren(), renderables);
        }
    }
}
