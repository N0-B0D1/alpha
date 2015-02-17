#ifndef RENDERER_H
#define RENDERER_H

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
#include <string>
#include <vector>

//#include<GL/gl.h>
#include<GL/glx.h>
//#include<GL/glu.h>

namespace alpha
{
	class RenderWindow;
    class RenderData;
    class Asset;

    class GraphicsRenderer
    {
    public:
        static const std::string sk_shader_extension;

        GraphicsRenderer();
        virtual ~GraphicsRenderer();

        bool Initialize();
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        void Render(std::vector<RenderData *> & renderables);

        /** Set basic GLSL shaders for default usage */
        void SetBasicShaders(std::shared_ptr<Asset> psShader, std::shared_ptr<Asset> vsShader);

    private:
        bool InitializeDevice();

        RenderWindow *m_pWindow;

        std::shared_ptr<Asset> m_vsDefaultShader;
        std::shared_ptr<Asset> m_psDefaultShader;

        /** OpenGL variables */
        //GLXContext m_glContext;

        GLuint m_VertexBuffer;
        GLuint m_VertexAttribute;
        GLuint m_ElementBuffer;
        GLuint m_ShaderProgram;
        
        const GLubyte *m_pRendererInfo; // = glGetString(GL_RENDERER);
        const GLubyte *m_pVersionInfo; // = glGetString(GL_VERSION);

        /** Creates and returns a Vertex Shader from the given asset, also outputs blob data which can be passed into Input Layout creation */
        GLuint CreateVertexShaderFromAsset(std::shared_ptr<Asset> vsAsset);
        /** Creates and returns a Pixel/Fragment Shader from the given asset */
        GLuint CreatePixelShaderFromAsset(std::shared_ptr<Asset> psAsset);
    };
}

#endif // RENDERER_H
