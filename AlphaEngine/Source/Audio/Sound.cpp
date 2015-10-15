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

#include <string.h>

#include "Audio/Sound.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    void mix_callback(void *userdata, unsigned char *stream, int length)
    {
        SoundData *data = (struct SoundData*)userdata;

        length = (length > (int)data->length ? data->length : length);
        SDL_MixAudio(stream, data->position, length, SDL_MIX_MAXVOLUME);

        data->position += length;
        data->length -= length;
    }

    Sound::Sound(std::weak_ptr<Asset> pAsset)
        : m_pAsset(pAsset)
        , m_pWavBuffer(nullptr)
    {
        // create the sound so it is preped for use
        if (auto asset = pAsset.lock())
        {
            std::string path = asset->GetPath();

            if (SDL_LoadWAV(path.c_str(), &m_wavSpec, &m_pWavBuffer, &m_wavLength) == nullptr)
            {
                LOG_ERR("Failed to load sound WAV from path.");
            }
            else
            {
                m_pUserData.position = m_pWavBuffer;
                m_pUserData.length = m_wavLength;

                m_wavSpec.callback = mix_callback;
                m_wavSpec.userdata = &m_pUserData;

                LOG("WAV loaded and set to wav spec...");
            }
        }
    }

    Sound::~Sound()
    {
        if (m_pWavBuffer)
        {
            LOG("Freeing wav buffer for sound.");
            SDL_FreeWAV(m_pWavBuffer);
        }
    }

    void Sound::Play()
    {
    }

    void Sound::Stop()
    {
    }

    void Sound::SetVolume(float volume)
    {
        m_volume = volume;
    }
}
