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

#include "Audio/AudioSystem.h"
#include "Audio/Sound.h"
#include "Assets/Asset.h"
#include "Toolbox/FileSystem.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    AudioSystem::AudioSystem()
        : AlphaSystem(60)
        , m_pSystem(nullptr)
    { }
    AudioSystem::~AudioSystem() { }

    bool AudioSystem::VInitialize()
    {
        FMOD_RESULT result;
        unsigned int version;
        void *pExtraDriverVersion = 0;

        // create fmod system
        result = FMOD::System_Create(&m_pSystem);
        if (result != FMOD_OK)
        {
            LOG_ERR("AudioSystem > Failed to initialize FMOD system.");
            return false;
        }

        // get the fmod version, and make sure lib and headers match
        result = m_pSystem->getVersion(&version);
        if (result != FMOD_OK)
        {
            LOG_ERR("AudioSystem > An error occured while attempting to get the FMOD system version.");
            return false;
        }
        if (version < FMOD_VERSION)
        {
            LOG_ERR("AudioSystem > FMOD lib version ", version, " does not match the header version ", FMOD_VERSION);
            return false;
        }

        result = m_pSystem->init(32, FMOD_INIT_NORMAL, pExtraDriverVersion);
        if (result != FMOD_OK)
        {
            LOG_ERR("AudioSystem > FMOD system init failed.");
            return false;
        }

        return true;
    }

    bool AudioSystem::VShutdown()
    {
        unsigned int result;

        // release handles to all sound objects
        // forces destructor to be called for each
        // and all sounds get properly released.
        m_sounds.clear();

        if (m_pSystem)
        {
            result = m_pSystem->close();
            if (result != FMOD_OK)
            {
                LOG_ERR("AudioSystem > An error occured while closing the FMOD system.");
            }
            result = m_pSystem->release();
            if (result != FMOD_OK)
            {
                LOG_ERR("AudioSystem > And error occured while releasing the FMOD system.");
            }
        }

        return true;
    }

    std::weak_ptr<Sound> AudioSystem::CreateSound(std::shared_ptr<Asset> pAsset)
    {
        auto sound = std::make_shared<Sound>(m_pSystem, pAsset);
        m_sounds.push_back(sound);
        return sound;
    }

    bool AudioSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        FMOD_RESULT result = m_pSystem->update();
        if (result != FMOD_OK)
        {
            // XXX for now log the error, however we may need to do something more drastic ...
            LOG_ERR("AudioSystem > FMOD system failed to update.");
        }
        return true;
    }
}