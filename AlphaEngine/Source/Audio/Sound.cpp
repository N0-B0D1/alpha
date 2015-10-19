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
    Sound::Sound(std::weak_ptr<Asset> pAsset, SDL_AudioSpec audio_spec)
        : m_pAsset(pAsset)
        , m_pWavBuffer(nullptr)
        , m_position(nullptr)
        , m_state(SoundState::STOP)
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
                // convert audio if needed
                if (m_wavSpec.freq != audio_spec.freq ||
                    m_wavSpec.format != audio_spec.format ||
                    m_wavSpec.channels != audio_spec.channels)
                {
                    SDL_AudioCVT cvt;
                    SDL_BuildAudioCVT(&cvt,
                                      m_wavSpec.format, m_wavSpec.channels, m_wavSpec.freq,
                                      audio_spec.format, audio_spec.channels, audio_spec.freq);
                    
                    cvt.len = m_wavLength;
                    cvt.buf = (unsigned char *)SDL_malloc(cvt.len * cvt.len_mult);
                    memcpy(cvt.buf, m_pWavBuffer, m_wavLength);

                    SDL_ConvertAudio(&cvt);

                    m_wavLength = cvt.len_cvt;
                    SDL_FreeWAV(m_pWavBuffer);
                    m_pWavBuffer = (unsigned char *)SDL_malloc(cvt.len_cvt);
                    memcpy(m_pWavBuffer, cvt.buf, cvt.len_cvt);
                }
            }
        }
    }

    Sound::~Sound()
    {
        if (m_pWavBuffer)
        {
            SDL_FreeWAV(m_pWavBuffer);
        }
    }

    void Sound::Play()
    {
        m_qStateChange.Push(SoundState::PLAY);
    }

    void Sound::Stop()
    {
        m_qStateChange.Push(SoundState::STOP);
    }

    void Sound::Pause()
    {
        m_qStateChange.Push(SoundState::PAUSE);
    }

    void Sound::SetVolume(float volume)
    {
        m_volume = volume;
    }

    void Sound::Mix(unsigned char * stream, int length)
    {
        int len = (length > (int)m_length ? m_length : length);
        // change state if requested
        m_qStateChange.TryPop(m_state);

        switch (m_state)
        {
            case SoundState::STOP:
                m_length = m_wavLength;
                m_position = m_pWavBuffer;
                m_state = SoundState::STOPPED;

            case SoundState::STOPPED:
                break;

            case SoundState::PLAY:
                m_state = SoundState::PLAYING;

            case SoundState::PLAYING:
                // make sure we still have audio to play, if none left, then we
                // can stop playing.
                if (m_length == 0)
                {
                    m_state = SoundState::STOP;
                    break;
                }

                SDL_MixAudioFormat(stream, m_position, AUDIO_F32, len, SDL_MIX_MAXVOLUME / 2);

                m_position += len;
                m_length -= len;
                break;

            case SoundState::PAUSE:
                m_state = SoundState::PAUSED;

            case SoundState::PAUSED:
            default:
                break;
        }
    }
}
