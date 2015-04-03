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

#include "Entities/LightComponent.h"
#include "Scripting/LuaVar.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    const std::string LightComponent::sk_name = "light";

    LightComponent::~LightComponent() { }

    //! Provides logic for how to initialize a transform component from Lua script data
    void LightComponent::VInitialize(std::shared_ptr<LuaVar> var)
    {
        std::shared_ptr<LuaTable> table = std::dynamic_pointer_cast<LuaTable>(var);

        // load rgba color value
        auto color = std::dynamic_pointer_cast<LuaTable>(table->Get("color"));
        if (color != nullptr)
        {
            this->GetTableVarValue(color, "r", &m_color.x);
            this->GetTableVarValue(color, "g", &m_color.y);
            this->GetTableVarValue(color, "b", &m_color.z);
            this->GetTableVarValue(color, "a", &m_color.w);
        }

        // init base scene component
        std::shared_ptr<LuaVar> transform = table->Get("transform");
        if (transform != nullptr)
        {
            SceneComponent::VInitialize(transform);
        }
    }

    bool LightComponent::VUpdate(float /*fCurrentTime*/, float /*fElapsedTime*/)
    {
        return true;
    }

    std::string LightComponent::VGetName() const
    {
        return LightComponent::sk_name;
    }

    Vector4 LightComponent::GetColor() const
    {
        return m_color;
    }

    void LightComponent::SetColor(const Vector4 & color)
    {
        m_color.x = color.x;
        m_color.y = color.y;
        m_color.z = color.z;
        m_color.w = color.w;
    }
}
