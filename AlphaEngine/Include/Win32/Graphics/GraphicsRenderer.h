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
    class GraphicsWindow;
    class RenderSet;
    class Light;
    class Asset;
    class AssetSystem;
    class Camera;

    struct PointLight;
    struct DirectionalLight;

    class GraphicsRenderer : public IRenderer
    {
    public:
        GraphicsRenderer();
        virtual ~GraphicsRenderer();

        bool Initialize(std::shared_ptr<AssetSystem> pAssets);
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        /** PreRender takes a list of data that will be rendered, and preps it rendering. */
        void PreRender(RenderSet * renderSet);
        void Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights);

    private:
        HRESULT InitializeDevice();
        void CleanupDevice();

        GraphicsWindow *m_pWindow;

        std::shared_ptr<Asset> m_vsDefaultShader;
        std::shared_ptr<Asset> m_psDefaultShader;
        std::shared_ptr<Asset> m_vsLightShader;
        std::shared_ptr<Asset> m_psLightShader;

        void PrepRenderable(RenderSet * renderSet);
        void RenderRenderable(std::shared_ptr<Camera> pCamera, RenderSet * renderSet);

        /** Compile a shader from a given asset file, which is presumably an hlsl file. */
        bool CompileShaderFromAsset(std::shared_ptr<Asset> asset, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
        /** Creates and returns a Vertex Shader from the given asset, also outputs blob data which can be passed into Input Layout creation */
        ID3D11VertexShader * CreateVertexShaderFromAsset(std::shared_ptr<Asset> vsAsset, const std::string & sEntryPoint, ID3DBlob** ppVSBlobOut);
        /** Creates and returns a Pixel Shader from the given asset */
        ID3D11PixelShader * CreatePixelShaderFromAsset(std::shared_ptr<Asset> psAsset, const std::string & sEntryPoint);
        /** Creates and returns a Vertx Shader Layout from the given blobl data */
        ID3D11InputLayout * CreateInputLayoutFromVSBlob(ID3DBlob ** const pVSBlob);
        /** Create a d3d buffer object */
        void CreateBuffer(unsigned int byte_width, unsigned int bind_flags, const void * object_memory, ID3D11Buffer ** buffer);

        /** Create point or direction light buffer data objects for each light in the scene. */
        void CreateLightBufferData(const std::vector<Light *> & lights);

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
        ID3D11Texture2D* m_pDepthStencil;
        ID3D11DepthStencilView* m_pDepthStencilView;
        ID3D11RasterizerState* m_pWireFrame;

        std::vector<PointLight> m_pointLights;
        std::vector<DirectionalLight> m_directionalLights;
    };
}

#endif // RENDERER_H
