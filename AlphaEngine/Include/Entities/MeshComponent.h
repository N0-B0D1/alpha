#ifndef ALPHA_MESH_COMPONENT_H
#define ALPHA_MESH_COMPONENT_H

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

#include "Entities/EntityComponent.h"

namespace alpha
{
    class MeshComponent : public SceneComponent
    {
    public:
        static const std::string sk_name;

        virtual ~MeshComponent();

        virtual void VInitialize(std::shared_ptr<LuaVar> var);
        virtual bool VUpdate(float fCurrentTime, float fElapsedTime);
        virtual std::string VGetName() const;

        std::string GetMeshPath() const;

    private:
        std::string m_sModelPath;
    };
}

#endif // ALPHA_MESH_COMPONENT_H
