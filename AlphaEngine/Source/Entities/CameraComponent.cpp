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

#include "Entities/CameraComponent.h"
#include "Scripting/LuaVar.h"
#include "Graphics/Camera.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

namespace alpha
{
    const std::string CameraComponent::sk_name = "camera";

    CameraComponent::~CameraComponent() { }

    //! Provides logic for how to initialize a transform component from Lua script data
    void CameraComponent::VInitialize(std::shared_ptr<LuaVar> var)
    {
        std::shared_ptr<LuaTable> table = std::dynamic_pointer_cast<LuaTable>(var);

        m_fov = this->ParseFloatVar(table->Get("fov"), 45.f);
        m_near = this->ParseFloatVar(table->Get("near"), 0.1f);
        m_far = this->ParseFloatVar(table->Get("far"), 100.f);

        std::shared_ptr<LuaVar> transform = table->Get("transform");
        if (transform != nullptr)
        {
            SceneComponent::VInitialize(transform);
        }
    }

    bool CameraComponent::VUpdate(float /*fCurrentTime*/, float /*fElapsedTime*/)
    {
        return true;
    }

    std::string CameraComponent::VGetName() const
    {
        return CameraComponent::sk_name;
    }

    float CameraComponent::ParseFloatVar(std::shared_ptr<LuaVar> var, float defaultValue)
    {
        if (var != nullptr)
        {
            std::shared_ptr<LuaStatic<double>> fVar = std::dynamic_pointer_cast<LuaStatic<double>>(var);
            // make sure cast succeeded
            if (fVar)
            {
                return static_cast<float>(fVar->GetValue());
            }
        }

        // return default value
        return defaultValue;
    }
}
