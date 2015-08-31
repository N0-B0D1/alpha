#ifndef ALPHA_LIGHTING_PASS_H
#define ALPHA_LIGHTING_PASS_H

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

#include <vector>

#include "Graphics/ARenderPass.h"

namespace alpha
{
    class Light;

    struct PointLight;
    struct DirectionalLight;

    class LightingPass : public ARenderPass
    {
    public:
        LightingPass();
        virtual ~LightingPass();

        virtual bool VInitialize(AssetSystem * const pAssetSystem, int windowWidth, int windowHeight);
        virtual bool VShutdown();

        virtual void VRender(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> render_sets, std::vector<Light *> lights);

        /** Attach gbuffer textures to lighting pass, for final render */
        void AttachGBufferTexture(GBUFFER_TYPE texture_type, GLuint texture);

    private:
        /** Convert light objects into type data structs for rendering. */
        void CreateLightBufferData(const std::vector<Light *> & lights);

        void RenderPointLights(std::shared_ptr<Camera> pCamera);
        void RenderDirectionalLights();

        /** light data for rendering */
        std::vector<PointLight> m_pointLights;
        std::vector<DirectionalLight> m_directionalLights;

        /** Lighting shader assets */
        std::shared_ptr<Asset> m_vsShader;
        std::shared_ptr<Asset> m_psPLShader;
        std::shared_ptr<Asset> m_psDLShader;

        /** Lighting shader program */
        GLuint m_PLShaderProgram;
        GLuint m_DLShaderProgram;

        /** GBuffer texture array, shared from the geometry pass */
        GLuint m_gBufferTextures[GBUFFER_TEXTURE_COUNT];

        /** Quad vertex objects */
        GLuint m_vboQuad;
        GLuint m_vaoQuad;

        /** Lighting volume sphere objects */
        std::shared_ptr<Asset> m_pSphere;
    };
}

#endif // ALPHA_LIGHTING_PASS_H
