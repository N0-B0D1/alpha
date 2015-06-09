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

        CreateLightBufferData(lights);

        for (auto rs : render_sets)
        {
            this->SetRender(pCamera, rs);
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

    void GraphicsRenderer::SetRender(std::shared_ptr<Camera> pCamera, RenderSet * renderSet)
    {
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

        // get/make view and projection matrix
        // view matrix
        Matrix view = pCamera->GetView();
        Vector3 viewPosition = view.Position();
        // projection matrix
        Matrix proj = pCamera->GetProjection();
        
        auto renderables = renderSet->GetRenderables();
        auto material = renderSet->material.lock();

        auto ambient = material->GetAmbient();
        auto diffuse = material->GetDiffuse();
        auto specular = material->GetSpecular();
        auto shininess = material->GetShininess();
        auto objectColor = material->GetColor();

        for (Renderable * renderable : renderables)
        {
            // set shader program
            glUseProgram(renderable->m_shaderProgram);

            // attach object matrix values
            glUniformMatrix4fv(glGetUniformLocation(renderable->m_shaderProgram, "model"), 1, GL_FALSE, &renderSet->worldTransform.m_11);
            glUniformMatrix4fv(glGetUniformLocation(renderable->m_shaderProgram, "view"), 1, GL_FALSE, &view.m_11);
            glUniformMatrix4fv(glGetUniformLocation(renderable->m_shaderProgram, "projection"), 1, GL_FALSE, &proj.m_11);

            // XXX set test light color
            glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "objectColor"), objectColor.x, objectColor.y, objectColor.z);
            if (!renderSet->emitsLight)
            {
                // set light positions
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].position"), m_pointLights[0].position.x, m_pointLights[0].position.y, m_pointLights[0].position.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].ambient"), m_pointLights[0].ambient.x, m_pointLights[0].ambient.y, m_pointLights[0].ambient.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].diffuse"), m_pointLights[0].diffuse.x, m_pointLights[0].diffuse.y, m_pointLights[0].diffuse.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].specular"), m_pointLights[0].specular.x, m_pointLights[0].specular.y, m_pointLights[0].specular.z);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].constant"), m_pointLights[0].constant);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].linear"), m_pointLights[0].linear);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[0].quadratic"), m_pointLights[0].quadratic);

                // light #2
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].position"), m_pointLights[1].position.x, m_pointLights[1].position.y, m_pointLights[1].position.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].ambient"), m_pointLights[1].ambient.x, m_pointLights[1].ambient.y, m_pointLights[1].ambient.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].diffuse"), m_pointLights[1].diffuse.x, m_pointLights[1].diffuse.y, m_pointLights[1].diffuse.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].specular"), m_pointLights[1].specular.x, m_pointLights[1].specular.y, m_pointLights[1].specular.z);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].constant"), m_pointLights[1].constant);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].linear"), m_pointLights[1].linear);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "pointLight[1].quadratic"), m_pointLights[1].quadratic);

                // directional light
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "directionalLight.direction"), m_directionalLights[0].direction.x, m_directionalLights[0].direction.y, m_directionalLights[0].direction.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "directionalLight.ambient"), m_directionalLights[0].ambient.x, m_directionalLights[0].ambient.y, m_directionalLights[0].ambient.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "directionalLight.diffuse"), m_directionalLights[0].diffuse.x, m_directionalLights[0].diffuse.y, m_directionalLights[0].diffuse.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "directionalLight.specular"), m_directionalLights[0].specular.x, m_directionalLights[0].specular.y, m_directionalLights[0].specular.z);

                // set view position variable
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "viewPos"), viewPosition.x, viewPosition.y, viewPosition.z);

                // set object light colors
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "ambient"), ambient.x, ambient.y, ambient.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "diffuse"), diffuse.x, diffuse.y, diffuse.z);
                glUniform3f(glGetUniformLocation(renderable->m_shaderProgram, "specular"), specular.x, specular.y, specular.z);
                glUniform1f(glGetUniformLocation(renderable->m_shaderProgram, "shininess"), shininess);
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

    void GraphicsRenderer::CreateLightBufferData(const std::vector<Light *> & lights)
    {
        m_pointLights.clear();
        m_directionalLights.clear();

        for (auto light : lights)
        {
            switch(light->GetLightType())
            {
            case LightType::DIRECTIONAL:
                m_directionalLights.push_back(DirectionalLight());

                m_directionalLights[m_directionalLights.size() - 1].direction = Vector4(light->GetLightDirection(), 1.f);
                m_directionalLights[m_directionalLights.size() - 1].ambient = light->GetAmbientLight();
                m_directionalLights[m_directionalLights.size() - 1].diffuse = light->GetDiffuseLight();
                m_directionalLights[m_directionalLights.size() - 1].specular = light->GetSpecularLight();
                break;

            case LightType::POINT:
                m_pointLights.push_back(PointLight());

                //LOG_ERR(light->GetAttenuationConstant(), ", ", light->GetAttenuationLinear(), ", ", light->GetAttenuationQuadratic());

                m_pointLights[m_pointLights.size() - 1].position = Vector4(light->worldTransform.Position(), 1.f);
                m_pointLights[m_pointLights.size() - 1].ambient = light->GetAmbientLight();
                m_pointLights[m_pointLights.size() - 1].diffuse = light->GetDiffuseLight();
                m_pointLights[m_pointLights.size() - 1].specular = light->GetSpecularLight();
                m_pointLights[m_pointLights.size() - 1].constant = light->GetAttenuationConstant();
                m_pointLights[m_pointLights.size() - 1].linear = light->GetAttenuationLinear();
                m_pointLights[m_pointLights.size() - 1].quadratic = light->GetAttenuationQuadratic();
                break;

            default:
                LOG_WARN("Unknown light type, unable to render: ", light->GetLightType());
                break;
            }
        }
    }
}
