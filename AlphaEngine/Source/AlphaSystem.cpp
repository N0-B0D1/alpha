/**
Copyright 2014 Jason R. Wendlandt

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

#include "AlphaSystem.h"

namespace alpha
{
    AlphaSystem::AlphaSystem(uint8_t hertz) : m_hertz(hertz)
    {
        m_updateFrequency = 1.0f / m_hertz;
    }
    AlphaSystem::~AlphaSystem() { }

    bool AlphaSystem::VInitialize()
    {
        return true;
    }

    bool AlphaSystem::Update(double currentTime, double elapsedTime)
    {
        bool success = true;
        m_elapsedTime += elapsedTime;
        if (m_elapsedTime > m_updateFrequency)
        {
            success = this->VUpdate(currentTime, m_updateFrequency);
            m_elapsedTime = m_elapsedTime - m_updateFrequency;
        }
        return success;
    }

    bool AlphaSystem::VShutdown()
    {
        return true;
    }
}