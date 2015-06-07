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

#include "Graphics/Light.h"
#include "Entities/LightComponent.h"

namespace alpha
{
    Light::Light(std::shared_ptr<LightComponent> light_component)
    {
        m_eLightType = light_component->GetLightType();
        m_vBaseColor = light_component->GetLightColor();
        m_fIntensity = light_component->GetIntensity();
        m_fAmbientIntensity = light_component->GetAmbientIntensity();
        m_vDirection = light_component->GetLightDirection();

        GenerateMaterial();
    }

    Light::~Light() { }

    Vector4 Light::GetAmbientLight() const
    {
        return m_vAmbient;
    }

    Vector4 Light::GetDiffuseLight() const
    {
        return m_vDiffuse;
    }

    Vector4 Light::GetSpecularLight() const
    {
        return m_vSpecular;
    }

    void Light::GenerateMaterial()
    {
        m_vDiffuse = m_vBaseColor * m_fIntensity;
        m_vAmbient = m_vDiffuse * m_fAmbientIntensity;
        m_vSpecular = Vector4(1.f, 1.f, 1.f, 1.f);
    }

    Vector3 Light::GetLightDirection() const
    {
        return m_vDirection;
    }

    LightType Light::GetLightType() const
    {
        return m_eLightType;
    }
}
