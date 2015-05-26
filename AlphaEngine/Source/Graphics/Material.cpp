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

#include "Graphics/Material.h"

namespace alpha
{
    Material::Material()
        : m_ambient(0.0215f, 0.1745f, 0.0215f, 0.f)
        , m_diffuse(0.07568f, 0.61424f, 0.07568f, 0.f)
        , m_specular(0.633f, 0.727811f, 0.633f, 0.f)
        , m_shininess(0.6f * 128.f)
    { }
    Material::~Material() { }

    const Vector4 & Material::GetAmbient() const
    {
        return m_ambient;
    }

    const Vector4 & Material::GetDiffuse() const
    {
        return m_diffuse;
    }

    const Vector4 & Material::GetSpecular() const
    {
        return m_specular;
    }

    float Material::GetShininess() const
    {
        return m_shininess;
    }
}