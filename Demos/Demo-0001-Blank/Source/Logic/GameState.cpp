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

#include "Logic/GameState.h"

GameState::GameState() { }
GameState::~GameState() { }

bool GameState::VInitialize()
{
    // setup the state, make actors, etc.

    return true;
}

bool GameState::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
{
    // Update the state, move actors, shoot bullets, solve world hunger

    return true;
}

void GameState::VTransition(std::shared_ptr<AState> nextState)
{
    AGameState::VTransition(nextState);
    // Pass any persistent data to the next state if necessary.
}

std::shared_ptr<alpha::AState> GameState::VShutdown()
{
    // return the next game state, or nullptr for end of state machine

    return nullptr;
}
