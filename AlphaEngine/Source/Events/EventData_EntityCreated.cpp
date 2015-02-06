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

#include "Events/EventData_EntityCreated.h"

namespace alpha
{
    const std::string EventData_EntityCreated::sk_name = "EventData_EntityCreated";

    EventData_EntityCreated::EventData_EntityCreated(std::shared_ptr<Entity> entity)
        : m_entity(entity)
    { }
    //EventData_EntityCreated::~EventData_EntityCreated() { }

    std::string EventData_EntityCreated::VGetTypeName() const
    {
        return EventData_EntityCreated::sk_name;
    }

    std::shared_ptr<Entity> EventData_EntityCreated::GetEntity() const
    {
        return m_entity;
    }
}
