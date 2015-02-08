#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

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

#include "AlphaSystem.h"

namespace alpha
{
    class AState;

    class StateMachine : public AlphaSystem
    {
    public:
        /** \brief The Primary Constructor
         * \param startingState The state which the StateMachien will begin processing from.
         */
        explicit StateMachine(std::shared_ptr<AState> startingState);

        virtual bool VInitialize();
        virtual bool VShutdown();

    private:
        /** \brief Update the current state and advance to next state if necessary.
        * \param currentTime The current global tick time.
        * \param elapsedTime Time elapsed since last update.
        */
        virtual bool VUpdate(double currentTime, double elapsedTime);

        /** Tracks the current active state. */
        std::shared_ptr<AState> m_pCurrentState;
    };
}

#endif // STATE_MACHINE_H
