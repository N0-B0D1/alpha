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

#include <string>

#include <GL/glew.h>

#include "Graphics/LightingPass.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Renderable.h"
#include "Graphics/Light.h"
#include "Graphics/Camera.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    LightingPass::LightingPass()
        : m_PLShaderProgram(0)
        , m_DLShaderProgram(0)
    { }
    LightingPass::~LightingPass() { }

    bool LightingPass::VInitialize(AssetSystem * const pAssetSystem, int /*windowWidth*/, int /*windowHeight*/)
    {
        // load sphere model for point light volumes
        m_pSphere = pAssetSystem->GetAsset("Models/unitsphere.am");

        // load shader asset files
        m_vsShader = pAssetSystem->GetAsset("Shaders/gl_lighting_vs.glsl");
        m_psPLShader = pAssetSystem->GetAsset("Shaders/gl_pointlight_ps.glsl");
        m_psDLShader = pAssetSystem->GetAsset("Shaders/gl_dirlight_ps.glsl");

        m_PLShaderProgram = CreateShaderProgram(m_vsShader, m_psPLShader);
        m_DLShaderProgram = CreateShaderProgram(m_vsShader, m_psDLShader);

        // set texture samplers
        glUseProgram(m_PLShaderProgram);
        glUniform1i(glGetUniformLocation(m_PLShaderProgram, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(m_PLShaderProgram, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(m_PLShaderProgram, "gAlbedoSpec"), 2);
        glUseProgram(m_DLShaderProgram);
        glUniform1i(glGetUniformLocation(m_DLShaderProgram, "gPosition"), 0);
        glUniform1i(glGetUniformLocation(m_DLShaderProgram, "gNormal"), 1);
        glUniform1i(glGetUniformLocation(m_DLShaderProgram, "gAlbedoSpec"), 2);

        // Create the quad that will be rendered to the screen, and have the gbuffer textures dawn on it
        GLfloat vertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &m_vaoQuad);
        glGenBuffers(1, &m_vboQuad);
        glBindVertexArray(m_vaoQuad);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboQuad);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

        return true;
    }

    bool LightingPass::VShutdown()
    {
        return true;
    }

    void LightingPass::VRender(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> /*render_sets*/, std::vector<Light *> lights)
    {
        CreateLightBufferData(lights);

        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->RenderPointLights(pCamera);
        this->RenderDirectionalLights();
    }

    void LightingPass::AttachGBufferTexture(GBUFFER_TYPE texture_type, GLuint texture)
    {
        m_gBufferTextures[texture_type] = texture;
    }

    void LightingPass::CreateLightBufferData(const std::vector<Light *> & lights)
    {
        m_pointLights.clear();
        m_directionalLights.clear();

        for (auto light : lights)
        {
            unsigned int dindex = m_directionalLights.size();
            unsigned int pindex = m_pointLights.size();

            switch(light->GetLightType())
            {
            case LightType::DIRECTIONAL:
                m_directionalLights.push_back(DirectionalLight());

                m_directionalLights[dindex].direction = Vector4(light->GetLightDirection(), 1.f);

                m_directionalLights[dindex].ambient = light->GetAmbientLight();
                m_directionalLights[dindex].diffuse = light->GetDiffuseLight();
                m_directionalLights[dindex].specular = light->GetSpecularLight();
                break;

            case LightType::POINT:
                m_pointLights.push_back(PointLight());

                //LOG_ERR(light->GetAttenuationConstant(), ", ", light->GetAttenuationLinear(), ", ", light->GetAttenuationQuadratic());

                m_pointLights[pindex].position = Vector4(light->worldTransform.Position(), 1.f);

                m_pointLights[pindex].ambient = light->GetAmbientLight();
                m_pointLights[pindex].diffuse = light->GetDiffuseLight();
                m_pointLights[pindex].specular = light->GetSpecularLight();

                m_pointLights[pindex].constant = light->GetAttenuationConstant();
                m_pointLights[pindex].linear = light->GetAttenuationLinear();
                m_pointLights[pindex].quadratic = light->GetAttenuationQuadratic();
                break;

            default:
                LOG_WARN("Unknown light type, unable to render: ", light->GetLightType());
                break;
            }
        }

        // XXX hack
        // make sure at least 2 point lights exist, and 1 directional light
        /*
        switch (m_directionalLights.size())
        {
        case 0:
            m_directionalLights.push_back(DirectionalLight());
            break;
        }

        switch (m_pointLights.size())
        {
        case 1:
            m_pointLights.push_back(PointLight());
            
            m_pointLights[m_pointLights.size() - 1].constant = 1.f;
            m_pointLights[m_pointLights.size() - 1].linear = 0.045f;
            m_pointLights[m_pointLights.size() - 1].quadratic = 0.0075f;

            break;

        case 0:
            for (int i = 0; i < 2; i++)
            {
                m_pointLights.push_back(PointLight());
            }
            break;
        }
        */
    }

    void LightingPass::RenderPointLights(std::shared_ptr<Camera> pCamera)
    {
        // set shader program
        glUseProgram(m_PLShaderProgram);

        // bind gbuffer textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBUFFER_POSITION]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBUFFER_NORMAL]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBUFFER_ALBEDOSPEC]);

        // add camera view position
        Vector3 view_pos = pCamera->GetView().Position();
        glUniform3f(glGetUniformLocation(m_PLShaderProgram, "viewPosition"), view_pos.x, view_pos.y, view_pos.z);

        Vector4 pos;
        Vector4 color;
        
        for (unsigned int i = 0; i < m_pointLights.size(); ++i)
        {
            // add point light data
            pos = m_pointLights[i].position;
            color = m_pointLights[i].diffuse;

            glUniform3f(glGetUniformLocation(m_PLShaderProgram, "pointLight.position"), pos.x, pos.y, pos.z);
            glUniform3f(glGetUniformLocation(m_PLShaderProgram, "pointLight.diffuse"), color.x, color.y, color.z);
            glUniform1f(glGetUniformLocation(m_PLShaderProgram, "pointLight.constant"), m_pointLights[i].constant);
            glUniform1f(glGetUniformLocation(m_PLShaderProgram, "pointLight.linear"), m_pointLights[i].linear);
            glUniform1f(glGetUniformLocation(m_PLShaderProgram, "pointLight.quadratic"), m_pointLights[i].quadratic);
        }
    }

    void LightingPass::RenderDirectionalLights()
    {
        // set shader program
        glUseProgram(m_DLShaderProgram);

        // bind gbuffer textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBUFFER_POSITION]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBUFFER_NORMAL]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[GBUFFER_ALBEDOSPEC]);

        for (unsigned int i = 0; i < m_directionalLights.size(); ++i)
        {
            // set single directional light data
            Vector4 pos = m_directionalLights[i].direction;
            Vector4 color = m_directionalLights[i].diffuse;
            Vector4 ambient = m_directionalLights[i].ambient;
            Vector4 specular = m_directionalLights[i].specular;

            glUniform3f(glGetUniformLocation(m_DLShaderProgram, "directionalLight.position"), pos.x, pos.y, pos.z);
            glUniform3f(glGetUniformLocation(m_DLShaderProgram, "directionalLight.ambient"), ambient.x, ambient.y, ambient.z);
            glUniform3f(glGetUniformLocation(m_DLShaderProgram, "directionalLight.diffuse"), color.x, color.y, color.z);
            glUniform3f(glGetUniformLocation(m_DLShaderProgram, "directionalLight.specular"), specular.x, specular.y, specular.z);

            // draw the quad for draw textures to
            glBindVertexArray(m_vaoQuad);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
    }
}
