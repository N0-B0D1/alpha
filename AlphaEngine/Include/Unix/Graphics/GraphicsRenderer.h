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

#include <GL/glx.h>

namespace alpha
{
	class RenderWindow;
    class RenderSet;
    class Camera;
    class Asset;
    class AssetSystem;
    class Light;

    class GraphicsRenderer
    {
    public:
        GraphicsRenderer();
        virtual ~GraphicsRenderer();

        bool Initialize(std::shared_ptr<AssetSystem> pAssets);
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        void Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights);

    private:
        /** Initializes the OpenGL device with GLFW */
        bool InitializeDevice();

        /** PreRender takes a list of data that will be rendered, and preps it rendering. */
        void PreRender(std::vector<RenderSet *> renderables);
        /** Pre-render a set of Renderables */
        void PreRenderSet(RenderSet * renderSet);
        /** Render a set of renderables */
        void SetRender(std::shared_ptr<Camera> pCamera, RenderSet * renderSet, std::vector<Light *> lights);

        RenderWindow *m_pWindow;

        std::shared_ptr<Asset> m_vsDefaultShader;
        std::shared_ptr<Asset> m_psDefaultShader;
        
        const GLubyte *m_pRendererInfo;
        const GLubyte *m_pVersionInfo;

        /** Creates and returns a Vertex Shader from the given asset, also outputs blob data which can be passed into Input Layout creation */
        GLuint CreateVertexShaderFromAsset(std::shared_ptr<Asset> vsAsset);
        /** Creates and returns a Pixel/Fragment Shader from the given asset */
        GLuint CreatePixelShaderFromAsset(std::shared_ptr<Asset> psAsset);
    };
}

#endif // RENDERER_H
