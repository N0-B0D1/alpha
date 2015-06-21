#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

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

#include "AlphaSystem.h"

namespace alpha
{
    class IRenderer;
    class SceneManager;
    class RenderData;
    class Camera;
    class AssetSystem;
    class Asset;
    class AEvent;

    class GraphicsSystem : public AlphaSystem
    {
    public:
        GraphicsSystem();
        virtual ~GraphicsSystem();

        virtual bool VInitialize();
        virtual bool VShutdown();

        void Render();

        void SetAssetSystem(AssetSystem * const pAssets);

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);

        /** Handle entity created event. */
        void HandleEntityCreatedEvent(AEvent * pEvent);
        /** Handle set active camera event */
        void HandleSetActiveCameraEvent(AEvent * pEvent);

        /** A handle to the main asset system. */
        AssetSystem * m_pAssets;
        /** Renderer implementation (e.g.: DirectX, OpenGL) */
        IRenderer *m_pRenderer;
        /** SceneManager for tracking logic and propagation of renderable objects in the Scene */
        SceneManager * m_pSceneManager;
        /** Track the current camera that is viewing the scene */
        std::shared_ptr<Camera> m_pCamera;
    };
}

#endif // GRAPHICS_SYSTEM_H
