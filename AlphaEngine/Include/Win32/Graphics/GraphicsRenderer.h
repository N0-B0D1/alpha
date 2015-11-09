#ifndef RENDERER_H
#define RENDERER_H

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

#include <string>
#include <memory>

#include "Graphics/IRenderer.h"

namespace alpha
{
    class RenderWindow;
    class RenderSet;
    class Light;
    class Asset;
    class AssetSystem;
    class Camera;
    class GeometryPass;
    class LightingPass;

    class GraphicsRenderer : public IRenderer
    {
    public:
        GraphicsRenderer();
        virtual ~GraphicsRenderer();

        bool Initialize(AssetSystem * const pAssets, int windowWidth, int windowHeight);
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        /** PreRender takes a list of data that will be rendered, and preps it rendering. */
        void PreRender(RenderSet * renderSet);
        void Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights);

    private:
        HRESULT InitializeDevice();
        void CleanupDevice();

        /** Windows hwnd manager */
        RenderWindow *m_pWindow;

        /** Create a d3d buffer object */
        void CreateBuffer(unsigned int byte_width, unsigned int bind_flags, const void * object_memory, ID3D11Buffer ** buffer);

        /** Geometry render pass for render to GBuffer */
        GeometryPass * m_pGeometryPass;
        /** Lighting render pass for final render to screen */
        LightingPass * m_pLightingPass;

        /** DirectX 11 variables */
        D3D_DRIVER_TYPE m_driverType;
        D3D_FEATURE_LEVEL m_featureLevel;
        ID3D11Device* m_pd3dDevice;
        ID3D11Device1* m_pd3dDevice1;
        ID3D11DeviceContext* m_pImmediateContext;
        ID3D11DeviceContext1* m_pImmediateContext1;
        IDXGISwapChain* m_pSwapChain;
        IDXGISwapChain1* m_pSwapChain1;
        ID3D11RenderTargetView* m_pRenderTargetView;
        ID3D11ShaderResourceView * m_pNULLShaderResourceView;
        ID3D11Texture2D* m_pDepthStencil;
        ID3D11DepthStencilState * m_pDepthStencilState;
        ID3D11DepthStencilState * m_pDepthDisabledStencilState;
        ID3D11DepthStencilView* m_pDepthStencilView;
        ID3D11RasterizerState* m_pWireFrame;
        ID3D11RasterizerState* m_pRasterizerState;
        D3D11_VIEWPORT m_viewport;
    };
}

#endif // RENDERER_H
