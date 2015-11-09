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

#include <SDL.h>
#include <SDL_video.h>

#include "Graphics/IRenderer.h"

namespace alpha
{
	class RenderWindow;
    class RenderSet;
    class GeometryPass;
    class LightingPass;
    class Camera;
    class AssetSystem;
    class Light;

    class GraphicsRenderer : public IRenderer
    {
    public:
        GraphicsRenderer();
        virtual ~GraphicsRenderer();

        bool Initialize(AssetSystem * const pAssets, int windowWidth, int windowHeight);
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        /** PreRender takes a list of data that will be rendered, and preps it rendering. */
        void PreRender(RenderSet * renderSet);
        void Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights);

    private:
        /** Initializes the OpenGL device with SDL/GLEW */
        bool InitializeDevice();

        /** Game window width */
        int m_windowWidth;
        /** Game window height */
        int m_windowHeight;

        /** Window manager */
        RenderWindow *m_pWindow;

        /** SDL OpengGL Context */
        SDL_GLContext m_GLContext;

        /** Geometry render pass for render to GBuffer */
        GeometryPass * m_pGeometryPass;
        /** Lighting render pass for final render to screen */
        LightingPass * m_pLightingPass;
        
        const GLubyte *m_pRendererInfo;
        const GLubyte *m_pVersionInfo;
    };
}

#endif // RENDERER_H
