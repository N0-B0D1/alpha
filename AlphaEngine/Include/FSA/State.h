#ifndef FINITE_STATE_H
#define FINITE_STATE_H

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

#include <memory>
#include <string>

namespace alpha
{
    class AState
    {
    public:
        virtual ~AState();

        /** Prep the State to run */
        virtual bool VInitialize() = 0;

        /**
         * Called by the StateMachine when this is the active state.
         * Should return false when the state is done and/or ready to transition.
         */
        virtual bool VUpdate(double currentTime, double elapsedTime) = 0;

        /**
         * \brief Transition allows custom logic for passing data between current and next state.
         */
        virtual void VTransition(std::shared_ptr<AState> nextState) = 0;

        /**
         * Shutdown the state, and prep for transition to next state
         * This should return the next state that should be transitioned to
         * or nullptr to tell the StateMachine to exit.
         */
        virtual std::shared_ptr<AState> VShutdown() = 0;
    };
}

#endif // FINITE_STATE_H
