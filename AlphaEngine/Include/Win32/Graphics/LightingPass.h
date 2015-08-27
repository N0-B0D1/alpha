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

#include <d3d11_1.h>
#include <DirectXMath.h>

#include "Graphics/ARenderPass.h"
#include "Math/Vector4.h"
#include "Math/Vector2.h"

namespace alpha
{
    class LightingPass : public ARenderPass
    {
        struct LightingVertex
        {
            Vector3 position;
            Vector2 texture;
        };

        typedef struct MatrixBuffer
        {
            DirectX::XMMATRIX mWorld;
            DirectX::XMMATRIX mView;
            DirectX::XMMATRIX mProjection;
        } MatrixBuffer;

        typedef struct DirectionalLight
        {
            Vector4 direction;

            Vector4 ambient;
            Vector4 diffuse;
            Vector4 specular;
        } DirectionalLight;

        typedef struct PointLight
        {
            Vector4 position;

            Vector4 ambient;
            Vector4 diffuse;
            Vector4 specular;

            float attenuationConstant;
            float attenuationLinear;
            float attenuationQuadratic;
            float _spacer;
        } PointLight;

        typedef struct LightBuffer
        {
            PointLight pointLight[2];
            DirectionalLight directionalLight;
        } ConstantBuffer;

        typedef struct CameraBuffer
        {
            Vector3 cameraPosition;
            float _spacer;
        } CameraBuffer;

    public:
        LightingPass();
        virtual ~LightingPass();

        virtual bool VInitialize(AssetSystem * const pAssetSystem, ID3D11Device * pD3DDevice, int windowWidth, int windowHeight);
        virtual bool VShutdown();

        virtual void VSetRenderTarget(ID3D11DeviceContext * pDeviceContext);
        virtual void VClearRenderTarget(ID3D11DeviceContext * pDeviceContext);

        virtual void VRender(ID3D11DeviceContext * pDeviceContext, std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights);

        /** Attach GBuffer Shader Resource Views before before rendering to the screen. */
        void AttachShaderResourceViews(ID3D11ShaderResourceView * pShaderResourceViews[GBUFFER_TEXTURE_COUNT]);

    private:
        /** Create point or direction light buffer data objects for each light in the scene. */
        void CreateLightBufferData(const std::vector<Light *> & lights);

        /** Compiled vertex shader */
        ID3D11VertexShader * m_pVertexShader;
        /** Input layout for shader */
        ID3D11InputLayout * m_pInputLayout;
        /** Compiled pixel shader */
        ID3D11PixelShader * m_pPixelShader;

        /** GBuffer shader texture data */
        ID3D11ShaderResourceView * m_pShaderResourceViews[GBUFFER_TEXTURE_COUNT];

        /** Sample state for deferred geometry textures */
        ID3D11SamplerState * m_pSamplerState;

        ID3D11Buffer * m_pTextureVertexBuffer;
        ID3D11Buffer * m_pTextureIndexBuffer;

        /** Shader buffer data */
        ID3D11Buffer * m_pMatrixBuffer;
        ID3D11Buffer * m_pLightBuffer;
        ID3D11Buffer * m_pCameraBuffer;

        /** Simplified light information, XXX to be removed once deferred lighting complete ... */
        std::vector<PointLight> m_pointLights;
        std::vector<DirectionalLight> m_directionalLights;
    };
}

#endif // ALPHA_LIGHTING_PASS_H
