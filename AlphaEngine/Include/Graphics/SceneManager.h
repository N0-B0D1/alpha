#ifndef ALPHA_SCENE_MANAGER_H
#define ALPHA_SCENE_MANAGER_H

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

namespace alpha
{
    class Entity;
    class EntityComponent;
    class SceneNode;

    /**
     * \brief The SceneManager manages the logical scene layout.
     */
    class SceneManager
    {
    public:
        virtual ~SceneManager();

        /**
         * \brief Add an entity to the scene.
         * \param entity Shared pointer to an entity instance.
         */
        bool Add(const std::shared_ptr<Entity> & entity);
        /**
         * \brief Update and existing entity in the scene.
         * \param entity Shared pointer to an entity instance.
         */
        bool Update(const std::shared_ptr<Entity> & entity);
        /**
         * \brief Remove an entity from the scene.
         * \param entity Shared pointer to an entity instance.
         */
        bool Remove(const std::shared_ptr<Entity> & entity);

    private:
        /**
         * \brief Given an entity component, recuresively add SceneNodes.
         */
        std::map<unsigned int, std::shared_ptr<SceneNode> > CreateNodes(const std::map<unsigned int, std::shared_ptr<EntityComponent> > components);

        /** Map of entity ID to SceneNode maps */
        std::map<unsigned int, std::map<unsigned int, std::shared_ptr<SceneNode> > > m_nodes;
    };
}

#endif // ALPHA_SCENE_MANAGER_H
