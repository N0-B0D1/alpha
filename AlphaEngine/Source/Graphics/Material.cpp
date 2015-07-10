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

#include <memory>

#include "Graphics/Material.h"
#include "Assets/Asset.h"
#include "Scripting/LuaVar.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    Material::Material()
        : m_pMaterialScript(nullptr)
        , m_ambient(0.0215f, 0.1745f, 0.0215f, 0.f)
        , m_diffuse(0.07568f, 0.61424f, 0.07568f, 0.f)
        , m_specular(0.633f, 0.727811f, 0.633f, 0.f)
        , m_shininess(0.6f * 128.f)
    { }
    Material::Material(std::shared_ptr<Asset> pAsset)
        : m_pMaterialScript(nullptr)
        , m_vColor(1.f, 0.2f, 1.f, 0.f)
        , m_fRoughness(0.5f)
        , m_fMetallic(0.0f)
        , m_fSpecular(0.5f)
    {
        if (pAsset != nullptr)
        {
            m_pMaterialScript = new MaterialScript(pAsset);

            auto table = m_pMaterialScript->GetMaterialTable();

            if (table == nullptr)
            {
                LOG_ERR("Material script variable data does not represent a valid data table.");
                return;
            }
            else
            {
                // get object color value
                auto color_table = std::dynamic_pointer_cast<LuaTable>(table->Get("color"));
                if (color_table != nullptr)
                {
                    m_pMaterialScript->GetTableFloatValue(color_table, "r", &m_vColor.x);
                    m_pMaterialScript->GetTableFloatValue(color_table, "g", &m_vColor.y);
                    m_pMaterialScript->GetTableFloatValue(color_table, "b", &m_vColor.z);
                    m_pMaterialScript->GetTableFloatValue(color_table, "a", &m_vColor.w);
                }

                // get object roughness
                m_pMaterialScript->GetTableFloatValue(table, "roughness", &m_fRoughness);
                // get object metallic factor
                m_pMaterialScript->GetTableFloatValue(table, "metallic", &m_fMetallic);
                // get object spcular
                m_pMaterialScript->GetTableFloatValue(table, "specular", &m_fSpecular);
            }
        }

        m_diffuse = m_vColor * m_fRoughness;
        m_ambient = m_diffuse;
        m_specular = m_vColor * m_fSpecular;
        m_shininess = 32.0f;
    }
    Material::Material(const Vector4 & color, float roughness, float metallic, float specular)
        : m_pMaterialScript(nullptr)
        , m_vColor(color)
        , m_fRoughness(roughness)
        , m_fMetallic(metallic)
        , m_fSpecular(specular)
    {
        m_ambient = m_vColor;
        m_diffuse = m_vColor * roughness;
        m_specular = m_vColor * specular;
        m_shininess = 32.0f;
    }
    Material::Material(const Vector4 & ambient, const Vector4 & diffuse, const Vector4 & specular, float shininess)
        : m_pMaterialScript(nullptr)
        , m_ambient(ambient)
        , m_diffuse(diffuse)
        , m_specular(specular)
        , m_shininess(shininess)
    { }
    Material::~Material()
    {
        if (m_pMaterialScript) { delete m_pMaterialScript; }
    }

    const Vector4 & Material::GetColor() const
    {
        return m_vColor;
    }

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

    float Material::GetSpecularCoefficient() const
    {
        return m_fSpecular;
    }




    MaterialScript::MaterialScript(std::shared_ptr<Asset> asset)
        : m_pMaterialTable(nullptr)
    {
        this->Add(asset);
    }
    MaterialScript::~MaterialScript() { }

    //! Get a list of the components specified by the script.
    std::shared_ptr<LuaTable> MaterialScript::GetMaterialTable()
    {
        if (m_pMaterialTable == nullptr)
        {
            this->Load();
            this->Run();

            m_pMaterialTable = this->GetGlobalTable("material");
        }
        return m_pMaterialTable;
    }
}