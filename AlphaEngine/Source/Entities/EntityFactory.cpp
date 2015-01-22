/**
Copyright 2014 Jason R. Wendlandt

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

#include "Entities/EntityFactory.h"
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Entities/TransformComponent.h"
#include "Entities/EntityScript.h"

#include "Assets/AssetSystem.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    EntityFactory::EntityFactory()
    {
        RegisterComponent<TransformComponent>(EntityComponent::GetIDFromName(TransformComponent::sk_name));
    }

    std::shared_ptr<Entity> EntityFactory::CreateEntity(std::shared_ptr<Asset> asset)
    {
        // TODO:
        // Creating a single entity is most likely never the common case.
        // More than likely we will need to batch create 2 or more entities
        // based on the same script asset.  This method should be enhanced so
        // that it can create multiple entities in a data oriented manner.

        // 1. get a list of components that the script wants to make
        
        // 2. use the data from the script, build each component once, so it can be copied
        
        // 3. for 0 to N: 
        //        create an entity
        //        for each component
        //            copy component
        //            add copy to entity.

        auto script = std::make_shared<EntityScript>(asset);
        auto entity = std::make_shared<Entity>(m_lastEntityId++, script);
        
        // iterate over the list of component variables list in the script
        // and create a component for that variable, if it is properly registered.
        auto vars = script->GetComponentVars();
        for (auto var_pair : vars)
        {
            unsigned int component_id = EntityComponent::GetIDFromName(var_pair.first.c_str());
            std::shared_ptr<EntityComponent> component = this->CreateComponent(var_pair.first, var_pair.second);

            if (component != nullptr)
            {
                entity->Add(component_id, component);
                LOG("EntityFactory -> Component added to entity: ", var_pair.first);
            }
            else
            {
                LOG_WARN("EntityFactory -> Attempted to add an invalid component type: ", var_pair.first);
            }
        }

        return entity;
    }

    std::shared_ptr<EntityComponent> EntityFactory::CreateComponent(const std::string & name, std::shared_ptr<LuaVar> data)
    {
        unsigned int component_id = EntityComponent::GetIDFromName(name.c_str());
        auto it = m_componentCreationFunctions.find(component_id);

        if (it != m_componentCreationFunctions.end())
        {
            std::function<EntityComponent *()> func = it->second;
            std::shared_ptr<EntityComponent> component(func());
            component->VInitialize(data);
            return component;
        }
        return nullptr;
    }
}
