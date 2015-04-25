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
#include <vector>

#include "Events/EventDataPublisher.h"
#include "Events/EventData_ThreadTaskCreated.h"

namespace alpha
{
    class AssetSystem;
    class Entity;
    class EntityComponent;
    class SceneNode;
    class RenderSet;
    class Light;

    /**
     * \brief The SceneManager manages the logical scene layout.
     */
    class SceneManager
    {
    public:
        explicit SceneManager(std::weak_ptr<EventDataPublisher<EventData_ThreadTaskCreated>> pTaskPublisher, std::weak_ptr<AssetSystem> pAssets);
        virtual ~SceneManager();

        /** Keeps the Render Data structurs up to date, and preped for rendering if needed */
        bool Update(double currentTime, double elapsedTime);

        /**
         * \brief Constructs and array of data to be rendered on the next render call.
         */
        const std::vector<RenderSet *> & GetRenderData();

        /**
         * \brief Retrieve the lastest list of lights that is ready to be rendered.
         */
        const std::vector<Light *> & GetLightData();

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
        std::map<unsigned int, SceneNode *> CreateNodes(const std::map<unsigned int, std::shared_ptr<EntityComponent> > components, SceneNode * pParent);

        /** Recursively build render data for an entities scene node map */
        void BuildRenderData(unsigned int entity_id, std::map<unsigned int, SceneNode *> nodes, std::vector<RenderSet *> & renderables, std::vector<Light *> & lights) const;

        /** Handle to the asset system, so that the scene manager can pull in any necessary assets */
        std::weak_ptr<AssetSystem> m_pAssets;

        /** Map of entity ID to SceneNode maps */
        std::map<unsigned int, std::map<unsigned int, SceneNode *> > m_nodes;
        /** Store Render Data array for easy retrieval when rendering. */
        std::vector<RenderSet *> m_vRenderData;
        /** Store a list of lights for use on the next render call. */
        std::vector<Light *> m_vLightData;

        /** Handle to task publisher for creating thread tasks. */
        std::weak_ptr<EventDataPublisher<EventData_ThreadTaskCreated>> m_pTaskPublisher;
    };
}

#endif // ALPHA_SCENE_MANAGER_H
