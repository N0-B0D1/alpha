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

#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"

#include "Toolbox/Logger.h"

namespace alpha
{
    Entity::Entity(unsigned long entityId, std::shared_ptr<EntityScript> script)
        : m_entityId(entityId)
        , m_script(script)
    { }
    Entity::~Entity() { }

    bool Entity::VUpdate(float /*fCurrentTime*/, float /*fElapsedTime*/)
    {
        return true;
    }

    unsigned long Entity::GetId() const
    {
        return m_entityId;
    }

    std::shared_ptr<EntityScript> Entity::GetScript() const
    {
        return m_script;
    }

    void Entity::Add(unsigned int component_id, std::shared_ptr<EntityComponent> component)
    {
        auto it = m_components.find(component_id);
        if (it != m_components.end())
        {
            LOG_WARN("  <Entity> Attempt to add a component type that already exists: <type: ", component->VGetName());
        }
        else 
        {
            m_components[component_id] = component;
        }
    }

    std::shared_ptr<EntityComponent> Entity::Get(unsigned int component_id)
    {
        auto it = m_components.find(component_id);
        if (it != m_components.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void Entity::Remove(unsigned int component_id)
    {
        auto it = m_components.find(component_id);
        if (it != m_components.end())
        {
            m_components.erase(it);
        }
    }

    const std::map<unsigned int, std::shared_ptr<EntityComponent> > Entity::GetComponents() const
    {
        return m_components;
    }
}
