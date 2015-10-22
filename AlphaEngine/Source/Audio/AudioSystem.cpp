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

// SDL2
#include <SDL.h>
#include <SDL_audio.h>

#include "Audio/AudioSystem.h"
#include "Audio/AudioMixer.h"
#include "Audio/Sound.h"
#include "Assets/Asset.h"
#include "Toolbox/FileSystem.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    static void EmptyAudioCallback(void * userdata, unsigned char * stream, int length)
    {
        SDL_memset(stream, 0, length);

        AudioMixer * mixer = (class AudioMixer *)userdata;
        if (mixer)
        {
            mixer->Mix(stream, length);
        }
    }

    AudioSystem::AudioSystem()
        : AlphaSystem(60)
        , m_pMainChannel(nullptr)
    { }
    AudioSystem::~AudioSystem() { }

    bool AudioSystem::VInitialize()
    {
        m_pMainChannel = new AudioMixer();

        if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
            LOG_ERR("AudioSystem > Failed to initialize SDL Audio system.");
            return false;
        }

        // list audio devices
        for (int i = 0; i < SDL_GetNumAudioDevices(0); ++i)
        {
            LOG("Audio Device ", i, ": ", SDL_GetAudioDeviceName(i, 0));
        }

        // acquire the audio device and spec
        SDL_AudioSpec wanted;
        SDL_zero(wanted);
        wanted.freq = 48000;
        wanted.format = AUDIO_F32;
        wanted.channels = 2;
        wanted.samples = 4096;
        wanted.callback = EmptyAudioCallback;
        wanted.userdata = m_pMainChannel;

        m_audioDevID = SDL_OpenAudioDevice(nullptr, 0, &wanted, &m_audioSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);
        if (m_audioDevID == 0)
        {
            LOG_ERR("AudioSystem failed to obtain audio device spec! ", SDL_GetError());
        }
        else
        {
            // start playing audio
            SDL_PauseAudioDevice(m_audioDevID, 0);
        }

        return true;
    }

    bool AudioSystem::VShutdown()
    {
        // close the audio device
        if (m_audioDevID != 0)
        {
            SDL_PauseAudioDevice(m_audioDevID, 1);
            SDL_CloseAudioDevice(m_audioDevID);
        }

        SDL_Quit();

        if (m_pMainChannel)
        {
            delete m_pMainChannel;
        }

        return true;
    }

    std::weak_ptr<Sound> AudioSystem::CreateSound(std::shared_ptr<Asset> pAsset)
    {
        auto sound = std::make_shared<Sound>(pAsset, m_audioSpec);
        m_pMainChannel->Add(sound);
        return sound;
    }

    bool AudioSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        return true;
    }
}
