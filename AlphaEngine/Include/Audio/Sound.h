#ifndef ALPHA_SOUND_H
#define ALPHA_SOUND_H

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

#include <SDL2/SDL_audio.h>

namespace alpha
{
    class Asset;

    struct SoundData
    {
        unsigned char * position;
        unsigned int length;
    };

    /**
     * Sound is a helper class for managing an FMOD sound and its playback state.
     * Provides an interface for other engine systems to manipulate sounds.
     */
    class Sound
    {
    public:
        explicit Sound(std::weak_ptr<Asset> pAsset);
        ~Sound();

        void Play();
        void Stop();

        void SetVolume(float volume);

    private:
        std::weak_ptr<Asset> m_pAsset;

        // userdata struct
        SoundData m_pUserData;

        // SDL Audio format
        unsigned int m_wavLength;
        unsigned char * m_pWavBuffer;
        SDL_AudioSpec m_wavSpec;

        float m_volume;
    };
}

#endif // ALPHA_SOUND_H
