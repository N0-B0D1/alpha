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

#include <map>

#include "Math/Vector4.h"
#include "Scripting/LuaScript.h"

namespace alpha
{
    class Asset;
    class MaterialScript;

    class Material
    {
    public:
        Material();
        explicit Material(std::shared_ptr<Asset> pAsset);
        Material(const Vector4 & color, float roughness, float metallic, float specular);
        Material(const Vector4 & ambient, const Vector4 & diffuse, const Vector4 & specular, float shininess);
        virtual ~Material();

        const Vector4 & GetColor() const;

        const Vector4 & GetAmbient() const;
        const Vector4 & GetDiffuse() const;
        const Vector4 & GetSpecular() const;
        float GetShininess() const;

    private:
        MaterialScript * m_pMaterialScript;

        Vector4 m_ambient;
        Vector4 m_diffuse;
        Vector4 m_specular;
        float m_shininess;

        Vector4 m_vColor;
        float m_fRoughness;
        float m_fMetallic;
        float m_fSpecular;
    };

    class MaterialScript : public LuaScript
    {
    public:
        explicit MaterialScript(std::shared_ptr<Asset> asset);
        virtual ~MaterialScript();

        std::shared_ptr<LuaTable> GetMaterialTable();

    private:
        std::shared_ptr<LuaTable> m_pMaterialTable;
    };
}

#endif // ALPHA_MATERIAL_H