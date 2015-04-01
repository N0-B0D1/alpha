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

#include "Logic/GameLogic.h"

GameLogic::GameLogic() { }
GameLogic::~GameLogic() { }

bool GameLogic::VInitialize()
{
    if (LogicSystem::VInitialize())
    {
        // create any game specific systems here
        // this should be a prep spot for setting up the game to run

        //e.g. should not make entities here, as proper events will not be sent or received.
        // Unless those entities should persist throughout the games life

        return true;
    }
    return false;
}
