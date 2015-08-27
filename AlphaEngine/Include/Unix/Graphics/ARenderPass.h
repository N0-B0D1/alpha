#ifndef ALPHA_ARENDER_PASS_H
#define ALPHA_ARENDER_PASS_H

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

#include <GL/glx.h>

namespace alpha
{
    class AssetSystem;
    class Asset;
    class Camera;
    class RenderSet;
    class Light;

    class ARenderPass
    {
    public:
        enum GBUFFER_TYPE
        {
            GBUFFER_POSITION,
            GBUFFER_NORMAL,
            GBUFFER_ALBEDOSPEC,
            GBUFFER_TEXTURE_COUNT,
        };

        ARenderPass();
        virtual ~ARenderPass();

        virtual bool VInitialize(AssetSystem * const pAssetSystem, int windowWidth, int windowHeight) = 0;
        virtual bool VShutdown() = 0;

        virtual void VRender(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> render_sets, std::vector<Light *> lights) = 0;

    protected:
        /** Create a shader program from a vertex shader and a pixel shader asset. */
        GLuint CreateShaderProgram(std::shared_ptr<Asset> pVSAsset, std::shared_ptr<Asset> pPSAsset);
        /** Compile a shader from an asset of the given type */
        GLuint CreateShaderFromAsset(std::shared_ptr<Asset> pAsset, GLenum shaderType);
    };
}

#endif // ALPHA_ARENDER_PASS_H
