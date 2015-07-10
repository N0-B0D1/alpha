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

#include <Windows.h>
#include <d3d11_1.h>

#include "Graphics/ARenderPass.h"
#include "Math/Vector4.h"

namespace alpha
{
    class GeometryPass : public ARenderPass
    {
        typedef struct MatrixBuffer
        {
            DirectX::XMMATRIX mWorld;
            DirectX::XMMATRIX mView;
            DirectX::XMMATRIX mProjection;
        } MatrixBuffer;

        typedef struct DeferredBuffer
        {
            Vector4 diffuse;
            float specular;
            float _spacer1;
            float _spacer2;
            float _spacer3; // buffer size has to be a multiple of 16 bytes to be properly byte aligned
        } DeferredBuffer;

    public:
        GeometryPass();
        virtual ~GeometryPass();

        virtual bool VInitialize(AssetSystem * const pAssetSystem, ID3D11Device * pD3DDevice, int windowWidth, int windowHeight);
        virtual bool VShutdown();

        virtual void VSetRenderTarget(ID3D11DeviceContext * pDeviceContext);
        virtual void VClearRenderTarget(ID3D11DeviceContext * pDeviceContext);

        virtual void VRender(ID3D11DeviceContext * pDeviceContext, std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights);

        ID3D11ShaderResourceView ** GetShaderResourceViews();

    private:
        /** Compiled vertex shader */
        ID3D11VertexShader * m_pVertexShader;
        /** Input layout for shader */
        ID3D11InputLayout * m_pInputLayout;
        /** Compiled pixel shader */
        ID3D11PixelShader * m_pPixelShader;

        ID3D11Texture2D * m_pRenderTargetTextures[GBUFFER_TEXTURE_COUNT];
        ID3D11RenderTargetView * m_pRenderTargetViews[GBUFFER_TEXTURE_COUNT];
        ID3D11ShaderResourceView * m_pShaderResourceViews[GBUFFER_TEXTURE_COUNT];
        ID3D11Texture2D * m_pDepthStencilBuffer;
        ID3D11DepthStencilView * m_pDepthStencilView;
        D3D11_VIEWPORT m_viewport;

        /** Shader buffer bind objects */
        ID3D11Buffer * m_pMatrixBuffer;
        ID3D11Buffer * m_pDeferredBuffer;
    };
}

#endif // ALPHA_GEOMETRY_PASS_H
