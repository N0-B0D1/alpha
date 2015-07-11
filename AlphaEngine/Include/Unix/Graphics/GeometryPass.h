#ifndef ALPHA_GEOMETRY_PASS_H
#define ALPHA_GEOMETRY_PASS_H

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

#include "Graphics/ARenderPass.h"

namespace alpha
{
    class GeometryPass : public ARenderPass
    {
    public:
        GeometryPass();
        virtual ~GeometryPass();

        virtual bool VInitialize(AssetSystem * const pAssetSystem, int windowWidth, int windowHeight);
        virtual bool VShutdown();

    private:
        std::shared_ptr<Asset> m_vsDefaultShader;
        std::shared_ptr<Asset> m_psDefaultShader;
    };
}

#endif // ALPHA_GEOMETRY_PASS_H
