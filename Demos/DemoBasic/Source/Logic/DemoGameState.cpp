#include <functional>
#include <math.h>

#include "Logic/DemoGameState.h"

#include "FSA/GameState.h"
#include "Entities/Entity.h"
#include "Entities/EntityComponent.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Audio/Sound.h"
#include "HID/DemoContext.h"
#include "Toolbox/Logger.h"

DemoGameState::DemoGameState()
    : m_pInputContext(nullptr)
{ }
DemoGameState::~DemoGameState() { }

bool DemoGameState::VInitialize()
{
    // setup the state, make actors, etc.

    m_test = CreateEntity("Entities/test.lua");
    m_test2 = CreateEntity("Entities/test.lua");

    auto root = std::dynamic_pointer_cast<alpha::SceneComponent>(m_test->Get("root"));
    if (root != nullptr)
    {
        root->SetPosition(alpha::Vector3(0, 1, -1));
    }

    root = std::dynamic_pointer_cast<alpha::SceneComponent>(m_test2->Get("root"));
    if (root != nullptr)
    {
        root->SetPosition(alpha::Vector3(5, 5, 0));
    }

    // create a test sound to play
    m_pTestSound = this->CreateSound("media/hit.mp3");
    if (auto pSound = m_pTestSound.lock())
    {
        // hit sound is fairly loud, so set the channel low on play
        pSound->SetVolume(0.2f);
    }

    m_pInputContext = new DemoContext();
    this->SetActiveInputContext(m_pInputContext);
    this->BindState("STRAFE_LEFT", [this](bool pressed) { this->OnStrafeLeft(pressed); });

    return true;
}

bool DemoGameState::VUpdate(double /*currentTime*/, double elapsedTime)
{
    // Update the state, move actors, shoot bullets, blah blah

    // get root node and rotate it
    // I know root is a scene component, so cast it
    auto root = std::dynamic_pointer_cast<alpha::SceneComponent>(m_test->Get("root"));
    auto root2 = std::dynamic_pointer_cast<alpha::SceneComponent>(m_test2->Get("root"));

    // but just in case, check before doing anything
    if (root != nullptr && root2 != nullptr)
    {
        //float t = static_cast<float>(currentTime); // / 100.0f;
        static int tick = 0;
        tick += 1;

        float degrees = static_cast<float>(tick / 4);
        float radians = static_cast<float>(degrees * (3.14 / 180));

        alpha::Quaternion q1 = alpha::Quaternion::RotationFromAxisAngle(alpha::Vector3(0, 0, -1), radians);
        root->SetRotation(q1);

        // slerp back and forth between 45 degrees and 135 degrees
        static alpha::Quaternion q90 = alpha::Quaternion::RotationFromAxisAngle(alpha::Vector3(0, 0, 1), 135 * (3.14f / 180.f));
        static alpha::Quaternion q00 = alpha::Quaternion::RotationFromAxisAngle(alpha::Vector3(0, 0, 1), 45 * (3.14f / 180.f));
        static alpha::Quaternion q2end = q90;
        static float slerpTime = 0.0f;

        alpha::Quaternion q2start = root2->GetRotation();
        if (q2start == q00)
        {
            // got back to 00
            q2end = q90;
            slerpTime = 0.001f;

            // whenever we reach a target reversal point, play the test sound
            if (auto pSound = m_pTestSound.lock())
            {
                pSound->Stop();
                pSound->Play();
            }
        }
        if (q2start == q90)
        {
            // got to 90 degrees
            q2end = q00;
            slerpTime = 0.001f;

            // whenever we reach a target reversal point, play the test sound
            if (auto pSound = m_pTestSound.lock())
            {
                pSound->Stop();
                pSound->Play();
            }
        }
        alpha::Quaternion q2slerp = alpha::Quaternion::Slerp(q2start, q2end, slerpTime);
        root2->SetRotation(q2slerp);
        slerpTime += static_cast<float>(elapsedTime) / 45.f;
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
    if (m_pInputContext)
    {
        delete m_pInputContext;
    }

    // Remove any actors that are no longer needed
    DestroyEntity(m_test->GetId());
    DestroyEntity(m_test2->GetId());

    // return the next game state, or nullptr for end of state machine
    return nullptr;
}

void DemoGameState::OnStrafeLeft(bool pressed)
{
    if (pressed)
    {
        m_strafingLeft = true;
    }
    else
    {
        m_strafingLeft = false;
    }
}