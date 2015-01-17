/**
Copyright 2014 Jason R. Wendlandt

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

#include <sys/stat.h>

#include "Assets/AssetSystem.h"
#include "Toolbox/FileSystem.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    AssetSystem::AssetSystem() : AlphaSystem(10) { }
    AssetSystem::~AssetSystem() { }

    bool AssetSystem::VInitialize()
    {
        char * base = OSGetBaseDirectory();
        m_contentPath = OSJoinPath(base, "Content");

        LOG("Opened AssetSystem for directory: ", m_contentPath);
        free(base);
        return true;
    }

    bool AssetSystem::VShutdown()
    {
        free(m_contentPath);
        return true;
    }

    bool AssetSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        // The AssetSystem is meant to tick very infrequenty
        // with the sole purpose of the tick being to cull
        // out the Least Recently Used assets from memory.
        return true;
    }

    std::shared_ptr<Asset> AssetSystem::GetAsset(const char * name)
    {
        auto it = m_assets.find(name);
        if (it == m_assets.end())
        {
            LOG("Asset not found in memory '", name, "', creating new asset.");
            return this->LoadAsset(name); // std::shared_ptr<Asset>();
        }
        return it->second;
    }

    std::shared_ptr<Asset> AssetSystem::LoadAsset(const char * name)
    {
        // get meta data about the file, and "lazy load" the asset
        // The idea here is to get the structure set up as fast as possible
        // with information about any needed assets, then later the assets
        // will actually be loaded once they are closer to being used

        char * path = OSJoinPath(m_contentPath, name);

        struct stat fileStats;
        if (stat(path, &fileStats) >= 0)
        {
            LOG("File found, creating asset.");
            auto asset = std::make_shared<Asset>(path, fileStats);
            m_assets[name] = asset;
            return asset;
        }

        LOG_WARN("File not found in content directory.");
        return std::shared_ptr<Asset>();
    }
}
