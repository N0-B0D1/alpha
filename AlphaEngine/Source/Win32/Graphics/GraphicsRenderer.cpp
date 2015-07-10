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
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <vector>

#include "Graphics/GraphicsRenderer.h"
#include "Graphics/GraphicsWindow.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Renderable.h"
#include "Graphics/Light.h"
#include "Graphics/Camera.h"
#include "Graphics/GeometryPass.h"
#include "Graphics/LightingPass.h"
#include "Math/Matrix.h"
#include "Math/Matrix_Conversions.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

using namespace DirectX;

namespace alpha
{
    GraphicsRenderer::GraphicsRenderer()
        : m_pGeometryPass(nullptr)
        , m_pLightingPass(nullptr)
        , m_driverType(D3D_DRIVER_TYPE_NULL)
        , m_featureLevel(D3D_FEATURE_LEVEL_11_1)
        , m_pd3dDevice(nullptr)
        , m_pd3dDevice1(nullptr)
        , m_pImmediateContext(nullptr)
        , m_pImmediateContext1(nullptr)
        , m_pSwapChain(nullptr)
        , m_pSwapChain1(nullptr)
        , m_pRenderTargetView(nullptr)
        , m_pDepthStencil(nullptr)
        , m_pDepthStencilView(nullptr)
        , m_pWireFrame(nullptr)
    { }
    GraphicsRenderer::~GraphicsRenderer() { }

    bool GraphicsRenderer::Initialize(AssetSystem * const pAssets, int windowWidth, int windowHeight)
    {
        m_pWindow = new GraphicsWindow();
        if (!m_pWindow->Initialize(windowWidth, windowHeight))
        {
            LOG_ERR("Failed to create and open game window.");
            return false;
        }
        if (FAILED(this->InitializeDevice()))
        {
            LOG_ERR("Failed to initialize graphics device.");
            return false;
        }

        // create the render passes to run against the scene
        m_pGeometryPass = new GeometryPass();
        if (!m_pGeometryPass->VInitialize(pAssets, m_pd3dDevice, windowWidth, windowHeight))
        {
            LOG_ERR("Failed to initialize graphics render pass: Geometry pass");
            return false;
        }

        m_pLightingPass = new LightingPass();
        if (!m_pLightingPass->VInitialize(pAssets, m_pd3dDevice, windowWidth, windowHeight))
        {
            LOG_ERR("Failed to initialize graphics render pass: Lighting pass");
            return false;
        }

        // attach gbuffer shader views to lighting pass
        m_pLightingPass->AttachShaderResourceViews(m_pGeometryPass->GetShaderResourceViews());

        return true;
    }

    bool GraphicsRenderer::Shutdown()
    {
        // shutdown each render pass
        if (m_pGeometryPass)
        {
            if (!m_pGeometryPass->VShutdown())
            {
                LOG_ERR("Failed to shutdown graphics render pass: Geometry");
            }
            delete m_pGeometryPass;
        }

        if (m_pLightingPass)
        {
            if (!m_pLightingPass->VShutdown())
            {
                LOG_ERR("Failed to shutdown graphics render pass: Lighting");
            }
            delete m_pLightingPass;
        }

        // close game window
        if (!m_pWindow->Shutdown())
        {
            LOG_ERR("Failed to close game window.");
        }
        delete m_pWindow;

        // cleanup graphics device
        this->CleanupDevice();

        return true;
    }

    bool GraphicsRenderer::Update(double currentTime, double elapsedTime)
    {
        // let any windows messages through
        return m_pWindow->Update(currentTime, elapsedTime);
    }

    void GraphicsRenderer::PreRender(RenderSet * renderSet)
    {
        auto renderables = renderSet->GetRenderables();

        for (Renderable * renderable : renderables)
        {
            // vertex buffer
            if (renderable->m_pVertexBuffer == nullptr || renderable->m_pIndexBuffer == nullptr)
            {
                this->CreateBuffer(sizeof(Vertex) * renderable->vertices.size(), D3D11_BIND_VERTEX_BUFFER, &renderable->vertices[0], &renderable->m_pVertexBuffer);
                this->CreateBuffer(sizeof(unsigned int) * renderable->indices.size(), D3D11_BIND_INDEX_BUFFER, &renderable->indices[0], &renderable->m_pIndexBuffer);
            }
        }
    }

    void GraphicsRenderer::Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights)
    {
        // Clear PS Shader resources, so the textures are un-bound and can be used as a render target again
        m_pImmediateContext->PSSetShaderResources(0, 1, &m_pNULLShaderResourceView);
        m_pImmediateContext->PSSetShaderResources(1, 1, &m_pNULLShaderResourceView);
        m_pImmediateContext->PSSetShaderResources(2, 1, &m_pNULLShaderResourceView);

        // render geometry to multiple render target (MRT) textures
        m_pGeometryPass->VSetRenderTarget(m_pImmediateContext);
        m_pGeometryPass->VClearRenderTarget(m_pImmediateContext);
        m_pGeometryPass->VRender(m_pImmediateContext, pCamera, renderables, lights);

        // switch back to render to back buffer
        m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
        m_pImmediateContext->RSSetViewports(1, &m_viewport);

        // clear back buffer target
        m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);
        m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // turn off z-buffer for 2d rendering
        m_pImmediateContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);

        // render gbuffer textures with lighting pass
        m_pLightingPass->VRender(m_pImmediateContext, pCamera, renderables, lights);

        // turn z-buffer back on for next iteration
        m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

        // swap buffer that we just drew to.
        m_pSwapChain->Present(0, 0);
    }

    HRESULT GraphicsRenderer::InitializeDevice()
    {
        HRESULT hr = S_OK;
        HWND hwnd = m_pWindow->GetHWND();

        RECT rc;
        GetClientRect(hwnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        UINT createDeviceFlags = 0;
        #ifdef ALPHA_DEBUG
                createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        #endif

        D3D_DRIVER_TYPE driverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE,
        };
        UINT numDriverTypes = ARRAYSIZE(driverTypes);

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
        };
        UINT numFeatureLevels = ARRAYSIZE(featureLevels);

        for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
        {
            m_driverType = driverTypes[driverTypeIndex];
            hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);

            if (hr == E_INVALIDARG)
            {
                // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
                hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
            }

            if (SUCCEEDED(hr))
            {
                break;
            }
        }
        if (FAILED(hr)) { return hr; }

        // Obtain DXGI factory from device (since we used nullptr for pAdapter above)
        IDXGIFactory1* dxgiFactory = nullptr;
        {
            IDXGIDevice* dxgiDevice = nullptr;
            hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
            if (SUCCEEDED(hr))
            {
                IDXGIAdapter* adapter = nullptr;
                hr = dxgiDevice->GetAdapter(&adapter);
                if (SUCCEEDED(hr))
                {
                    hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
                    adapter->Release();
                }
                dxgiDevice->Release();
            }
        }
        if (FAILED(hr)) { return hr; }

        // Create swap chain
        IDXGIFactory2* dxgiFactory2 = nullptr;
        hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
        if (dxgiFactory2)
        {
            // DirectX 11.1 or later
            hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pd3dDevice1));
            if (SUCCEEDED(hr))
            {
                (void)m_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
            }

            DXGI_SWAP_CHAIN_DESC1 sd;
            ZeroMemory(&sd, sizeof(sd));
            sd.Width = width;
            sd.Height = height;
            sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.BufferCount = 1;

            hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3dDevice, hwnd, &sd, nullptr, nullptr, &m_pSwapChain1);
            if (SUCCEEDED(hr))
            {
                hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
            }

            dxgiFactory2->Release();
        }
        else
        {
            // DirectX 11.0 systems
            DXGI_SWAP_CHAIN_DESC sd;
            ZeroMemory(&sd, sizeof(sd));
            sd.BufferCount = 1;
            sd.BufferDesc.Width = width;
            sd.BufferDesc.Height = height;
            sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            sd.BufferDesc.RefreshRate.Numerator = 60;
            sd.BufferDesc.RefreshRate.Denominator = 1;
            sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            sd.OutputWindow = hwnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;

            hr = dxgiFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
        }

        dxgiFactory->Release();
        if (FAILED(hr)) { return hr; }

        // Create a render target view
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (FAILED(hr)) { return hr; }

        hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
        pBackBuffer->Release();
        if (FAILED(hr)) { return hr; }

        // Create depth stencil texture
        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory(&descDepth, sizeof(descDepth));
        descDepth.Width = width;
        descDepth.Height = height;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
        if (FAILED(hr)) { return hr; }

        // Create the normal depth buffer for 3d rendering
        D3D11_DEPTH_STENCIL_DESC descStencil;
        ZeroMemory(&descStencil, sizeof(descStencil));
        descStencil.DepthEnable = true;
        descStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        descStencil.DepthFunc = D3D11_COMPARISON_LESS;
        descStencil.StencilEnable = true;
        descStencil.StencilReadMask = 0xFF;
        descStencil.StencilWriteMask = 0xFF;
        // Stencil operations if pixel is front-facing.
        descStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        descStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        descStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        descStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        // Stencil operations if pixel is back-facing.
        descStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        descStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        descStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        descStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        hr = m_pd3dDevice->CreateDepthStencilState(&descStencil, &m_pDepthStencilState);
        if (FAILED(hr)) { return hr; }

        m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

        // create the 2d depth stencil
        ZeroMemory(&descStencil, sizeof(descStencil));
        descStencil.DepthEnable = false;
        descStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        descStencil.DepthFunc = D3D11_COMPARISON_LESS;
        descStencil.StencilEnable = true;
        descStencil.StencilReadMask = 0xFF;
        descStencil.StencilWriteMask = 0xFF;
        // Stencil operations if pixel is front-facing.
        descStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        descStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        descStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        descStencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        // Stencil operations if pixel is back-facing.
        descStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        descStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        descStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        descStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        hr = m_pd3dDevice->CreateDepthStencilState(&descStencil, &m_pDepthDisabledStencilState);
        if (FAILED(hr)) { return hr; }

        // Create the depth stencil view
        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory(&descDSV, sizeof(descDSV));
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
        if (FAILED(hr)) { return hr; }

        m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

        // create wireframe rasterizer
        D3D11_RASTERIZER_DESC rasterDesc;
        ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
        rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
        rasterDesc.CullMode = D3D11_CULL_NONE;
        hr = m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pWireFrame);
        if (FAILED(hr)) { return hr; }

        // create default rasterizer state
        rasterDesc.AntialiasedLineEnable = false;
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_BACK;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.ScissorEnable = false;
        rasterDesc.SlopeScaledDepthBias = 0.0f;
        hr = m_pd3dDevice->CreateRasterizerState(&rasterDesc, &m_pRasterizerState);
        if (FAILED(hr)) { return hr; }

        m_pImmediateContext->RSSetState(m_pRasterizerState);
        //m_pImmediateContext->RSSetState(m_pWireFrame);

        // Setup the viewport
        m_viewport.Width = (FLOAT)width;
        m_viewport.Height = (FLOAT)height;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        m_viewport.TopLeftX = 0;
        m_viewport.TopLeftY = 0;
        m_pImmediateContext->RSSetViewports(1, &m_viewport);

        // make a NULL shader resource view to clear the PS Shader Resource before use
        m_pNULLShaderResourceView = { nullptr };

        return S_OK;
    }

    void GraphicsRenderer::CleanupDevice()
    {
        if (m_pImmediateContext) m_pImmediateContext->ClearState();

        if (m_pWireFrame) m_pWireFrame->Release();
        if (m_pRasterizerState) m_pRasterizerState->Release();
        if (m_pDepthDisabledStencilState) m_pDepthDisabledStencilState->Release();
        if (m_pDepthStencilState) m_pDepthStencilState->Release();
        if (m_pDepthStencil) m_pDepthStencil->Release();
        if (m_pDepthStencilView) m_pDepthStencilView->Release();
        if (m_pRenderTargetView) m_pRenderTargetView->Release();
        if (m_pSwapChain1) m_pSwapChain1->Release();
        if (m_pSwapChain) m_pSwapChain->Release();
        if (m_pImmediateContext1) m_pImmediateContext1->Release();
        if (m_pImmediateContext) m_pImmediateContext->Release();
        if (m_pd3dDevice1) m_pd3dDevice1->Release();
        if (m_pd3dDevice) m_pd3dDevice->Release();
    }

    void GraphicsRenderer::CreateBuffer(unsigned int byte_width, unsigned int bind_flags, const void * object_memory, ID3D11Buffer ** buffer)
    {
        HRESULT hr = S_OK;
        D3D11_BUFFER_DESC bd;
        D3D11_SUBRESOURCE_DATA InitData;

        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0;
        bd.ByteWidth = byte_width;
        bd.BindFlags = bind_flags;

        if (object_memory) 
        {
            ZeroMemory(&InitData, sizeof(InitData));
            InitData.pSysMem = object_memory;
            hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, buffer);
        }
        else
        {
            hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, buffer);
        }
    }
}
