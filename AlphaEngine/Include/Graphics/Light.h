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
    class LightComponent;

    typedef enum LightType
    {
        DIRECTIONAL,
        POINT,
    } LightType;

    /**
     * Lights are Models that are rendered in the scene
     * but also effect other renderables in the scene by emitting light.
     */
    class Light
    {
    public:
        explicit Light(std::shared_ptr<LightComponent> light_component);
        virtual ~Light();

        Vector4 GetAmbientLight() const;
        Vector4 GetDiffuseLight() const;
        Vector4 GetSpecularLight() const;
        Vector3 GetLightDirection() const;

        LightType GetLightType() const;

        float GetAttenuationConstant() const;
        float GetAttenuationLinear() const;
        float GetAttenuationQuadratic() const;

        Matrix worldTransform;

    private:
        /** calculate lighting based on color and intensity */
        void CalculateLighting();

        /** Base color emitted by the light */
        Vector4 m_vBaseColor;
        /** The intensity of the light on objects that it directly hits */
        float m_fIntensity;
        /**
         * The ambient intensity of the light on objects that it does not
         * directly hit.
         */
        float m_fAmbientIntensity;

        /** The calculated ambient color of the light. */
        Vector4 m_vAmbient;
        /** Calculated diffuse color of the light. */
        Vector4 m_vDiffuse;
        /** Calculated specular color of the light. */
        Vector4 m_vSpecular;

        /**
         * The type of the light, which determines how the light colors are
         * calculated and how the light effects objects in the scene.
         */
        LightType m_eLightType;

        /** The direction of a directional light. */
        Vector3 m_vDirection;

        /** point light calculated attenuation values */
        float m_fConstant;
        float m_fLinear;
        float m_fQuadratic;
    };
}

#endif // ALPHA_LIGHT_H
