#include "Logic/DemoGameState.h"
#include "FSA/GameState.h"
#include "Entities/Entity.h"

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