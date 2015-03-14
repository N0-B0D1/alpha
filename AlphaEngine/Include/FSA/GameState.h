#ifndef ALPHA_GAME_STATE_H
#define ALPHA_GAME_STATE_H

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

#include <functional>
#include <string>

#include "FSA/State.h"

namespace alpha
{
    class AlphaController;
    class LogicSystem;
    class Entity;
    class CameraComponent;
    class Sound;
    class HIDContext;

    /**
     * class GameState
     *
     * GameState is a special type of state which has access to the game Logic System.
     * Which allows it to do things like load assets and create entities.
     */
    class AGameState : public AState
    {
        // only friends can touch your privates
        friend class AlphaController;
    public:
        virtual ~AGameState();

        virtual void VTransition(std::shared_ptr<AState> nextState);

        // Create any helpful methods that might be useful for a programmer building a game using alpha.
        // The Logic System should always be accessed via a helper function, so it does not have to be
        // directely exposed to any derived class.

        /** Provide pass through access to entity life-cycle methods. */
        std::shared_ptr<Entity> GetEntity(const unsigned long entityId);
        std::shared_ptr<Entity> CreateEntity(const char * resource);
        void DestroyEntity(const unsigned long entityId);

        /** Audio system pass through methods */
        std::weak_ptr<Sound> CreateSound(const char * resource);

        /** Set the active context in the input context manager */
        void SetActiveInputContext(HIDContext * context);
        /** Bind function delegates to input action */
        void BindAction(std::string action, std::function<void()> delegate);
        /** Bind function delegate to input action state */
        void BindState(std::string state, std::function<void(bool)> delegate);
        /** Bind function delegate to input action axis range */
        void BindRange(std::string range, std::function<void(long, float)> delegate);

        /** Set the specified camera as the active camera to render with */
        void SetActiveCamera(std::shared_ptr<CameraComponent> pCameraComponent);

    private:
        void SetLogic(std::shared_ptr<LogicSystem> pLogic);
        std::shared_ptr<LogicSystem> m_pLogic;
    };
}

#endif // ALPHA_GAME_STATE_H
