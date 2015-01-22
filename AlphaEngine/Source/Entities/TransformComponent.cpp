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

#include "Entities/TransformComponent.h"

#include "Scripting/LuaVar.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    const std::string TransformComponent::sk_name = "transform";

    TransformComponent::TransformComponent()
        : m_position(0.0f, 0.0f, 0.0f)
    { }
    TransformComponent::~TransformComponent() { }

    //! Provides logic for how to initialize a transform component from Lua script data
    void TransformComponent::VInitialize(std::shared_ptr<LuaVar> var)
    {
        if (var->GetVarType() != VT_TABLE)
        {
            LOG_ERR("Script variable data does not represent a valid transform component.");
            return;
        }

        std::shared_ptr<LuaTable> table = std::dynamic_pointer_cast<LuaTable>(var);

        auto varx = std::dynamic_pointer_cast<LuaStatic<double> >(table->Get("x"));
        auto vary = std::dynamic_pointer_cast<LuaStatic<double> >(table->Get("y"));
        auto varz = std::dynamic_pointer_cast<LuaStatic<double> >(table->Get("z"));

        this->m_position.x = varx->GetValue();
        this->m_position.y = vary->GetValue();
        this->m_position.z = varz->GetValue();

        LOG(" .... test position.x = (", this->m_position.x, ", ", this->m_position.y, ", ", this->m_position.z, ")");
    }

    bool TransformComponent::VUpdate(float /*fCurrentTime*/, float /*fElapsedTime*/)
    {
        return true;
    }

    std::string TransformComponent::VGetName() const
    {
        return TransformComponent::sk_name;
    }
}
