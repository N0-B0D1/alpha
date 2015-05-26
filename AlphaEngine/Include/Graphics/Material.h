#ifndef ALPHA_MATERIAL_H
#define ALPHA_MATERIAL_H

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

#include "Math/Vector4.h"

namespace alpha
{
    class Material
    {
    public:
        Material();
        virtual ~Material();

        const Vector4 & GetAmbient() const;
        const Vector4 & GetDiffuse() const;
        const Vector4 & GetSpecular() const;
        float GetShininess() const;

    private:
        Vector4 m_ambient;
        Vector4 m_diffuse;
        Vector4 m_specular;
        float m_shininess;
    };
}

#endif // ALPHA_MATERIAL_H