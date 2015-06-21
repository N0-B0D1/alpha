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

#include "Events/EventDataPublisher.h"
#include "Events/EventDataSubscriber.h"
#include "Events/EventData_ThreadTaskCreated.h"

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

        /** Allow other systems to subscribe to new threading tasks from this system */
        void SubscribeToThreadTaskCreated(std::shared_ptr<AEventDataSubscriber> pSubscriber);

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);
        /** Read the EntityCreated subscription on each update to handle any new entities that need to be rendered. */
        void ReadSubscriptions();

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

        /** Publisher for new threading tasks */
        std::shared_ptr<EventDataPublisher<EventData_ThreadTaskCreated>> m_pubThreadTaskCreated;
    };
}

#endif // GRAPHICS_SYSTEM_H
