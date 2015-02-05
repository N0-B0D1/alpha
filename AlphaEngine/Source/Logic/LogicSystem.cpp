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

#include "Logic/LogicSystem.h"
#include "Entities/EntityFactory.h"
#include "Entities/Entity.h"
#include "Toolbox/Logger.h"
#include "Assets/AssetSystem.h"

#include "Events/EventData_EntityCreated.h"

namespace alpha
{
    LogicSystem::LogicSystem()
        : AlphaSystem(60)
        , m_pEntityFactory(nullptr)
    { }
    LogicSystem::~LogicSystem() { }

    bool LogicSystem::VInitialize()
    {
        m_pEntityFactory = new EntityFactory();
        if (!m_pEntityFactory)
        {
            LOG_ERR("LogicSystem: Failed to create EntityManager");
            return false;
        }
        return true;
    }

    bool LogicSystem::VShutdown()
    {
        m_entities.clear();

        if (m_pEntityFactory)
        {
            delete m_pEntityFactory;
        }
        return true;
    }

    bool LogicSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        return true;
    }

    void LogicSystem::SetAssetSystem(std::shared_ptr<AssetSystem> pAssets)
    {
        m_pAssets = pAssets;
    }

    std::shared_ptr<Entity> LogicSystem::GetEntity(const unsigned long entityId)
    {
        auto it = m_entities.find(entityId);
        if (it != m_entities.end())
        {
            return it->second;
        }
        return std::shared_ptr<Entity>();
    }

    std::shared_ptr<Entity> LogicSystem::CreateEntity(const char * resource)
    {
        std::shared_ptr<Entity> new_entity = nullptr;

        // get the entity script resource specified, and make an entity with it
        auto asset = m_pAssets->GetAsset(resource);
        if (asset != nullptr)
        {
            new_entity = m_pEntityFactory->CreateEntity(asset);
            m_entities[new_entity->GetId()] = new_entity;
        }

        // create an Entity Created event, and publish it to all subscribers
        std::shared_ptr<EventData_EntityCreated> event = std::make_shared<EventData_EntityCreated>(new_entity);
        m_pubEntityCreated.Publish(event);

        // might be nullptr
        return new_entity;
    }

    void LogicSystem::DestroyEntity(const unsigned long entityId)
    {
        m_entities.erase(m_entities.find(entityId));
    }

    void LogicSystem::SubscribeToEntityCreated(std::shared_ptr<AEventDataSubscriber> pSubscriber)
    {
        m_pubEntityCreated.Subscribe(pSubscriber);
    }
}
