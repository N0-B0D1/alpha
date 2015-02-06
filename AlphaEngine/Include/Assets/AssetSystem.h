#ifndef ASSET_SYSTEM_H
#define ASSET_SYSTEM_H

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

#include <map>
#include <string>
#include <memory>

#include "AlphaSystem.h"

#include "Asset.h"

namespace alpha
{
    class AssetSystem : public AlphaSystem
    {
    public:
        AssetSystem();
        virtual ~AssetSystem();

        virtual bool VInitialize();
        virtual bool VShutdown();

        /** Retrieve or create an asset that tracks a specific asset file in the system */
        std::shared_ptr<Asset> GetAsset(const char * name);

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);

        std::shared_ptr <Asset> LoadAsset(const char * name);

        typedef std::map<std::string, std::shared_ptr<Asset>> AssetMap;
        AssetMap m_assets;

        char * m_contentPath;
    };
}

#endif // ASSET_SYSTEM_H
