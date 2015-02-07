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

#include "FSA/StateMachine.h"
#include "FSA/State.h"

#include "Toolbox/Logger.h"

namespace alpha
{
    StateMachine::StateMachine(std::shared_ptr<AState> startingState)
        : m_pCurrentState(startingState)
    {
        LOG("StateMachine beginning execution.");
        // initialize the starting state, so it is ready to immediate update calls
        m_pCurrentState->VInitialize();
    }

    bool StateMachine::Update()
    {
        // update current state
        bool complete = m_pCurrentState->VUpdate();

        // if state is complete, advance state
        if (complete)
        {
            LOG("StateMachine shutting down current state.");
            std::shared_ptr<AState> nextState = m_pCurrentState->VShutdown();

            if (nextState == nullptr)
            {
                LOG("StateMachine completed execution.");
                return false;
            }

            LOG("StateMachine transitioning to next state.");
            m_pCurrentState = nextState;
            m_pCurrentState->VInitialize();
        }

        return true;
    }
}
