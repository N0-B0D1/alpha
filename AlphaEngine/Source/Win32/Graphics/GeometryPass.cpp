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

#include <DirectXColors.h>

#include "Graphics/GeometryPass.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Renderable.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Math/Matrix_Conversions.h"

namespace alpha
{
    GeometryPass::GeometryPass()
        : m_pVertexShader(nullptr)
        , m_pInputLayout(nullptr)
        , m_pPixelShader(nullptr)
        , m_pMatrixBuffer(nullptr)
        , m_pDeferredBuffer(nullptr)
    {
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            m_pRenderTargetTextures[i] = nullptr;
            m_pRenderTargetViews[i] = nullptr;
            m_pShaderResourceViews[i] = nullptr;
        }

        m_pDepthStencilBuffer = nullptr;
        m_pDepthStencilView = nullptr;
    }
    GeometryPass::~GeometryPass() { }

    bool GeometryPass::VInitialize(AssetSystem * const pAssetSystem, ID3D11Device * pD3DDevice, int windowWidth, int windowHeight)
    {
        auto pVSShader = pAssetSystem->GetAsset("Shaders/dx_deferred_vs.hlsl");
        auto pPSShader = pAssetSystem->GetAsset("Shaders/dx_deferred_ps.hlsl");

        // create vertex shader
        ID3DBlob* pVSBlob = nullptr;
        m_pVertexShader = this->CreateVertexShaderFromAsset(pD3DDevice, pVSShader, "VS", &pVSBlob);
        // vertex input layout
        if (pVSBlob != nullptr)
        {
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            UINT numElements = ARRAYSIZE(layout);
            m_pInputLayout = this->CreateInputLayoutFromVSBlob(pD3DDevice, layout, numElements, &pVSBlob);
            pVSBlob->Release();
        }
        // pixel shader
        m_pPixelShader = this->CreatePixelShaderFromAsset(pD3DDevice, pPSShader, "PS");

        HRESULT hr;
        D3D11_TEXTURE2D_DESC texture_desc;
        D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
        D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
        D3D11_TEXTURE2D_DESC depth_buffer_desc;
        D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;

        // prepare render target texture description
        ZeroMemory(&texture_desc, sizeof(texture_desc));
        texture_desc.Width = windowWidth;
        texture_desc.Height = windowHeight;
        texture_desc.MipLevels = 1;
        texture_desc.ArraySize = 1;
        texture_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.Usage = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags = 0;
        texture_desc.MiscFlags = 0;

        // create render target texutres for each texture buffer
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            hr = pD3DDevice->CreateTexture2D(&texture_desc, nullptr, &m_pRenderTargetTextures[i]);
            if (FAILED(hr)) { return false; }
        }

        // create render target view description
        render_target_view_desc.Format = texture_desc.Format;
        render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        render_target_view_desc.Texture2D.MipSlice = 0;

        // create render target views for each texture buffer
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            hr = pD3DDevice->CreateRenderTargetView(m_pRenderTargetTextures[i], &render_target_view_desc, &m_pRenderTargetViews[i]);
            if (FAILED(hr)) { return false; }
        }

        // setup shader resource view description
        shader_resource_view_desc.Format = texture_desc.Format;
        shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
        shader_resource_view_desc.Texture2D.MipLevels = 1;

        // create shader resource views for each texture buffer
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            hr = pD3DDevice->CreateShaderResourceView(m_pRenderTargetTextures[i], &shader_resource_view_desc, &m_pShaderResourceViews[i]);
            if (FAILED(hr)) { return false; }
        }

        // set up depth buffer description
        ZeroMemory(&depth_buffer_desc, sizeof(depth_buffer_desc));
        depth_buffer_desc.Width = windowWidth;
        depth_buffer_desc.Height = windowHeight;
        depth_buffer_desc.MipLevels = 1;
        depth_buffer_desc.ArraySize = 1;
        depth_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_buffer_desc.SampleDesc.Count = 1;
        depth_buffer_desc.SampleDesc.Quality = 0;
        depth_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depth_buffer_desc.CPUAccessFlags = 0;
        depth_buffer_desc.MiscFlags = 0;

        // create depth buffer texture
        hr = pD3DDevice->CreateTexture2D(&depth_buffer_desc, nullptr, &m_pDepthStencilBuffer);
        if (FAILED(hr)) { return false; }

        // set up depth stencil view desciption
        ZeroMemory(&depth_stencil_view_desc, sizeof(depth_stencil_view_desc));
        depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depth_stencil_view_desc.Texture2D.MipSlice = 0;

        // create the depth stencil view
        hr = pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depth_stencil_view_desc, &m_pDepthStencilView);
        if (FAILED(hr)) { return false; }

        // finally setup the viewport for rendering
        m_viewport.Width = (float)windowWidth;
        m_viewport.Height = (float)windowHeight;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0.0f;
        m_viewport.TopLeftY = 0.0f;
        
        // create pixel/vertex shader constant buffers
        this->CreateBuffer(pD3DDevice, sizeof(MatrixBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr, &m_pMatrixBuffer);
        this->CreateBuffer(pD3DDevice, sizeof(DeferredBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr, &m_pDeferredBuffer);

        return true;
    }

    bool GeometryPass::VShutdown()
    {
        if (m_pDeferredBuffer)
        {
            m_pDeferredBuffer->Release();
            m_pDeferredBuffer = nullptr;
        }

        if (m_pMatrixBuffer)
        {
            m_pMatrixBuffer->Release();
            m_pMatrixBuffer = nullptr;
        }

        if (m_pDepthStencilView)
        {
            m_pDepthStencilView->Release();
            m_pDepthStencilView = nullptr;
        }

        if (m_pDepthStencilBuffer)
        {
            m_pDepthStencilBuffer->Release();
            m_pDepthStencilBuffer = nullptr;
        }

        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; i++)
        {
            if (m_pShaderResourceViews[i])
            {
                m_pShaderResourceViews[i]->Release();
                m_pShaderResourceViews[i] = nullptr;
            }

            if (m_pRenderTargetViews[i])
            {
                m_pRenderTargetViews[i]->Release();
                m_pRenderTargetViews[i] = nullptr;
            }

            if (m_pRenderTargetTextures[i])
            {
                m_pRenderTargetTextures[i]->Release();
                m_pRenderTargetTextures[i] = nullptr;
            }
        }

        if (m_pInputLayout)
        {
            m_pInputLayout->Release();
            m_pInputLayout = nullptr;
        }
        if (m_pVertexShader)
        {
            m_pVertexShader->Release();
            m_pVertexShader = nullptr;
        }
        if (m_pPixelShader)
        {
            m_pPixelShader->Release();
            m_pPixelShader = nullptr;
        }

        return true;
    }

    void GeometryPass::VSetRenderTarget(ID3D11DeviceContext * pDeviceContext)
    {
        pDeviceContext->OMSetRenderTargets(GBUFFER_TEXTURE_COUNT, m_pRenderTargetViews, m_pDepthStencilView);
        pDeviceContext->RSSetViewports(1, &m_viewport);
    }

    void GeometryPass::VClearRenderTarget(ID3D11DeviceContext * pDeviceContext)
    {
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            pDeviceContext->ClearRenderTargetView(m_pRenderTargetViews[i], DirectX::Colors::Black);
        }
        pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void GeometryPass::VRender(ID3D11DeviceContext * pDeviceContext, std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights)
    {
        for (RenderSet * render_set : renderables)
        {
            auto renderables = render_set->GetRenderables();
            auto material = render_set->material.lock();

            // update constant buffers
            MatrixBuffer mb;
            mb.mWorld = DirectX::XMMatrixTranspose(MatrixToXMMATRIX(render_set->worldTransform));
            mb.mView = DirectX::XMMatrixTranspose(MatrixToXMMATRIX(pCamera->GetView()));
            mb.mProjection = DirectX::XMMatrixTranspose(MatrixToXMMATRIX(pCamera->GetProjection()));
            pDeviceContext->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &mb, 0, 0);

            DeferredBuffer cb;
            cb.diffuse = material->GetDiffuse();
            cb.specular = material->GetSpecularCoefficient();
            pDeviceContext->UpdateSubresource(m_pDeferredBuffer, 0, nullptr, &cb, 0, 0);

            // apply constant buffers to shaders
            pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
            pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
            pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
            pDeviceContext->PSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
            pDeviceContext->PSSetConstantBuffers(1, 1, &m_pDeferredBuffer);

            for (auto renderable : renderables)
            {
                // set input layout
                pDeviceContext->IASetInputLayout(m_pInputLayout);

                // Set vertex buffer
                UINT stride = sizeof(Vertex);
                UINT offset = 0;
                pDeviceContext->IASetVertexBuffers(0, 1, &renderable->m_pVertexBuffer, &stride, &offset);
                // Set index buffer
                pDeviceContext->IASetIndexBuffer(renderable->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
                // Set primitive topology
                pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                // finally draw the indexed object
                pDeviceContext->DrawIndexed(renderable->indices.size(), 0, 0);
            }
        }
    }

    ID3D11ShaderResourceView ** GeometryPass::GetShaderResourceViews()
    {
        return m_pShaderResourceViews;
    }
}
