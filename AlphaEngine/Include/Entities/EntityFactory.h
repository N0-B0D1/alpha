#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

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

#include <map>
#include <memory>

namespace alpha
{
    class Entity;
    class EntityComponent;

    class EntityFactory
    {
    public:
        EntityFactory();

        std::shared_ptr<Entity> CreateEntity(const char *resource);

        // component creation function
        template <class SubClass>
        EntityComponent * ComponentObjectCreationFunction(void) { return new SubClass; }

        // register a component, and allow the factory to generate them
        // when creating an entity.
        template <class SubClass>
        bool RegisterComponent(unsigned long componentId)
        {
            auto it = m_componentCreationFunctions.find(componentId);
            if (it == m_componentCreationFunctions.end())
            {
                m_componentCreationFunctions[componentId] = &ComponentObjectCreationFunction<SubClass>;
                return true;
            }
            return false;
        }

        EntityComponent * Create(unsigned long componentId);

    private:
        typedef EntityComponent * (*ComponentCreationFunction)(void);
        std::map<unsigned long, ComponentCreationFunction> m_componentCreationFunctions;
    };
}

#endif // ENTITY_FACTORY_H
