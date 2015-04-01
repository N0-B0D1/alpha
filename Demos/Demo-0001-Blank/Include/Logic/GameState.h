#ifndef GAME_STATE_H
#define GAME_STATE_H

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

#include "FSA/GameState.h"

class GameState : public alpha::AGameState
{
public:
    GameState();
    virtual ~GameState();

    virtual bool VInitialize();
    virtual bool VUpdate(double currentTime, double elapsedTime);
    virtual void VTransition(std::shared_ptr<AState> nextState);
    virtual std::shared_ptr<AState> VShutdown();
};

#endif // GAME_STATE_H
