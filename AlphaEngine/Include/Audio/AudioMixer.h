#ifndef ALPHA_AUDIO_MIXER_H
#define ALPHA_AUDIO_MIXER_H

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

namespace alpha
{
    class Sound;

    class AudioMixer
    {
    public:
        ~AudioMixer();

        /** Update all sound states */
        void Update();
        /** Mix all sounds in this mixer channel */
        void Mix(unsigned char * stream, int length);

        /** Add a sound to this mixer */
        void Add(std::shared_ptr<Sound> pSound);

    private:
        /** Manage a list of sounds, so they can be properly updated and disposed of. */
        std::vector<std::shared_ptr<Sound>> m_sounds;
    };
}

#endif // ALPHA_AUDIO_MIXER_H
