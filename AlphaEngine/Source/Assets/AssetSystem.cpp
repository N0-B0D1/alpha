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

namespace alpha
{
    AssetSystem::AssetSystem() : AlphaSystem(10) { }
    AssetSystem::~AssetSystem() { }

    bool AssetSystem::VInitialize()
    {
        return true;
    }

    bool AssetSystem::VShutdown()
    {
        return true;
    }

    bool AssetSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        // The AssetSystem is meant to tick very infrequenty
        // with the sole purpose of the tick being to cull
        // out the Least Recently Used assets from memory.
        return true;
    }

    std::shared_ptr<Asset> AssetSystem::GetAsset(std::string name)
    {
        auto it = m_assets.find(name);
        if (it == m_assets.end())
            return this->LoadAsset(name); // std::shared_ptr<Asset>();
        return it->second;
    }

    std::shared_ptr<Asset> AssetSystem::LoadAsset(std::string name)
    {
        // get meta data about the file, and "lazy load" the asset
        // The idea here is to get the structure set up as fast as possible
        // with information about any needed assets, then later the assets
        // will actually be loaded once they are closer to being used

        struct stat fileStats;
        if (stat(name.c_str(), &fileStats) >= 0)
        {
            auto asset = std::make_shared<Asset>(name, fileStats);
            m_assets[name] = asset;
            return asset;
        }

        return std::shared_ptr<Asset>();
    }
}