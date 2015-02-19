#include <math.h>

#include "Logic/DemoGameState.h"

#include "FSA/GameState.h"
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

DemoGameState::DemoGameState()
{ }
DemoGameState::~DemoGameState() { }

bool DemoGameState::VInitialize()
{
    // setup the state, make actors, etc.

    m_test = CreateEntity("Entities/test.lua");
    m_test2 = CreateEntity("Entities/test.lua");

    return true;
}

bool DemoGameState::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
{
    // Update the state, move actors, shoot bullets, blah blah

    // get root node and rotate it
    // I know root is a scene component, so cast it
    auto root = std::dynamic_pointer_cast<alpha::SceneComponent>(m_test->Get("root"));

    // but just in case, check before doing anything
    if (root != nullptr)
    {
        //float t = static_cast<float>(currentTime); // / 100.0f;
        static int tick = 0;
        tick += 1;

        float degrees = static_cast<float>(tick / 3);
        float radians = static_cast<float>(degrees * (3.14 / 180));

        alpha::Quaternion q;
        q.RotationFromAxisAngle(alpha::Vector3(0, 0, 1), radians);
        root->SetRotation(q);
    }

    return true;
}

void DemoGameState::VTransition(std::shared_ptr<AState> nextState)
{
    AGameState::VTransition(nextState);
    // Pass any persistent data to the next state if necessary.
}

std::shared_ptr<alpha::AState> DemoGameState::VShutdown()
{
    // Remove any actors that are no longer needed
    DestroyEntity(m_test->GetId());
    DestroyEntity(m_test2->GetId());

    // return the next game state, or nullptr for end of state machine
    return nullptr;
}