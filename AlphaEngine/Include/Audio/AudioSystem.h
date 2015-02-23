#ifndef ALPHA_AUDIO_SYSTEM_H
#define ALPHA_AUDIO_SYSTEM_H

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

#include <Windows.h>
#include "fmod.hpp"

#include "AlphaSystem.h"

namespace alpha
{
    class AudioSystem : public AlphaSystem
    {
    public:
        AudioSystem();
        virtual ~AudioSystem();

        virtual bool VInitialize();
        virtual bool VShutdown();

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);

        FMOD::System * m_pSystem;
        FMOD::Sound * m_pSound;
        FMOD::Channel * m_pChannel;
    };
}

#endif // ALPHA_AUDIO_SYSTEM_H