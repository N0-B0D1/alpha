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

    void GraphicsRenderer::Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> render_sets, std::vector<Light *> lights)
    {
        this->PreRender(render_sets);

        auto window = m_pWindow->GetWindow();

        // prep viewport for rendering
        glViewport(0, 0, 800, 600);

        // draw some stuff, like a cube or some shit.
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

    void GraphicsRenderer::PreRender(std::vector<RenderSet *> render_sets)
    {
        for (auto rs : render_sets)
        {
            this->PreRenderSet(rs);
        }
    }

    void GraphicsRenderer::PreRenderSet(RenderSet * renderSet)
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
        float vLightDirs[6] =
        {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f
        };
        float vLightColors[6] =
        {
            0.f, 0.f, 0.f,
            0.f, 0.f, 0.f
        };

        Vector3 lightPos;
        switch (lights.size())
        {
        case 2:
            lightPos = lights[1]->worldTransform.Position();
            vLightDirs[3] = lightPos.x;
            vLightDirs[4] = lightPos.y;
            vLightDirs[5] = lightPos.z;

            vLightColors[3] = lights[1]->m_color.x;
            vLightColors[4] = lights[1]->m_color.y;
            vLightColors[5] = lights[1]->m_color.z;
        case 1:
            lightPos = lights[0]->worldTransform.Position();
            vLightDirs[0] = lightPos.x;
            vLightDirs[1] = lightPos.y;
            vLightDirs[2] = lightPos.z;

            vLightColors[0] = lights[0]->m_color.x;
            vLightColors[1] = lights[0]->m_color.y;
            vLightColors[2] = lights[0]->m_color.z;
        default:
            break;
        }

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
            glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);

            if (!renderSet->emitsLight)
            {
                // set light colors
                GLuint lightColorLoc = glGetUniformLocation(renderable->m_shaderProgram, "lightColor");
                glUniform3fv(lightColorLoc, 6, vLightColors);

                // set light positions
                GLuint lightPosLoc = glGetUniformLocation(renderable->m_shaderProgram, "lightPos");
                glUniform3fv(lightPosLoc, 6, vLightDirs);

                // set specular lighting variables
                GLuint viewPosLoc = glGetUniformLocation(renderable->m_shaderProgram, "viewPos");
                glUniform3f(viewPosLoc, viewPosition.x, viewPosition.y, viewPosition.z);
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
