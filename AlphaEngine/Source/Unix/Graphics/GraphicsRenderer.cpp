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

#include <math.h>

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "Graphics/GraphicsRenderer.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Renderable.h"
#include "Graphics/Camera.h"
#include "Graphics/Light.h"
#include "Math/Vector4.h"

#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    GraphicsRenderer::GraphicsRenderer() { }
    GraphicsRenderer::~GraphicsRenderer() { }

    bool GraphicsRenderer::Initialize(std::shared_ptr<AssetSystem> pAssets)
    {
        // prep shader assets
        m_vsDefaultShader = pAssets->GetAsset("Shaders/gl_vs_normal.glsl");
        m_psDefaultShader = pAssets->GetAsset("Shaders/gl_ps_normal.glsl");
        m_vsLightShader = pAssets->GetAsset("Shaders/gl_vs_light.glsl");
        m_psLightShader = pAssets->GetAsset("Shaders/gl_ps_light.glsl");

        LOG("GraphicsRenderer > Creating render window.");
		m_pWindow = new RenderWindow();
		if (!m_pWindow->Initialize())
		{
			return false;
		}

        this->InitializeDevice();

		return true;
	}

    bool GraphicsRenderer::Update(double currentTime, double elapsedTime)
    {
        return m_pWindow->Update(currentTime, elapsedTime);
	}

    bool GraphicsRenderer::Shutdown()
    {
        if (m_pWindow) 
		{
			m_pWindow->Shutdown();
			delete m_pWindow;
		}

        // let glfw cleanup opengl
        glfwTerminate();
		return true;
	}

    void GraphicsRenderer::PreRender(RenderSet * renderSet)
    {
        auto renderables = renderSet->GetRenderables();

        // set the shader to user for renderables in this set
        auto vsShader = renderSet->emitsLight ? m_vsLightShader : m_vsDefaultShader;
        auto psShader = renderSet->emitsLight ? m_psLightShader : m_psDefaultShader;

        for (Renderable * renderable : renderables)
        {
            if (renderable->m_vertexBuffer == 0 || renderable->m_vertexAttribute == 0 || renderable->m_elementBuffer == 0)
            {
                // make vertex buffer object (vbo)
                glGenBuffers(1, &renderable->m_vertexBuffer);
                // make vertex array object (vao)
                glGenVertexArrays(1, &renderable->m_vertexAttribute);
                // make element buffer object
                glGenBuffers(1, &renderable->m_elementBuffer);

                glBindVertexArray(renderable->m_vertexAttribute);
                
                    // copy vertices into vertex buffer
                    glBindBuffer(GL_ARRAY_BUFFER, renderable->m_vertexBuffer);
                    glBufferData(GL_ARRAY_BUFFER, renderable->vertices.size() * sizeof(Vertex), renderable->vertices.data(), GL_STATIC_DRAW);

                    // set indicies into element buffer
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderable->m_elementBuffer);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderable->indices.size() * sizeof(GLuint), renderable->indices.data(), GL_STATIC_DRAW);

                    // set vertex attribute pointers
                    glEnableVertexAttribArray(0);
                    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

                    // Vertex Normals
                    glEnableVertexAttribArray(1);
                    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
                
                glBindVertexArray(0);
            }

            if (renderable->m_shaderProgram == 0)
            {
                // create vertex shader
                GLuint vs = this->CreateVertexShaderFromAsset(vsShader);
                
                // create pixel/fragment shader
                GLuint ps = this->CreatePixelShaderFromAsset(psShader);

                // make shader program, to render with
                renderable->m_shaderProgram = glCreateProgram();
                glAttachShader(renderable->m_shaderProgram, ps);
                glAttachShader(renderable->m_shaderProgram, vs);
                glLinkProgram(renderable->m_shaderProgram);

                GLint result = GL_FALSE;
                int info_log_length = 0;

                glGetProgramiv(renderable->m_shaderProgram, GL_LINK_STATUS, &result);
                glGetProgramiv(renderable->m_shaderProgram, GL_INFO_LOG_LENGTH, &info_log_length);
                std::vector<char> ProgramErrorMessage( int(1) > info_log_length ? int(1) : info_log_length );
                glGetProgramInfoLog(renderable->m_shaderProgram, info_log_length, NULL, &ProgramErrorMessage[0]);
                LOG("GraphicsRenderer > Shader compile program results: ", &ProgramErrorMessage[0]);

                // now that we've built the shader program, dispose of the shaders
                glDeleteShader(ps);
                glDeleteShader(vs);
            }
        }
    }

    void GraphicsRenderer::Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> render_sets, std::vector<Light *> lights)
    {
        auto window = m_pWindow->GetWindow();

        // prep viewport for rendering
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto rs : render_sets)
        {
            this->SetRender(pCamera, rs, lights);
        }

        // swap buffer to display cool new render objects.
        glfwSwapBuffers(window);
    }

    bool GraphicsRenderer::InitializeDevice()
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            LOG_ERR("Failed to initialize GLEW.");
            return false;
        }

        // store renderer and gl version ... for debug info?
        m_pRendererInfo = glGetString(GL_RENDERER);
        m_pVersionInfo = glGetString(GL_VERSION);
        LOG("OpenGL Renderer = ", m_pRendererInfo);
        LOG("OpenGL Version = ", m_pVersionInfo);
         
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // wireframe mode!
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        return true;
    }

    void GraphicsRenderer::SetRender(std::shared_ptr<Camera> pCamera, RenderSet * renderSet, std::vector<Light *> lights)
    {

        // get/make view and projection matrix
        // view matrix
        Matrix view = pCamera->GetView();
        Vector3 viewPosition = view.Position();
        // projection matrix
        Matrix proj = pCamera->GetProjection();
        
        auto renderables = renderSet->GetRenderables();

        // Setup our lighting parameters
        float vLightPos[6] = {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f
        };
        float vLightAmbient[6] = {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f
        };
        float vLightDiffuse[6] = {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f
        };
        float vLightSpecular[6] = {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f
        };

        Vector3 lightPos;
        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;

        switch (lights.size())
        {
        case 2:
            lightPos = lights[1]->worldTransform.Position();
            vLightPos[3] = lightPos.x;
            vLightPos[4] = lightPos.y;
            vLightPos[5] = lightPos.z;

            diffuse = lights[1]->GetDiffuseLight();
            vLightDiffuse[3] = diffuse.x;
            vLightDiffuse[4] = diffuse.y;
            vLightDiffuse[5] = diffuse.z;

            ambient = lights[1]->GetAmbientLight();
            vLightAmbient[3] = ambient.x;
            vLightAmbient[4] = ambient.y;
            vLightAmbient[5] = ambient.z;

            specular = lights[1]->GetSpecularLight();
            vLightSpecular[3] = specular.x;
            vLightSpecular[4] = specular.y;
            vLightSpecular[5] = specular.z;
        case 1:
            lightPos = lights[0]->worldTransform.Position();
            vLightPos[0] = lightPos.x;
            vLightPos[1] = lightPos.y;
            vLightPos[2] = lightPos.z;

            diffuse = lights[0]->GetDiffuseLight();
            vLightDiffuse[0] = diffuse.x;
            vLightDiffuse[1] = diffuse.y;
            vLightDiffuse[2] = diffuse.z;

            ambient = lights[0]->GetAmbientLight();
            vLightAmbient[0] = ambient.x;
            vLightAmbient[1] = ambient.y;
            vLightAmbient[2] = ambient.z;

            specular = lights[0]->GetSpecularLight();
            vLightSpecular[0] = specular.x;
            vLightSpecular[1] = specular.y;
            vLightSpecular[2] = specular.z;
        default:
            break;
        }

        auto material = renderSet->material.lock();

        ambient = material->GetAmbient();
        diffuse = material->GetDiffuse();
        specular = material->GetSpecular();
        auto shininess = material->GetShininess();
        Vector4 objectColor = material->GetColor();

        for (Renderable * renderable : renderables)
        {
            // set shader program
            glUseProgram(renderable->m_shaderProgram);

            // attach model world matrix to shader
            GLuint modelLoc = glGetUniformLocation(renderable->m_shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &renderSet->worldTransform.m_11);

            // attach view matrix to shader
            GLuint viewLoc = glGetUniformLocation(renderable->m_shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view.m_11);

            // attach projection matrix to shader
            GLuint projLoc = glGetUniformLocation(renderable->m_shaderProgram, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj.m_11);

            // XXX set test light color
            GLuint objectColorLoc = glGetUniformLocation(renderable->m_shaderProgram, "objectColor");
            glUniform3f(objectColorLoc, objectColor.x, objectColor.y, objectColor.z);
            if (!renderSet->emitsLight)
            {
                // set light positions
                GLuint lightPosLoc = glGetUniformLocation(renderable->m_shaderProgram, "lightPos");
                glUniform3fv(lightPosLoc, 6, vLightPos);

                // set light colors
                GLuint lightAmbientLoc = glGetUniformLocation(renderable->m_shaderProgram, "lightAmbient");
                glUniform3fv(lightAmbientLoc, 6, vLightAmbient);

                // set light colors
                GLuint lightDiffuseLoc = glGetUniformLocation(renderable->m_shaderProgram, "lightDiffuse");
                glUniform3fv(lightDiffuseLoc, 6, vLightDiffuse);

                // set light colors
                GLuint lightSpecularLoc = glGetUniformLocation(renderable->m_shaderProgram, "lightSpecular");
                glUniform3fv(lightSpecularLoc, 6, vLightSpecular);

                // set view position variable
                GLuint viewPosLoc = glGetUniformLocation(renderable->m_shaderProgram, "viewPos");
                glUniform3f(viewPosLoc, viewPosition.x, viewPosition.y, viewPosition.z);

                // set ambient lighting variable
                GLuint ambientLoc = glGetUniformLocation(renderable->m_shaderProgram, "ambient");
                glUniform3f(ambientLoc, ambient.x, ambient.y, ambient.z);

                // set diffuse lighting variable
                GLuint diffuseLoc = glGetUniformLocation(renderable->m_shaderProgram, "diffuse");
                glUniform3f(diffuseLoc, diffuse.x, diffuse.y, diffuse.z);

                // set specular lighting variable
                GLuint specularLoc = glGetUniformLocation(renderable->m_shaderProgram, "specular");
                glUniform3f(specularLoc, specular.x, specular.y, specular.z);

                // set specular lighting variable
                GLuint shininessLoc = glGetUniformLocation(renderable->m_shaderProgram, "shininess");
                glUniform1f(shininessLoc, shininess);
            }

            // bind vertices to array object
            glBindVertexArray(renderable->m_vertexAttribute);

            // draw the triangles
            glDrawElements(GL_TRIANGLES, renderable->indices.size(), GL_UNSIGNED_INT, (void*)0);

            // unbind array object
            glBindVertexArray(0);
        }
    }

    GLuint GraphicsRenderer::CreateVertexShaderFromAsset(std::shared_ptr<Asset> vsAsset)
    {
        GLuint vs;
        GLint result = GL_FALSE;
        int info_log_length = 0;

        // create vertex shader
        std::vector<unsigned char> vsData = vsAsset->GetData();
        char * vsbuffer = reinterpret_cast<char *>(&vsData[0]);

        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsbuffer, nullptr);
        glCompileShader(vs);

        // check the shader
        glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> VertexShaderErrorMessage(info_log_length);
        glGetShaderInfoLog(vs, info_log_length, NULL, &VertexShaderErrorMessage[0]);
        LOG("GraphicsRenderer > Vertex Shader compilation result: ", &VertexShaderErrorMessage[0]);

        return vs;
    }

    GLuint GraphicsRenderer::CreatePixelShaderFromAsset(std::shared_ptr<Asset> psAsset)
    {
        GLuint ps;
        GLint result = GL_FALSE;
        int info_log_length = 0;

        // create pixel/fragment shader
        std::vector<unsigned char> psData = psAsset->GetData();
        char * psbuffer = reinterpret_cast<char *>(&psData[0]);

        ps = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(ps, 1, &psbuffer, nullptr);
        glCompileShader(ps);

        // check the shader
        glGetShaderiv(ps, GL_COMPILE_STATUS, &result);
        glGetShaderiv(ps, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> VertexShaderErrorMessage(info_log_length);
        glGetShaderInfoLog(ps, info_log_length, NULL, &VertexShaderErrorMessage[0]);
        LOG("GraphicsRenderer > Pixel Shader compilation result: ", &VertexShaderErrorMessage[0]);

        return ps;
    }
}
