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

#include "FSA/State.h"

namespace alpha
{
    class LogicSystem;

    /**
     * class GameState
     *
     * GameState is a special type of state which has access to the game Logic System.
     * Which allows it to do things like load assets and create entities.
     */
    class GameState : public AState
    {
    public:
        virtual ~GameState();

        // Create any helpful methods that might be useful for a programmer building a game using alpha.
        // The Logic System should always be accessed via a helper function, so it does not have to be
        // directely exposed to any derived class.
        
    private:
        std::shared_ptr<LogicSystem> m_pLogic;
    };
}

#endif // ALPHA_GAME_STATE_H
