#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

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

#include "Entities/EntityComponent.h"
#include "Math/Vector3.h"

namespace alpha
{
    class TransformComponent : public EntityComponent
    {
    public:
        static const std::string sk_name;

    public:
        TransformComponent();
        virtual ~TransformComponent();

        virtual void VInitialize(std::shared_ptr<LuaVar> var);
        virtual bool VUpdate(float fCurrentTime, float fElapsedTime);
        virtual std::string VGetName() const;

        //! Transform position
        Vector3 m_position;
    };
}

#endif // TRANSFORM_COMPONENT_H
