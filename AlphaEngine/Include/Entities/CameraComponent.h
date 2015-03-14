#ifndef ALPHA_CAMERA_COMPONENT_H
#define ALPHA_CAMERA_COMPONENT_H

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
    struct Vector3;
    struct Quaternion;

    /**
     * The CameraComponent is a special type of entity component that has a position
     * and a description of how it should view the world via a fov, near, and far plane values.
     */
    class CameraComponent : public SceneComponent
    {
    public:
        static const std::string sk_name;

        virtual ~CameraComponent();

        virtual void VInitialize(std::shared_ptr<LuaVar> var);
        virtual bool VUpdate(float fCurrentTime, float fElapsedTime);
        virtual std::string VGetName() const;

        float GetFov() const;
        float GetNear() const;
        float GetFar() const;

    private:
        /** Init helper methods */
        float ParseFloatVar(std::shared_ptr<LuaVar> var, float defaultValue);

        /** Camera field of view */
        float m_fov;
        /** Camera near plane position */
        float m_near;
        /** Camera far plane position */
        float m_far;
    };
}

#endif // ALPHA_CAMERA_COMPONENT_H
