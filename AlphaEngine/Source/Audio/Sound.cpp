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

#include "Audio/Sound.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    Sound::Sound(FMOD::System * pSystem, std::weak_ptr<Asset> pAsset)
        : m_pAsset(pAsset)
        , m_pSystem(pSystem)
        , m_pSound(nullptr)
        , m_pChannel(nullptr)
        , m_volume(0.5f)
    {
        // create the sound so it is preped for use
        if (auto asset = pAsset.lock())
        {
            std::vector<unsigned char> data = asset->GetData();
            if (data.size() > 0)
            {
                char * buffer = reinterpret_cast<char *>(&data[0]);

                FMOD_CREATESOUNDEXINFO exinfo;
                memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));
                exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
                exinfo.length = data.size();

                FMOD_RESULT result = m_pSystem->createSound(buffer, FMOD_OPENMEMORY, &exinfo, &m_pSound);
                if (result != FMOD_OK)
                {
                    LOG_ERR("AudioSystem > FMOD failed to create sound from asset data.");
                }
            }
        }
    }

    Sound::~Sound()
    {
        // release the sound
        // do not need to do anything with the channel
        // fmod will handle the rest
        if (m_pSound)
        {
            m_pSound->release();
        }
    }

    void Sound::Play()
    {
        FMOD_RESULT result = m_pSystem->playSound(m_pSound, 0, false, &m_pChannel);
        if (result != FMOD_OK)
        {
            LOG_ERR("Sound > Failed to play sound.");
        }
        result = m_pChannel->setVolume(m_volume);
    }

    void Sound::Stop()
    {
        if (m_pChannel != nullptr)
        {
            m_pChannel->stop();
        }
    }

    void Sound::SetVolume(float volume)
    {
        m_volume = volume;
        if (m_pChannel != nullptr)
        {
            m_pChannel->setVolume(m_volume);
        }
    }
}