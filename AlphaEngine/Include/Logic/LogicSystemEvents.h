#ifndef ALPHA_LOGIC_EVENTS_H
#define ALPHA_LOGIC_EVENTS_H

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

#include "Events/AEvent.h"

namespace alpha
{
    class Entity;
    class CameraComponent;

    /**
     * Event_EntityCreated
     * This event is published when any new Entity is created in the LogicSystem
     */
    class Event_EntityCreated : public AEvent
    {
    public:
        static const std::string sk_name;

        explicit Event_EntityCreated(std::shared_ptr<Entity> pEntity);

        virtual std::string VGetTypeName() const;
        virtual AEvent * VCopy();

        /** Retrieve the entity that was created. */
        std::shared_ptr<Entity> GetEntity() const;

    private:
        std::shared_ptr<Entity> m_pEntity;
    };

    /**
     * Event_SetActiveCamera
     * This event is published whenever the implementor of the game state logic requests that a
     * new camera be set as active.
     */
    class Event_SetActiveCamera : public AEvent
    {
    public:
        /** The type name that defines this event type */
        static const std::string sk_name;

        explicit Event_SetActiveCamera(std::weak_ptr<CameraComponent> pCameraComponent);

        virtual std::string VGetTypeName() const;
        virtual AEvent * VCopy();

        /** Retrieve the camera component to set as the active camera */
        std::weak_ptr<CameraComponent> GetCameraComponent() const;

    private:
        /** Weak pointer to the entity camera component to be set as the active camera */
        std::weak_ptr<CameraComponent> m_pCameraComponent;
    };
}

#endif // ALPHA_LOGIC_EVENTS_H