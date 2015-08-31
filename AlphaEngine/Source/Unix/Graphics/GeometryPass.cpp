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

#include <GL/glew.h>

#include "Graphics/GeometryPass.h"
#include "Graphics/Camera.h"
#include "Graphics/Light.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Renderable.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    GeometryPass::GeometryPass()
        : m_shaderProgram(0)
        , m_gBuffer(0)
    {
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            m_gBufferTextures[i] = 0;
        }
    }
    GeometryPass::~GeometryPass() { }

    bool GeometryPass::VInitialize(AssetSystem * const pAssetSystem, int windowWidth, int windowHeight)
    {
        m_vsShader = pAssetSystem->GetAsset("Shaders/gl_deferred_vs.glsl");
        m_psShader = pAssetSystem->GetAsset("Shaders/gl_deferred_ps.glsl");

        m_shaderProgram = CreateShaderProgram(m_vsShader, m_psShader);

        // create gbuffer textures
        glGenFramebuffers(1, &m_gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBuffer);

        glGenTextures(GBUFFER_TEXTURE_COUNT, m_gBufferTextures);

        for (unsigned int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, windowWidth, windowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_gBufferTextures[i], 0);
        }

        GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        // create and attach depth buffer
        glGenRenderbuffers(1, &m_rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERR("GeometryPass - Framebuffer not complete!");
            return false;
        }
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        return true;
    }

    bool GeometryPass::VShutdown()
    {
        return true;
    }

    void GeometryPass::VRender(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> render_sets, std::vector<Light *> /*lights*/)
    {
        // bind gBuffer textures to be rendered to
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBuffer);

        // write to depth buffer
        glDepthMask(GL_TRUE);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // enable depth test, and disable blend before drawing deometry to
        // gbuffer
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        
        // set shader program
        glUseProgram(m_shaderProgram);

        // get/make view and projection matrix
        Matrix view = pCamera->GetView();
        Matrix proj = pCamera->GetProjection();

        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, &view.m_11);
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, &proj.m_11);

        for (auto rs : render_sets)
        {
            auto material = rs->material.lock();
            auto diffuse = material->GetDiffuse();
            auto specular = material->GetSpecularCoefficient();

            // attach object matrix values
            glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "world"), 1, GL_FALSE, &rs->worldTransform.m_11);

            // set object color information
            glUniform3f(glGetUniformLocation(m_shaderProgram, "diffuse"), diffuse.x, diffuse.y, diffuse.z);
            glUniform1f(glGetUniformLocation(m_shaderProgram, "specular"), specular);

            for (Renderable * renderable : rs->GetRenderables())
            {
                // bind vertices to array object
                glBindVertexArray(renderable->m_vertexAttribute);

                // draw the triangles
                glDrawElements(GL_TRIANGLES, renderable->indices.size(), GL_UNSIGNED_INT, (void*)0);

                // unbind array object
                glBindVertexArray(0);
            }
        }

        // un-bind gbuffer textures.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glDepthMask(GL_FALSE);

        glDisable(GL_DEPTH_TEST);
    }
    
    GLuint GeometryPass::GetGBufferTexture(GBUFFER_TYPE texture_type)
    {
        return m_gBufferTextures[texture_type];
    }
}
