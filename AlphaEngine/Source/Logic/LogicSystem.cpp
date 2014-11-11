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

    std::shared_ptr<Entity> LogicSystem::GetEntity(const unsigned long entityId)
    {
        auto it = m_entities.find(entityId);
        if (it != m_entities.end())
        {
            return it->second;
        }
        return std::shared_ptr<Entity>();
    }

    std::shared_ptr<Entity> LogicSystem::CreateEntity(const char * /*resource*/)
    {
        return std::shared_ptr<Entity>();
    }

    void LogicSystem::DestroyEntity(const unsigned long entityId)
    {
        m_entities.erase(m_entities.find(entityId));
    }
}
