#ifndef ALPHA_LIGHT_H
#define ALPHA_LIGHT_H

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

#include "Math/Matrix.h"
#include "Math/Vector4.h"
#include "Graphics/Material.h"

namespace alpha
{
    /**
     * Lights are Models that are rendered in the scene
     * but also effect other renderables in the scene by emitting light.
     */
    class Light
    {
    public:
        Light();
        explicit Light(Vector4 color, float intensity, float ambient_intensity);
        virtual ~Light();

        Vector4 GetAmbientLight() const;
        Vector4 GetDiffuseLight() const;
        Vector4 GetSpecularLight() const;

        Matrix worldTransform;

    private:
        /** calculate material settings based on color an dintensity of light */
        void GenerateMaterial();

        Vector4 m_color;
        float m_fIntensity;
        float m_fAmbientIntensity;

        Material m_material;
    };
}

#endif // ALPHA_LIGHT_H