#ifndef EVENT_DATA_ENTITY_CREATED_H
#define EVENT_DATA_ENTITY_CREATED_H

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

#include <memory>

#include "Events/EventData.h"
#include "Entities/Entity.h"

namespace alpha
{
    class EventData_EntityCreated : public EventData
    {
        static const std::string sk_name;

    public:
        explicit EventData_EntityCreated(std::shared_ptr<Entity> entity);
        //virtual ~EventData_EntityCreated();

        virtual std::string VGetTypeName() const;

        std::shared_ptr<Entity> GetEntity() const;

    private:
        std::shared_ptr<Entity> m_entity;
    };
}

#endif // EVENT_DATA_ENTITY_CREATED_H
