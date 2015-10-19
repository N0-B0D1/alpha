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

#include "Audio/AudioMixer.h"
#include "Audio/Sound.h"

namespace alpha
{
    AudioMixer::~AudioMixer()
    {
        // release handles to all sound objects
        // forces destructor to be called for each
        // and all sounds get properly released.
        m_sounds.clear();
    }

    void AudioMixer::Update()
    {
        // update all sounds, allowing them to handle state transitions
        // properly.
        for (auto sound : m_sounds)
        {
            sound->Update();
        }
    }

    void AudioMixer::Add(std::shared_ptr<Sound> pSound)
    {
        m_sounds.push_back(pSound);
    }

    void AudioMixer::Mix(unsigned char * stream, int length)
    {
        for (auto sound : m_sounds)
        {
            sound->Mix(stream, length);
        }
    }
}
