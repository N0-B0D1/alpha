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
#include "Entities/EntityComponent.h"
#include "Entities/CameraComponent.h"

GameState::GameState()
    : m_pCube(nullptr)
    , m_pCamera(nullptr)
{ }
GameState::~GameState() { }

bool GameState::VInitialize()
{
    // setup the state, make actors, etc.
    m_pCube = CreateEntity("Entities/cube.lua");
    m_pCamera = CreateEntity("Entities/camera.lua");

    // set our camera as the active camera for the scene
    auto pCameraComponent = std::dynamic_pointer_cast<alpha::CameraComponent>(m_pCamera->Get("root"));
    if (pCameraComponent)
    {
        // set start position for the camera
        pCameraComponent->SetPosition(alpha::Vector3(0.f, -1.8f, 10.f));

        float degrees = 25;
        float radians = static_cast<float>(degrees * (3.14 / 180));
        alpha::Quaternion q1 = alpha::Quaternion::RotationFromAxisAngle(alpha::Vector3(0, 1, 0), radians);
        pCameraComponent->SetRotation(q1);

        SetActiveCamera(pCameraComponent);
    }

    return true;
}

bool GameState::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
{
    // Update the state, move actors, shoot bullets

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
