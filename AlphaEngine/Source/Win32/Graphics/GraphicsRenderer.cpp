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

#include <DirectXColors.h>

#include "Graphics/GraphicsRenderer.h"

namespace alpha
{
    HINSTANCE               m_hInstance = nullptr;
    HWND                    m_hWnd = nullptr;
    D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
    D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_1;
    ID3D11Device*           m_pd3dDevice = nullptr;
    ID3D11Device1*          m_pd3dDevice1 = nullptr;
    ID3D11DeviceContext*    m_pImmediateContext = nullptr;
    ID3D11DeviceContext1*   m_pImmediateContext1 = nullptr;
    IDXGISwapChain*         m_pSwapChain = nullptr;
    IDXGISwapChain1*        m_pSwapChain1 = nullptr;
    ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

    GraphicsRenderer::GraphicsRenderer() { }
    GraphicsRenderer::~GraphicsRenderer() { }

    bool GraphicsRenderer::Initialize()
    {
        /** TODO pass in hInstance and nCmdShow options */
        m_hInstance = GetModuleHandle(NULL);
        if (FAILED(this->InitializeWindow())) 
        {
            return false;
        }
        if (FAILED(this->InitializeDevice()))
        {
            return false;
        }
        return true;
    }

    bool GraphicsRenderer::Shutdown()
    {
        this->CleanupDevice();
        return true;
    }

    bool GraphicsRenderer::Update(double /*currentTime*/, double /*elapsedTime*/)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT)
        {
            //SendMessage(m_hWnd, WM_CLOSE, 0, 0);
            return false;
        }
        return true;
    }

    void GraphicsRenderer::Render()
    {
        // do something cool
        m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::MidnightBlue);
        m_pSwapChain->Present(0, 0);
    }

    LRESULT CALLBACK GraphicsRenderer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC hdc;

        switch (message)
        {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        return 0;
    }

    HRESULT GraphicsRenderer::InitializeWindow()
    {
        // Register class
        WNDCLASSEX wcex = {};
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = m_hInstance;
        wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // IDI_TUTORIAL1);
        //wcex.hIcon = nullptr;
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = "ALPHAClass";
        wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); // LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
        //wcex.hIconSm = nullptr;
        DWORD err = RegisterClassEx(&wcex);
        if (!err)
        {
            DWORD err = GetLastError();
            UNREFERENCED_PARAMETER(err);
            return E_FAIL;
        }

        // Create window
        //g_hInst = hInstance;
        RECT rc = { 0, 0, 800, 600 };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
        m_hWnd = CreateWindow("ALPHAClass", "ALPHA Engine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInstance, nullptr);
        if (!m_hWnd)
        {
            return E_FAIL;
        }

        ShowWindow(m_hWnd, 10); // nCmdShow);

        return S_OK;
    }

    HRESULT GraphicsRenderer::InitializeDevice()
    {
        HRESULT hr = S_OK;

        RECT rc;
        GetClientRect(m_hWnd, &rc);
        UINT width = rc.right - rc.left;
        UINT height = rc.bottom - rc.top;

        UINT createDeviceFlags = 0;
#ifdef _DEBUG
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
                break;
        }
        if (FAILED(hr))
            return hr;

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
        if (FAILED(hr))
            return hr;

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

            hr = dxgiFactory2->CreateSwapChainForHwnd(m_pd3dDevice, m_hWnd, &sd, nullptr, nullptr, &m_pSwapChain1);
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
            sd.OutputWindow = m_hWnd;
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
            sd.Windowed = TRUE;

            hr = dxgiFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain);
        }

        dxgiFactory->Release();

        if (FAILED(hr))
            return hr;

        // Create a render target view
        ID3D11Texture2D* pBackBuffer = nullptr;
        hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
        if (FAILED(hr))
            return hr;

        hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
        pBackBuffer->Release();
        if (FAILED(hr))
            return hr;

        m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

        // Setup the viewport
        D3D11_VIEWPORT vp;
        vp.Width = (FLOAT)width;
        vp.Height = (FLOAT)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        m_pImmediateContext->RSSetViewports(1, &vp);

        return S_OK;
    }

    void GraphicsRenderer::CleanupDevice()
    {
        CloseWindow(m_hWnd);

        if (m_pImmediateContext) m_pImmediateContext->ClearState();

        if (m_pRenderTargetView) m_pRenderTargetView->Release();
        if (m_pSwapChain1) m_pSwapChain1->Release();
        if (m_pSwapChain) m_pSwapChain->Release();
        if (m_pImmediateContext1) m_pImmediateContext1->Release();
        if (m_pImmediateContext) m_pImmediateContext->Release();
        if (m_pd3dDevice1) m_pd3dDevice1->Release();
        if (m_pd3dDevice) m_pd3dDevice->Release();

        m_hInstance = nullptr;
        m_hWnd = nullptr;
        //m_driverType = D3D_DRIVER_TYPE_NULL;
        //m_featureLevel = D3D_FEATURE_LEVEL_11_1;
        m_pd3dDevice = nullptr;
        m_pd3dDevice1 = nullptr;
        m_pImmediateContext = nullptr;
        m_pImmediateContext1 = nullptr;
        m_pSwapChain = nullptr;
        m_pSwapChain1 = nullptr;
        m_pRenderTargetView = nullptr;
    }
}