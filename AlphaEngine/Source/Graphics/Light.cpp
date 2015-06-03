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

namespace alpha
{
    Light::Light()
        : m_vBaseColor(1.f, 1.f, 1.f, 1.f)
        , m_fIntensity(0.5f)
        , m_fAmbientIntensity(0.2f)
    {
        GenerateMaterial();
    }
    Light::Light(Vector4 color, float intensity, float ambient_intensity)
        : m_vBaseColor(color)
        , m_fIntensity(intensity)
        , m_fAmbientIntensity(ambient_intensity)
    {
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

    LightType Light::GetLightType() const
    {
        return m_eLightType;
    }
}
