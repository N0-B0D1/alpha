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
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "Graphics/ARenderPass.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    ARenderPass::ARenderPass()
    { }
    ARenderPass::~ARenderPass() { }

    GLuint ARenderPass::CreateShaderProgram(std::shared_ptr<Asset> pVSAsset, std::shared_ptr<Asset> pPSAsset)
    {
        GLuint shader_program;

        // create vertex shader
        GLuint vs = CreateShaderFromAsset(pVSAsset, GL_VERTEX_SHADER);
        // create pixel/fragment shader
        GLuint ps = CreateShaderFromAsset(pPSAsset, GL_FRAGMENT_SHADER);

        // make shader program, to render with
        shader_program = glCreateProgram();
        glAttachShader(shader_program, ps);
        glAttachShader(shader_program, vs);
        glLinkProgram(shader_program);

        GLint result = GL_FALSE;
        int info_log_length = 0;

        glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_length);

        if (info_log_length > 0)
        {
            std::vector<char> ProgramErrorMessage( int(1) > info_log_length ? int(1) : info_log_length );
            glGetProgramInfoLog(shader_program, info_log_length, NULL, &ProgramErrorMessage[0]);

            LOG("GraphicsRenderer > Shader compile program results: ", &ProgramErrorMessage[0]);
        }

        // now that we've built the shader program, dispose of the shaders
        glDeleteShader(ps);
        glDeleteShader(vs);

        return shader_program;
    }

    GLuint ARenderPass::CreateShaderFromAsset(std::shared_ptr<Asset> pAsset, GLenum shaderType)
    {
        GLuint shader;
        GLint result = GL_FALSE;
        int info_log_length = 0;

        // create vertex shader
        std::vector<unsigned char> data = pAsset->GetData();
        char * shader_buffer = reinterpret_cast<char *>(&data[0]);

        shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shader_buffer, nullptr);
        glCompileShader(shader);

        // check the shader
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

        if (info_log_length > 0)
        {
            std::vector<char> shader_error_message(info_log_length);
            glGetShaderInfoLog(shader, info_log_length, NULL, &shader_error_message[0]);

            LOG("RenderPass > Shader compilation result: ", &shader_error_message[0]);
        }

        return shader;
    }
}
