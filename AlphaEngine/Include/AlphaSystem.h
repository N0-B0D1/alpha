#ifndef ALPHA_SYSTEM_H
#define ALPHA_SYSTEM_H

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

#include <cstdint>

namespace alpha
{
    /**
     * The AlphaSystem represents a classic engine sub-system, such as Graphcs, AI, Physics, etc.
     * Or it might represent a set of game logic.
     */
    class AlphaSystem
    {
        friend class AlphaController;

    public:
        AlphaSystem(uint8_t hertz);
        virtual ~AlphaSystem();

        virtual bool VInitialize() = 0;
        bool Update(double currentTime, double elapsedTime);
        virtual bool VShutdown() = 0;

    private:
        // non-copyable
        AlphaSystem(const AlphaSystem&);
        AlphaSystem & operator=(const AlphaSystem&);

        virtual bool VUpdate(double currentTime, double elapsedTime) = 0;

        /** update frequency */
        uint8_t m_hertz;
        double m_updateFrequency;
        double m_elapsedTime = 0.0f;
    };
}

#endif // ALPHA_SYSTEM_H
