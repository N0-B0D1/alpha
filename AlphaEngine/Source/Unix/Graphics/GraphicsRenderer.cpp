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
#include "Graphics/GeometryPass.h"
#include "Graphics/LightingPass.h"
#include "Graphics/Camera.h"
#include "Graphics/Light.h"
#include "Math/Vector4.h"

#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    GraphicsRenderer::GraphicsRenderer()
        : m_pGeometryPass(nullptr)
        , m_pLightingPass(nullptr)
    { }
    GraphicsRenderer::~GraphicsRenderer() { }

    bool GraphicsRenderer::Initialize(AssetSystem * const pAssets, int windowWidth, int windowHeight)
    {
        m_windowWidth = windowWidth;
        m_windowHeight = windowHeight;

        // init glfw
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // opengl 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
        LOG("GraphicsRenderer > Creating render window.");
		m_pWindow = new RenderWindow();
		if (!m_pWindow->Initialize(windowWidth, windowHeight))
		{
			return false;
		}

        this->InitializeDevice();

        m_pGeometryPass = new GeometryPass();
        if (!m_pGeometryPass->VInitialize(pAssets, windowWidth, windowHeight))
        {
            return false;
        }

        m_pLightingPass = new LightingPass();
        if (!m_pLightingPass->VInitialize(pAssets, windowWidth, windowHeight))
        {
            return false;
        }

        // Attach geometry render target textures to lighting pass for final render
        for (int i = 0; i < ARenderPass::GBUFFER_TEXTURE_COUNT; ++i)
        {
            auto g_index = static_cast<ARenderPass::GBUFFER_TYPE>(i);
            m_pLightingPass->AttachGBufferTexture(g_index, m_pGeometryPass->GetGBufferTexture(g_index));
        }

		return true;
	}

    bool GraphicsRenderer::Update(double currentTime, double elapsedTime)
    {
        return m_pWindow->Update(currentTime, elapsedTime);
	}

    bool GraphicsRenderer::Shutdown()
    {
        // destroy render passes
        if (m_pGeometryPass)
        {
            m_pGeometryPass->VShutdown();
            delete m_pGeometryPass;
        }
        if (m_pLightingPass)
        {
            m_pLightingPass->VShutdown();
            delete m_pLightingPass;
        }

        // close window
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
        }
    }

    void GraphicsRenderer::Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> render_sets, std::vector<Light *> lights)
    {
        auto window = m_pWindow->GetWindow();

        // prep viewport for rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render to gbuffer textures
        m_pGeometryPass->VRender(pCamera, render_sets, lights);
        // Render gbuffer textures to screen with lighting
        m_pLightingPass->VRender(pCamera, render_sets, lights);

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
        glViewport(0, 0, m_windowWidth, m_windowHeight);

        // store renderer and gl version ... for debug info?
        m_pRendererInfo = glGetString(GL_RENDERER);
        m_pVersionInfo = glGetString(GL_VERSION);
        LOG("OpenGL Renderer = ", m_pRendererInfo);
        LOG("OpenGL Version = ", m_pVersionInfo);
         
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // wireframe mode!
       // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
       
        glClearColor(0.f, 0.f, 0.f, 0.f);

        return true;
    }
}
