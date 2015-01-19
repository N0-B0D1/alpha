#ifndef ENTITY_H
#define ENTITY_H

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

namespace alpha
{
    class EntityScript;

    class Entity
    {
    public:
        explicit Entity(unsigned long entityId, std::shared_ptr<EntityScript> script);
        virtual ~Entity();

        bool VUpdate(float fCurrentTime, float fElapsedTime);

        unsigned long GetId() const;
        std::shared_ptr<EntityScript> GetScript() const;

    private:
        unsigned long m_entityId;
        std::shared_ptr<EntityScript> m_script;
    };
}

#endif // ENTITY_H
