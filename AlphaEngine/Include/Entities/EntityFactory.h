#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

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

#include <functional>
#include <map>
#include <memory>

namespace alpha
{
    class Entity;
    class EntityComponent;
    class Asset;
    class LuaVar;

    /** \brief Factory used to build entities based off of lua scripts.
     */
    class EntityFactory
    {
    public:
        EntityFactory();

        /** Creates an entity using the components outlined in the given script asset */
        std::shared_ptr<Entity> CreateEntity(std::shared_ptr<Asset> asset);

        /** Register a component, and allow the factory to generate them when creating an entity. */
        template <class SubClass>
        bool RegisterComponent(unsigned int componentId)
        {
            auto it = m_componentCreationFunctions.find(componentId);
            if (it == m_componentCreationFunctions.end())
            {
                // hurray for lambdas!
                m_componentCreationFunctions[componentId] = [] () { return new SubClass; };
                return true;
            }
            return false;
        }

    private:
        /** Creates a component using a registered creation function, if it exists. */
        std::shared_ptr<EntityComponent> CreateComponent(std::shared_ptr<LuaVar> var);

        std::map<unsigned int, std::function<EntityComponent *()> > m_componentCreationFunctions;

        unsigned int m_lastEntityId = 0;
    };
}

#endif // ENTITY_FACTORY_H
