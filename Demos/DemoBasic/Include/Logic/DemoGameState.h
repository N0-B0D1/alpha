#ifndef DEMO_GAME_STATE_H
#define DEMO_GAME_STATE_H

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

namespace alpha
{
    class Entity;
    class Sound;
}

class DemoContext;

class DemoGameState : public alpha::AGameState
{
public:
    DemoGameState();
    virtual ~DemoGameState();

    virtual bool VInitialize();
    virtual bool VUpdate(double currentTime, double elapsedTime);
    virtual void VTransition(std::shared_ptr<AState> nextState);
    virtual std::shared_ptr<AState> VShutdown();

    void OnStrafeLeft(bool pressed);
    void OnStrafeRight(bool pressed);

private:
    std::shared_ptr<alpha::Entity> m_test;
    std::shared_ptr<alpha::Entity> m_test2;

    std::shared_ptr<alpha::Entity> m_pCamera;

    std::weak_ptr<alpha::Sound> m_pTestSound;

    DemoContext * m_pInputContext;

    /** Track input state */
    bool m_strafeLeft;
    bool m_strafeRight;
};

#endif // DEMO_GAME_STATE_H
