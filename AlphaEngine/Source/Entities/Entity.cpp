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
        , m_updated(false)
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
        auto it = m_allComponents.find(component_id);
        if (it != m_allComponents.end())
        {
            LOG_WARN("  <Entity> Attempt to add a component type that already exists: type: ", component->VGetName());
        }
        else 
        {
            m_allComponents[component_id] = component;
            auto spParent = component->GetParent().lock();
            if (spParent == nullptr)
            {
                // this component is a root level component
                // so also store it in the vector array
                m_rootComponents[component_id] = component;
            }
        }
    }

    std::shared_ptr<EntityComponent> Entity::Get(const std::string & component_name)
    {
        // hash the component name
        unsigned int component_id = EntityComponent::GetIDFromName(component_name);

        // find the component by its hashed name id
        auto it = m_allComponents.find(component_id);
        if (it != m_allComponents.end())
        {
            return it->second;
        }
        return nullptr;
    }

    /*
    void Entity::Remove(unsigned int component_id)
    {
        auto it = m_components.find(component_id);
        if (it != m_components.end())
        {
            m_components.erase(it);
        }
    }
    */

    const std::map<unsigned int, std::shared_ptr<EntityComponent> > Entity::GetComponents() const
    {
        return m_rootComponents;
    }

    void Entity::SetUpdated(bool updated)
    {
        m_updated = updated;
    }

    bool Entity::IsUpdated() const
    {
        return m_updated;
    }
}
