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

#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace alpha
{
    class LuaVar;
    class LuaTable;

    /**
     * \brief Abstract base class for all entity components. 
     *
     * A component might represet a single piece of logic or renderable instanced for the entity it 
     * is attached to. 
     * */
    class EntityComponent
    {
    public:
        EntityComponent();
        virtual ~EntityComponent();

        /** set the parent component */
        void SetParent(const std::shared_ptr<EntityComponent> & parent);
        /** Get the parent component for this component */
        std::weak_ptr<EntityComponent> GetParent() const;

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

    protected:
        /** Reference to this components parent, null if is top-level component */
        std::weak_ptr<EntityComponent> m_parent;

        /** Child components, any component can be a child of another component for the greatest flexibility. */
        std::map<unsigned int, std::shared_ptr<EntityComponent> > m_components;

        /** Has updated flag */
        bool m_dirty;
    };

    /**
     * \brief A component base class used to represent a geometrical object in the scene.
     *
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

        bool IsDirty() const;

        const Vector3 & GetPosition() const;
        const Vector3 & GetScale() const;
        const Quaternion & GetRotation() const;
        Matrix GetTransform() const;

        void SetPosition(const Vector3 & position);
        void SetScale(const Vector3 & scale);
        void SetRotation(const Quaternion & rotation);

        // Light getters
        bool EmitsLight() const;
        Vector4 GetColor() const;

    protected:
        /** Helper function for retrieving x, y, or z vars from script tables */
        void GetTableVarValue(std::shared_ptr<LuaTable> table, const std::string axis, float * const out);

        /** Update the transform matrix */
        void UpdateTransform();

    private:
        /** This components relative position in relation to its parent. */
        Vector3 m_vPosition;
        /** This components relative scale in relation to its parent. */
        Vector3 m_vScale;
        /** This components relative rotation in relation to its parent. */
        Quaternion m_qRotation;

        /** This components combined rotation, scale, and position. */
        Matrix m_mTransform;

        /** Does the visible object in the scene absorb or emit light. */
        bool m_bLightEmitter;

        /** The objects default, unskinned, color.  Which effects how it emits light. */
        Vector4 m_vColor;
    };
}

#endif // ENTITY_COMPONENT_H
