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

#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include "AlphaSystem.h"

namespace alpha
{
    class Asset;
    class Sound;

    class AudioSystem : public AlphaSystem
    {
    public:
        AudioSystem();
        virtual ~AudioSystem();

        virtual bool VInitialize();
        virtual bool VShutdown();

        /** Create a sound from a given data asset */
        std::weak_ptr<Sound> CreateSound(std::shared_ptr<Asset> pAsset);

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);

        /** Manage a list of sounds, so they can be properly updated and disposed of. */
        std::vector<std::shared_ptr<Sound>> m_sounds;

        /** Audio device spec */
        SDL_AudioSpec m_audioSpec;
        SDL_AudioDeviceID m_audioDevID;
    };
}

#endif // ALPHA_AUDIO_SYSTEM_H
