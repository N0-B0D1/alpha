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

#include <vector>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "Graphics/GraphicsRenderer.h"
#include "Graphics/RenderWindow.h"

#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    const std::string GraphicsRenderer::sk_shader_extension = "glsl";

    GraphicsRenderer::GraphicsRenderer() { }
    GraphicsRenderer::~GraphicsRenderer() { }

    bool GraphicsRenderer::Initialize()
    {
		m_pWindow = new RenderWindow();
		if (!m_pWindow->Initialize())
		{
			return false;
		}

        this->InitializeDevice();

        float vertices[] = {
             0.0f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
        };
        
        //GLuint vbo;
        glGenBuffers(1, &m_VertexBuffer); // Generate 1 buffer
        //glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

        
        //GLuint vao = 0;
        glGenVertexArrays(1, &m_VertexAttribute);
        glBindVertexArray(m_VertexAttribute);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        // create vertex shader
        std::vector<unsigned char> vsData = m_vsDefaultShader->GetData();
        char * vsbuffer = reinterpret_cast<char *>(&vsData[0]);
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsbuffer, nullptr);
        glCompileShader(vs);
        
        // create pixel/fragment shader
        std::vector<unsigned char> psData = m_psDefaultShader->GetData();
        char * psbuffer = reinterpret_cast<char *>(&psData[0]);
        GLuint ps = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(ps, 1, &psbuffer, nullptr);
        glCompileShader(ps);

        // make shader program, to render with
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, ps);
        glAttachShader(m_ShaderProgram, vs);
        glLinkProgram(m_ShaderProgram);

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
            // destroy opengl context before disposing of the window.
            auto display = m_pWindow->GetDisplay();
            glXMakeCurrent(display, None, NULL);
            glXDestroyContext(display, m_glContext);

			m_pWindow->Shutdown();
			delete m_pWindow;
		}
		return true;
	}

    void GraphicsRenderer::Render()
    {
        auto display = m_pWindow->GetDisplay();
        auto window = m_pWindow->GetWindow();
        auto attrs = m_pWindow->GetXWindowAttrs();

        // prep viewport for rendering
        glViewport(0, 0, attrs.width, attrs.height);

        // draw some stuff, like a cube or some shit.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set shader program
        glUseProgram(m_ShaderProgram);

        glBindVertexArray(m_VertexAttribute);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffer to display cool new render objects.
        glXSwapBuffers(display, window); 
    }

    void GraphicsRenderer::SetBasicShaders(std::shared_ptr<Asset> psShader, std::shared_ptr<Asset> vsShader)
    {
        m_psDefaultShader = psShader;
        m_vsDefaultShader = vsShader;
    }

    bool GraphicsRenderer::InitializeDevice()
    {
        auto display = m_pWindow->GetDisplay();
        auto window = m_pWindow->GetWindow();
        auto visual_info = m_pWindow->GetVisualInfo();

        m_glContext = glXCreateContext(display, visual_info, NULL, GL_TRUE);
        glXMakeCurrent(display, window, m_glContext);

        glewInit();

        m_pRendererInfo = glGetString(GL_RENDERER);
        m_pVersionInfo = glGetString(GL_VERSION);
        LOG("OpenGL Renderer = ", m_pRendererInfo);
        LOG("OpenGL Version = ", m_pVersionInfo);
         
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        return true;
    }
}
