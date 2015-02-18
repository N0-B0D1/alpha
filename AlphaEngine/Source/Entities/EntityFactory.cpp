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

#include <memory>

#include "Entities/EntityFactory.h"
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Entities/PrimitiveComponent.h"
#include "Entities/EntityScript.h"

#include "Assets/AssetSystem.h"
#include "Scripting/LuaVar.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    EntityFactory::EntityFactory()
    {
        RegisterComponent<PrimitiveComponent>(EntityComponent::GetIDFromName(PrimitiveComponent::sk_name));
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
            if (var_pair.second->GetVarType() == VT_TABLE)
            {
                this->CreateComponent(entity, nullptr, var_pair.first, var_pair.second);
            }
        }

        return entity;
    }

    void EntityFactory::CreateComponent(std::shared_ptr<Entity> entity, std::shared_ptr<EntityComponent> parent_component, const std::string & var_name, std::shared_ptr<LuaVar> var_value)
    {
        // convert var data to a table so we can access its elements
        auto table = std::dynamic_pointer_cast<LuaTable>(var_value);

        // get the type name for this component
        auto type_name = std::dynamic_pointer_cast<LuaStatic<std::string>>(table->Get("type"));
        if (type_name != nullptr)
        {
            LOG("EntityFactory -> Attempting to create component type: ", type_name->GetValue());

            // convert the variable name into a component id, and type name into type_id
            unsigned int type_id = EntityComponent::GetIDFromName(type_name->GetValue());
            unsigned int component_id = EntityComponent::GetIDFromName(var_name);

            // get the creation function for this component type, if it exists
            auto it = m_componentCreationFunctions.find(type_id);
            if (it != m_componentCreationFunctions.end())
            {
                // make the component, and initialize it will the var data
                std::function<EntityComponent *()> func = it->second;
                std::shared_ptr<EntityComponent> component(func());
                component->VInitialize(table);

                // if var data contains a 'components' variable
                // then create each component in that table and
                // add it to this component
                std::shared_ptr<LuaVar> child_components = table->Get("components");
                if (child_components != nullptr && child_components->GetVarType() == VT_TABLE)
                {
                    auto child_table = std::dynamic_pointer_cast<LuaTable>(child_components);
                    for (auto var_pair : child_table->GetAll())
                    {
                        if (var_pair.second->GetVarType() == VT_TABLE)
                        {
                            //unsigned int child_component_id = EntityComponent::GetIDFromName(var_pair.first);
                            this->CreateComponent(entity, component, var_pair.first, var_pair.second);
                        }
                    }
                }

                // always add the component directly to the entity, so it can be easily retrieved.
                entity->Add(component_id, component);

                // then if this is a sub-component in the tree, also add it to the parent component
                if (parent_component != nullptr)
                {
                    parent_component->Attach(component_id, component);
                }
            }
            else
            {
                LOG_WARN("EntityFactory > Attempted to add an invalid component type: ", type_name->GetValue());
            }
        }
        else
        {
            LOG_WARN("EntityFactory > Attempted to add a component without a valid 'type'.");
        }
    }
}
