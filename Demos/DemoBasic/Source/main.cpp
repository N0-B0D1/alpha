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

#include "AlphaController.h"
#include "Logic/GameLogic.h"
#include "Logic/DemoGameState.h"

#if WIN32
#include <Windows.h>
int WINAPI wWinMain(_In_ HINSTANCE /*hInstance*/, _In_opt_ HINSTANCE /*hPrevInstance*/, _In_ LPWSTR /*lpCmdLine*/, _In_ int /*nCmdShow*/)
#else
int main(int /*argc*/, char ** /*argv*/)
#endif
{
    alpha::AlphaController controller;

    // The controller will manage the logic system life-cycle, no need to delete it
    GameLogic * pLogic = new GameLogic();
    controller.SetLogic(pLogic);

    // set starting game state
    std::shared_ptr<DemoGameState> state = std::make_shared<DemoGameState>();
    controller.SetGameState(state);

    // begin engine execution
    controller.Execute();

    return 0;
}
