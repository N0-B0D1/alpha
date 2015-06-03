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
#include "Graphics/Material.h"

namespace alpha
{
    const std::string LightComponent::sk_name = "light";

    LightComponent::~LightComponent() { }

    //! Provides logic for how to initialize a transform component from Lua script data
    void LightComponent::VInitialize(std::shared_ptr<LuaVar> var)
    {
        std::shared_ptr<LuaTable> table = std::dynamic_pointer_cast<LuaTable>(var);

        // get direct light intensity
        std::shared_ptr<LuaVar> intensity_var = table->Get("intensity");
        if (intensity_var->GetVarType() == VT_STATIC)
        {
            auto intensity = std::dynamic_pointer_cast<LuaStatic<double>>(intensity_var);
            m_fIntensity = static_cast<float>(intensity->GetValue());
        }

        // get indirect ambient intensity
        std::shared_ptr<LuaVar> ambient_var = table->Get("ambient_intensity");
        if (ambient_var->GetVarType() == VT_STATIC)
        {
            auto ambient = std::dynamic_pointer_cast<LuaStatic<double>>(ambient_var);
            m_fAmbientIntensity = static_cast<float>(ambient->GetValue());
        }

        // get the type of light this component represents
        if (auto light_type_var = std::dynamic_pointer_cast<LuaStatic<std::string>>(table->Get("light_type")))
        {
            std::string light_type = light_type_var->GetValue();
            
            if (light_type == "directional")
            {
                m_eLightType = LightType::DIRECTIONAL;
            }
            else if (light_type == "point")
            {
                m_eLightType = LightType::POINT;
            }
            else
            {
                m_eLightType = LightType::DIRECTIONAL;
            }
        }

        // init base scene component
        SceneComponent::VInitialize(var);
    }

    bool LightComponent::VUpdate(float /*fCurrentTime*/, float /*fElapsedTime*/)
    {
        return true;
    }

    std::string LightComponent::VGetName() const
    {
        return LightComponent::sk_name;
    }

    float LightComponent::GetIntensity() const
    {
        return m_fIntensity;
    }

    float LightComponent::GetAmbientIntensity() const
    {
        return m_fAmbientIntensity;
    }

    LightType LightComponent::GetLightType() const
    {
        return m_eLightType;
    }
}
