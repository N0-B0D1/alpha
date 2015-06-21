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

#include "Logic/LogicSystemEvents.h"
#include "Entities/Entity.h"

namespace alpha
{
    const std::string Event_EntityCreated::sk_name = "Event_EntityCreated";

    Event_EntityCreated::Event_EntityCreated(std::shared_ptr<Entity> pEntity)
        : m_pEntity(pEntity)
    { }

    std::string Event_EntityCreated::VGetTypeName() const
    {
        return Event_EntityCreated::sk_name;
    }

    AEvent * Event_EntityCreated::VCopy()
    {
        return new Event_EntityCreated(m_pEntity);
    }

    std::shared_ptr<Entity> Event_EntityCreated::GetEntity() const
    {
        return m_pEntity;
    }




    const std::string Event_SetActiveCamera::sk_name = "EventData_SetActiveCamera";

    Event_SetActiveCamera::Event_SetActiveCamera(std::weak_ptr<CameraComponent> pCameraComponent)
        : m_pCameraComponent(pCameraComponent)
    { }

    std::string Event_SetActiveCamera::VGetTypeName() const
    {
        return Event_SetActiveCamera::sk_name;
    }

    AEvent * Event_SetActiveCamera::VCopy()
    {
        return new Event_SetActiveCamera(m_pCameraComponent);
    }

    std::weak_ptr<CameraComponent> Event_SetActiveCamera::GetCameraComponent() const
    {
        return m_pCameraComponent;
    }
}