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
   static void mix_callback(void *userdata, unsigned char *stream, int length)
    {
        SoundData *data = (struct SoundData*)userdata;

        switch (data->state)
        {
            case SoundState::PLAYING:
                // in the mix callback, we only care if the sound bit is
                // actually in a playing state, all other states are handled in
                // the Sound class.
                //
                // make sure we still have audio to play, if none left, then we
                // can stop playing.
                if (data->length == 0)
                {
                    data->state = SoundState::STOP;
                    break;
                }

                // mix the audio if all conditions pass
                length = (length > (int)data->length ? data->length : length);
                LOG_WARN("MIXING AUDIO: ", data->length);
                SDL_MixAudio(stream, data->position, length, SDL_MIX_MAXVOLUME);

                data->position += length;
                data->length -= length;

                break;

            default:
                break;
        }
    }

    SoundData::SoundData()
        : wav_buffer(nullptr)
        , position(nullptr)
        , state(SoundState::STOP)
    { }

    Sound::Sound(std::weak_ptr<Asset> pAsset)
        : m_pAsset(pAsset)
    {
        // create the sound so it is preped for use
        if (auto asset = pAsset.lock())
        {
            std::string path = asset->GetPath();

            m_pUserData = new SoundData();

            if (SDL_LoadWAV(path.c_str(), &m_pUserData->wav_spec, &m_pUserData->wav_buffer, &m_pUserData->wav_length) == nullptr)
            {
                LOG_ERR("Failed to load sound WAV from path.");
            }
            else
            {
                m_pUserData->wav_spec.callback = mix_callback;
                m_pUserData->wav_spec.userdata = m_pUserData;

                LOG("WAV loaded and set to wav spec...");
            }
        }
    }

    Sound::~Sound()
    {
        if (m_pUserData)
        {
            if (m_pUserData->wav_buffer)
            {
                LOG("Freeing wav buffer for sound.");
                SDL_FreeWAV(m_pUserData->wav_buffer);
            }
            delete m_pUserData;
        }
    }

    void Sound::Update()
    {
        switch (m_pUserData->state)
        {
            case SoundState::STOP:
                // a stop request occured, reset the data length and set to
                // stopped
                m_pUserData->length = m_pUserData->wav_length;
                m_pUserData->state = SoundState::STOPPED;
                break;

            case SoundState::PLAY:
                // switch to playing state
                m_pUserData->state = SoundState::PLAYING;
                break;

            case SoundState::PAUSE:
                // switch to paused state, leaving the data length at its
                // current position, so that it can be resumed.
                m_pUserData->state = SoundState::PAUSED;
                break;

            case SoundState::PLAYING:
                // special case, if the sound is playing but no data is left,
                // the mix callback with not do anything, and the sound needs
                // to have its state changed to STOPPED
                if (m_pUserData->length == 0)
                {
                    m_pUserData->state = SoundState::STOP;
                }

            case SoundState::STOPPED:
            case SoundState::PAUSED:
            default:
                break;
        }
    }

    void Sound::Play()
    {
        if (m_pUserData)
        {
            // if already playing or transition from play to playing, then do
            // nothing, otherwise set the play.
            switch (m_pUserData->state)
            {
                case SoundState::STOP:
                case SoundState::STOPPED:
                case SoundState::PAUSE:
                case SoundState::PAUSED:
                    m_pUserData->state = SoundState::PLAY;
                    break;

                case SoundState::PLAY:
                case SoundState::PLAYING:
                default:
                    break;
            }
        }
    }

    void Sound::Stop()
    {
        if (m_pUserData)
        {
            // only stop if not already stopping or stopped
            switch (m_pUserData->state)
            {
                case SoundState::PLAY:
                case SoundState::PLAYING:
                case SoundState::PAUSE:
                case SoundState::PAUSED:
                    m_pUserData->state = SoundState::STOP;
                    break;

                case SoundState::STOP:
                case SoundState::STOPPED:
                default:
                    break;
            }
        }
    }

    void Sound::Pause()
    {
        if (m_pUserData)
        {
            // only pause if the sound is actually playing
            switch (m_pUserData->state)
            {
                case SoundState::PLAY:
                case SoundState::PLAYING:
                    m_pUserData->state = SoundState::PAUSE;
                    break;

                case SoundState::PAUSE:
                case SoundState::PAUSED:
                case SoundState::STOP:
                case SoundState::STOPPED:
                default:
                    break;
            }
        }
    }

    void Sound::SetVolume(float volume)
    {
        m_volume = volume;
    }
}
