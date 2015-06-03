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

#include <functional>
#include <string>

#include "Entities/EntityComponent.h"

#include "Scripting/LuaVar.h"
#include "Math/Vector3.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    EntityComponent::EntityComponent()
        : m_dirty(false)
    { }
    EntityComponent::~EntityComponent() { }

    void EntityComponent::SetParent(const std::shared_ptr<EntityComponent> & parent)
    {
        m_parent = parent;
    }
    std::weak_ptr<EntityComponent> EntityComponent::GetParent() const
    {
        return m_parent;
    }

    void EntityComponent::Attach(unsigned int component_id, std::shared_ptr<EntityComponent> component)
    {
        auto it = m_components.find(component_id);
        if (it == m_components.end())
        {
            m_components[component_id] = component;
            return;
        }
        LOG_WARN("  <EntityComponent> Attempt to add a component type that already exists: type: ", component->VGetName());
    }

    const std::map<unsigned int, std::shared_ptr<EntityComponent> > EntityComponent::GetComponents() const
    {
        return m_components;
    }

    unsigned int EntityComponent::GetID() const
    {
        return GetIDFromName(this->VGetName());
    }

    unsigned int EntityComponent::GetIDFromName(const std::string & name)
    {
        static std::hash<std::string> string_hash;
        return string_hash(name);
    }

    SceneComponent::~SceneComponent() { }

    void SceneComponent::VInitialize(std::shared_ptr<LuaVar> var)
    {
        // var should represent a LUA table containing some of the following variables:
        // 1. transform
        // 2. light_emitter
        // 3. light_color

        if (var->GetVarType() != VT_TABLE)
        {
            LOG_ERR("Script variable data does not represent a valid data table.");
            return;
        }

        std::shared_ptr<LuaTable> data_table = std::dynamic_pointer_cast<LuaTable>(var);

        // get and handle transform
        std::shared_ptr<LuaTable> transform_table = std::dynamic_pointer_cast<LuaTable>(data_table->Get("transform"));
        if (transform_table != nullptr)
        {
            // get the position, scale, and rotation tables.
            std::shared_ptr<LuaTable> position = std::dynamic_pointer_cast<LuaTable>(transform_table->Get("position"));
            std::shared_ptr<LuaTable> rotation = std::dynamic_pointer_cast<LuaTable>(transform_table->Get("rotation"));
            std::shared_ptr<LuaTable> scale = std::dynamic_pointer_cast<LuaTable>(transform_table->Get("scale"));

            // get x, y, z values for each position, scale, and rotation.
            this->GetTableVarValue(position, "x", &m_vPosition.x);
            this->GetTableVarValue(position, "y", &m_vPosition.y);
            this->GetTableVarValue(position, "z", &m_vPosition.z);

            this->GetTableVarValue(scale, "x", &m_vScale.x);
            this->GetTableVarValue(scale, "y", &m_vScale.y);
            this->GetTableVarValue(scale, "z", &m_vScale.z);

            // TODO - read rotation value and convert to quaternion

            this->UpdateTransform();
        }

        if (auto material_var = std::dynamic_pointer_cast<LuaStatic<std::string>>(data_table->Get("material")))
        {
            m_sMaterial = material_var->GetValue();
        }
    }

    bool SceneComponent::IsDirty() const
    {
        return m_dirty;
    }

    const Vector3 & SceneComponent::GetPosition() const
    {
        return m_vPosition;
    }

    const Vector3 & SceneComponent::GetScale() const
    {
        return m_vScale;
    }

    const Quaternion & SceneComponent::GetRotation() const
    {
        return m_qRotation;
    }

    Matrix SceneComponent::GetTransform() const
    {
        return m_mTransform;
    }

    std::string SceneComponent::GetMaterialPath() const
    {
        return m_sMaterial;
    }

    void SceneComponent::SetPosition(const Vector3 & position)
    {
        m_vPosition.x = position.x;
        m_vPosition.y = position.y;
        m_vPosition.z = position.z;

        this->UpdateTransform();
    }
    void SceneComponent::SetScale(const Vector3 & scale)
    {
        m_vScale.x = scale.x;
        m_vScale.y = scale.y;
        m_vScale.z = scale.z;

        this->UpdateTransform();
    }

    void SceneComponent::SetRotation(const Quaternion & rotation)
    {
        m_qRotation.x = rotation.x;
        m_qRotation.y = rotation.y;
        m_qRotation.z = rotation.z;
        m_qRotation.w = rotation.w;

        this->UpdateTransform();
    }

    void SceneComponent::GetTableVarValue(std::shared_ptr<LuaTable> table, const std::string key, float * const out)
    {
        std::shared_ptr<LuaStatic<double> > var = std::dynamic_pointer_cast<LuaStatic<double>>(table->Get(key));
        if (var != nullptr && out != nullptr)
        {
            *out = static_cast<float>(var->GetValue());
        }
    }

    void SceneComponent::UpdateTransform()
    {
        // star with identity matrix
        this->m_mTransform = Matrix();

        // then apply vector transforms
        // always apply R -> S -> T
        this->m_mTransform = this->m_mTransform * Matrix::Rotate(this->m_qRotation);
        this->m_mTransform = this->m_mTransform * Matrix::Scale(this->m_vScale);
        this->m_mTransform = this->m_mTransform * Matrix::Translate(this->m_vPosition);
    }
}
