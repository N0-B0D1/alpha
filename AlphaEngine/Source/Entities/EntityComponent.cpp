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

#include <functional>
#include <string>

#include "Entities/EntityComponent.h"

#include "Scripting/LuaVar.h"
#include "Math/Transform.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    EntityComponent::~EntityComponent() { }

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
        // var should represent the transform table from the SceneComponent
        // not the top level scene component.

        if (var->GetVarType() != VT_TABLE)
        {
            LOG_ERR("Script variable data does not represent a valid transform table.");
            return;
        }

        std::shared_ptr<LuaTable> table = std::dynamic_pointer_cast<LuaTable>(var);

        // get the position, scale, and rotation tables.
        std::shared_ptr<LuaTable> position = std::dynamic_pointer_cast<LuaTable>(table->Get("position"));
        std::shared_ptr<LuaTable> rotation = std::dynamic_pointer_cast<LuaTable>(table->Get("rotation"));
        std::shared_ptr<LuaTable> scale = std::dynamic_pointer_cast<LuaTable>(table->Get("scale"));

        // get x, y, z values for each position, scale, and rotation.
        m_transform.position.x = this->GetAxis(position, "x");
        m_transform.position.y = this->GetAxis(position, "y");
        m_transform.position.z = this->GetAxis(position, "z");

        m_transform.scale.x = this->GetAxis(scale, "x");
        m_transform.scale.y = this->GetAxis(scale, "y");
        m_transform.scale.z = this->GetAxis(scale, "z");

        LOG(" .... test position = (", this->m_transform.position.x, ", ", this->m_transform.position.y, ", ", this->m_transform.position.z, ")");
        LOG(" .... test scale = (", this->m_transform.scale.x, ", ", this->m_transform.scale.y, ", ", this->m_transform.scale.z, ")");
    }

    float SceneComponent::GetAxis(std::shared_ptr<LuaTable> table, const std::string axis)
    {
        std::shared_ptr<LuaStatic<double> > var = std::dynamic_pointer_cast<LuaStatic<double>>(table->Get(axis));
        return static_cast<float>(var->GetValue());
    }
}
