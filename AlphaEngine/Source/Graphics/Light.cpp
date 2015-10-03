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
        m_fMaxDistance = light_component->GetLightDistance();

        CalculateLighting();
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

    void Light::CalculateLighting()
    {
        m_vDiffuse = m_vBaseColor * m_fIntensity;
        m_vAmbient = m_vDiffuse * m_fAmbientIntensity;
        m_vSpecular = Vector4(1.f, 1.f, 1.f, 1.f);

        /**
         *                   I
         * Atten. = ---------------------
         *           C + L * D + Q * D^2
         *
         *
         * C - Constant coeficient
         *     For our purposes this is always equal to 1.
         *
         *
         * L - Linear coeficient
         *     When Light Radius = 100, L = 0.045f
         *     When Light Radius = 200, L = 0.022f
         *     
         *     L = X / LR
         *     0.045 * 100 = 4.5
         *     L = 4.5 / LR = 4.5 / 7 = 0.642857
         *     L = 4.5 / LR = 4.5 / 200 = 0.0225f
         *     L = 4.5 / LR = 4.5 / 50 = 0.09f
         *
         *     L = 4.5f / LR;
         *
         *
         * Q - Quadratic coeficient
         *     When Light Radius = 100, Q = 0.0075f
         *     When Light Radius = 200, Q = 0.0019f
         *
         *     Q = X / LR^2
         *     0.0075 * 10,000 = 75
         *     Q = 75 / 7^2   = 1.53
         *     Q = 75 / 50^2  = 0.03
         *     Q = 75 / 200^2 = 0.001875
         *     Q = 75 / 3250^2 = 75 / 10562500 =
         *
         * These calculations are most likely not completely accurate, but
         * they are pretty dang close and will will be close enough to
         * meet our needs to calculate the Constant, Linear, and Quadratic
         * constants for the attenuation formula.
         */
        m_fConstant = 1.f;
        m_fLinear = 4.5f / m_fMaxDistance;
        m_fQuadratic = 75.f / (m_fMaxDistance * m_fMaxDistance);
    }

    Vector3 Light::GetLightDirection() const
    {
        return m_vDirection;
    }

    LightType Light::GetLightType() const
    {
        return m_eLightType;
    }

    float Light::GetAttenuationConstant() const
    {
        return m_fConstant;
    }

    float Light::GetAttenuationLinear() const
    {
        return m_fLinear;
    }

    float Light::GetAttenuationQuadratic() const
    {
        return m_fQuadratic;
    }

    float Light::GetMaxDistance() const
    {
        return m_fMaxDistance;
    }
}
