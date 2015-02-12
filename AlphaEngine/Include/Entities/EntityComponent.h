#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

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
#include <string>

#include "Math/Transform.h"

namespace alpha
{
    class LuaVar;
    class LuaTable;

    /** \brief Abstract base class for all entity components. 
     * A component might represet a single piece of logic or renderable instanced for the entity it 
     * is attached to. 
     * */
    class EntityComponent
    {
    public:
        virtual ~EntityComponent();

        /** set the parent component */
        void SetParent(const std::shared_ptr<EntityComponent> & parent);

        /** Initialize the component from a script variable. */
        virtual void VInitialize(std::shared_ptr<LuaVar> var) = 0;
        /** Tick the component. */
        virtual bool VUpdate(float fCurrentTime, float fElapsedTime) = 0;

        /** Add a child component to this component */
        void Attach(unsigned int component_id, std::shared_ptr<EntityComponent> component);

        /** Get all sub-components in this entity. */
        const std::map<unsigned int, std::shared_ptr<EntityComponent> > GetComponents() const;

        /** Get the hashed string ID for the derived component. */
        unsigned int GetID() const;
        /** Get the name that represents the component type, NOT the component instance. */
        virtual std::string VGetName() const = 0;
        /** Hashes the given string and returns the unsigned int representation. */
        static unsigned int GetIDFromName(const std::string & name);

        /** Helper function for quickly identifying a SceneComponent from a non-renderable logic component. */
        virtual bool VIsRenderable() const;

    protected:
        /** Reference to this components parent, null if is top-level component */ 
        std::shared_ptr<EntityComponent> m_parent;

        /** Child components, any component can be a child of another component for the greatest flexibility. */
        std::map<unsigned int, std::shared_ptr<EntityComponent> > m_components;
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

        /** Base SceneComponent handles initialization of transform data */
        virtual void VInitialize(std::shared_ptr<LuaVar> var);

        virtual bool VIsRenderable() const;

    private:
        /** Helper function for retrieving x, y, or z vars from script tables */
        float GetAxis(std::shared_ptr<LuaTable> table, const std::string axis);

        /** This components relative transform */
        Transform m_transform;
    };
}

#endif // ENTITY_COMPONENT_H
