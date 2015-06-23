#ifndef ENTITY_H
#define ENTITY_H

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

#include <map>
#include <memory>
#include <vector>

namespace alpha
{
    class EntityScript;
    class EntityComponent;

    class Entity
    {
    public:
        Entity(unsigned long entityId, std::shared_ptr<EntityScript> script);
        virtual ~Entity();

        bool VUpdate(float fCurrentTime, float fElapsedTime);

        unsigned long GetId() const;
        std::shared_ptr<EntityScript> GetScript() const;

        void Add(unsigned int component_id, std::shared_ptr<EntityComponent> component);
        std::shared_ptr<EntityComponent> Get(const std::string & component_name);
        //void Remove(unsigned int component_id);

        /** Retrieve the map container of all components belonging to this entity instance. */
        const std::map<unsigned int, std::shared_ptr<EntityComponent> > GetComponents() const;

        /** Mark the entity as updated so that it will be pushed out to other systems. */
        void SetUpdated(bool updated);
        /** Check to see if the entity has been updated */
        bool IsUpdated() const;

    private:
        unsigned long m_entityId;

        /** The script instance that this entity is based on. */
        std::shared_ptr<EntityScript> m_script;

        /** Map containing every component belonging to this entity. */
        std::map<unsigned int, std::shared_ptr<EntityComponent> > m_allComponents;
        /** Root map container only contains root level components, so it can be treated more like a tree */
        std::map<unsigned int, std::shared_ptr<EntityComponent> > m_rootComponents;

        /** Flag signifying if the entity has been updated. */
        bool m_updated;
    };
}

#endif // ENTITY_H
