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

#include "Entities/MeshComponent.h"
#include "Scripting/LuaVar.h"

namespace alpha
{
    const std::string MeshComponent::sk_name = "mesh";

    MeshComponent::~MeshComponent() { }

    //! Provides logic for how to initialize a transform component from Lua script data
    void MeshComponent::VInitialize(std::shared_ptr<LuaVar> var)
    {
        std::shared_ptr<LuaTable> table = std::dynamic_pointer_cast<LuaTable>(var);

        // load model asset path
        // model asset will be loaded in the graphics system
        std::shared_ptr<LuaVar> modelVar = table->Get("model");
        if (modelVar->GetVarType() == VT_STATIC)
        {
            auto model = std::dynamic_pointer_cast<LuaStatic<std::string>>(modelVar);
            m_sModelPath = model->GetValue();
        }

        // init base scene component
        std::shared_ptr<LuaVar> transform = table->Get("transform");
        if (transform != nullptr)
        {
            SceneComponent::VInitialize(transform);
        }
    }

    bool MeshComponent::VUpdate(float /*fCurrentTime*/, float /*fElapsedTime*/)
    {
        return true;
    }

    std::string MeshComponent::VGetName() const
    {
        return MeshComponent::sk_name;
    }
}