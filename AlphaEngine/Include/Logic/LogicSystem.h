#ifndef LOGIC_SYSTEM_H
#define LOGIC_SYSTEM_H

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
#include "AlphaSystem.h"
//#include "Entities/Entity.h"

namespace alpha
{
    class AssetSystem;
    class EntityFactory;
    class Entity;

    class LogicSystem : public AlphaSystem
    {
    public:
        LogicSystem();
        virtual ~LogicSystem();

        virtual bool VInitialize(std::shared_ptr<AssetSystem> pAssets);
        virtual bool VShutdown();

        /** Entity life-cycle methods */
        std::shared_ptr<Entity> GetEntity(const unsigned long entityId);
        std::shared_ptr<Entity> CreateEntity(const char * resource);
        void DestroyEntity(const unsigned long entityId);

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);
        
        EntityFactory *m_pEntityFactory;
        std::map<unsigned long, std::shared_ptr<Entity> > m_entities;

        std::shared_ptr<AssetSystem> m_pAssets;
    };
}

#endif // LOGIC_SYSTEM_H
