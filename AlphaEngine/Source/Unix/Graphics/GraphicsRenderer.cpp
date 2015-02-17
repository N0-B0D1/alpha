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

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "Graphics/GraphicsRenderer.h"
#include "Graphics/RenderWindow.h"
#include "Graphics/RenderData.h"

#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    const std::string GraphicsRenderer::sk_shader_extension = "glsl";

    GraphicsRenderer::GraphicsRenderer() { }
    GraphicsRenderer::~GraphicsRenderer() { }

    bool GraphicsRenderer::Initialize()
    {
        LOG("GraphicsRenderer > Creating render window.");
		m_pWindow = new RenderWindow();
		if (!m_pWindow->Initialize())
		{
			return false;
		}

        this->InitializeDevice();

        
        static const GLfloat vertices[] = {
            -0.5f,  -0.5f, 0.0f,
             0.5f,  -0.5f, 0.0f,
             0.0f,   0.5f, 0.0f
        };
        
        // make vertex buffer object (vbo)
        glGenBuffers(1, &m_VertexBuffer);
        // make vertex array object (vao)
        glGenVertexArrays(1, &m_VertexAttribute);

        glBindVertexArray(m_VertexAttribute);
          // copy vertices into vertex buffer
          glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
          glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
          // set vertex attribute pointers
          glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
          glEnableVertexAttribArray(0);
        glBindVertexArray(0);

        // create vertex shader
        GLuint vs = this->CreateVertexShaderFromAsset(m_vsDefaultShader);
        
        // create pixel/fragment shader
        GLuint ps = this->CreatePixelShaderFromAsset(m_psDefaultShader);

        // make shader program, to render with
        m_ShaderProgram = glCreateProgram();
        glAttachShader(m_ShaderProgram, ps);
        glAttachShader(m_ShaderProgram, vs);
        glLinkProgram(m_ShaderProgram);

        GLint result = GL_FALSE;
        int info_log_length = 0;

        glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &result);
        glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> ProgramErrorMessage( int(1) > info_log_length ? int(1) : info_log_length );
        glGetProgramInfoLog(m_ShaderProgram, info_log_length, NULL, &ProgramErrorMessage[0]);
        LOG("GraphicsRenderer > Shader program results: ", &ProgramErrorMessage[0]);

        // now that we've built the shader program, dispose of the shaders
        glDeleteShader(ps);
        glDeleteShader(vs);

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

    void GraphicsRenderer::Render(std::vector<RenderData *> & /*renderables*/)
    {
        //auto display = m_pWindow->GetDisplay();
        auto window = m_pWindow->GetWindow();
        //auto attrs = m_pWindow->GetXWindowAttrs();

        // prep viewport for rendering
        //glViewport(0, 0, attrs.width, attrs.height);
        glViewport(0, 0, 800, 600);

        // draw some stuff, like a cube or some shit.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // set shader program
        glUseProgram(m_ShaderProgram);

        // bind vertices to array object
        glBindVertexArray(m_VertexAttribute);

        // draw the triangles
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // unbind array object
        glBindVertexArray(0);

        // swap buffer to display cool new render objects.
        glfwSwapBuffers(window);
    }

    void GraphicsRenderer::SetBasicShaders(std::shared_ptr<Asset> psShader, std::shared_ptr<Asset> vsShader)
    {
        m_psDefaultShader = psShader;
        m_vsDefaultShader = vsShader;
    }

    bool GraphicsRenderer::InitializeDevice()
    {
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            LOG_ERR("Failed to initialize GLEW.");
            return -1;
        }

        // store renderer and gl version ... for debug info?
        m_pRendererInfo = glGetString(GL_RENDERER);
        m_pVersionInfo = glGetString(GL_VERSION);
        LOG("OpenGL Renderer = ", m_pRendererInfo);
        LOG("OpenGL Version = ", m_pVersionInfo);
         
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        return true;
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
