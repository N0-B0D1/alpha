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
#include "Entities/EntityScript.h"
#include "Assets/AssetSystem.h"

namespace alpha
{
    EntityFactory::EntityFactory() { }

    std::shared_ptr<Entity> EntityFactory::CreateEntity(std::shared_ptr<Asset> asset)
    {
        auto entity = std::make_shared<Entity>(m_lastEntityId++);
        auto script = new EntityScript(asset);

        // get components data
        //auto components = script->getComponents();

        // make each component, and add it to the entity

        // dispose of the script
        // XXX we probably want to hold onto it for a limited amout of time
        // for reuse, the general case is probably not to make one single entity.
        delete script;

        return entity;
    }

    EntityComponent * EntityFactory::Create(unsigned long componentId)
    {
        auto it = m_componentCreationFunctions.find(componentId);
        if (it != m_componentCreationFunctions.end())
        {
            ComponentCreationFunction pFunc = it->second;
            return pFunc();
        }
        return nullptr;
    }
}
