#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

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
#include <string>

#include "Math/Transform.h"

namespace alpha
{
    class LuaVar;

    /** \brief Abstract base class for all entity components. 
     * A component might represet a single piece of logic or renderable instanced for the entity it 
     * is attached to. 
     * */
    class EntityComponent
    {
    public:
        virtual ~EntityComponent();

        /** Initialize the component from a script variable. */
        virtual void VInitialize(std::shared_ptr<LuaVar> var) = 0;
        /** Tick the component. */
        virtual bool VUpdate(float fCurrentTime, float fElapsedTime) = 0;

        /** Get the hashed string ID for the derived component. */
        unsigned int GetID() const;
        /** Get the name that represents the component type, NOT the component instance. */
        virtual std::string VGetName() const = 0;
        /** Hashes the given string and returns the unsigned int representation. */
        static unsigned int GetIDFromName(const std::string & name);

    protected:
        /** Reference to this components parent, null if is top-level component */ 
        std::shared_ptr<EntityComponent> m_parent;
    };

    /** \brief A component base class used to represent a geometrical object in the scene.
     * A scene component always has a transform which represents its relative position, orientation, 
     * and scale relative to its parent component.  If the parent is null then the transform is relative 
     * to the world. 
     */
    class SceneComponent : public EntityComponent
    {
    public:
        virtual ~SceneComponent();

    private:
        /** This components relative transform */
        Transform m_transform;
    };
}

#endif // ENTITY_COMPONENT_H
