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
#include "Entities/MeshComponent.h"

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
    m_pLight = CreateEntity("Entities/light.lua");
    m_pLight2 = CreateEntity("Entities/light.lua");

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

    // move the light up and back
    if (auto pLightComp = std::dynamic_pointer_cast<alpha::MeshComponent>(m_pLight->Get("root")))
    {
        pLightComp->SetPosition(alpha::Vector3(2.f, 2.f, 2.f));
    }

    // move second light forward and left
    if (auto pLightComp = std::dynamic_pointer_cast<alpha::MeshComponent>(m_pLight2->Get("root")))
    {
        pLightComp->SetPosition(alpha::Vector3(0.f, -1.f, -4.f));
    }

    return true;
}

bool GameState::VUpdate(double currentTime, double /*elapsedTime*/)
{
    // rotate the light in a circle around the origin about the y axis
    if (auto pLightComp = std::dynamic_pointer_cast<alpha::MeshComponent>(m_pLight2->Get("root")))
    {
        float degrees = static_cast<float>(currentTime * 10);
        float radians = static_cast<float>(degrees * (3.14 / 180));

        alpha::Quaternion rotation = alpha::Quaternion::RotationFromAxisAngle(alpha::Vector3(0, 1, 0), radians);
        alpha::Matrix matRotation = alpha::Matrix::Rotate(rotation);
        alpha::Matrix matPosition = alpha::Matrix::Translate(alpha::Vector3(0.f, -1.f, -4.f));
        alpha::Matrix matFinal = matPosition * matRotation;

        pLightComp->SetRotation(rotation);
        pLightComp->SetPosition(matFinal.Position());
    }

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
