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
#include <string>
#include <vector>

namespace alpha
{
    class AssetSystem;
    class Asset;
    class Camera;
    class Light;
    class RenderSet;

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

        virtual bool VInitialize(AssetSystem * const pAssetSystem, ID3D11Device * pD3DDevice, int windowWidth, int windowHeight) = 0;
        virtual bool VShutdown() = 0;

        virtual void VSetRenderTarget(ID3D11DeviceContext * pDeviceContext) = 0;
        virtual void VClearRenderTarget(ID3D11DeviceContext * pDeviceContext) = 0;

        virtual void VRender(ID3D11DeviceContext * pDeviceContext, std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights) = 0;

    protected:
        /** Compile a shader from a given asset file, which is presumably an hlsl file. */
        bool CompileShaderFromAsset(std::shared_ptr<Asset> asset, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
        /** Creates and returns a Vertex Shader from the given asset, also outputs blob data which can be passed into Input Layout creation */
        ID3D11VertexShader * CreateVertexShaderFromAsset(ID3D11Device * pD3DDevice, std::shared_ptr<Asset> vsAsset, const std::string & sEntryPoint, ID3DBlob** ppVSBlobOut);
        /** Creates and returns a Pixel Shader from the given asset */
        ID3D11PixelShader * CreatePixelShaderFromAsset(ID3D11Device * pD3DDevice, std::shared_ptr<Asset> psAsset, const std::string & sEntryPoint);
        /** Creates and returns a Vertx Shader Layout from the given blobl data */
        ID3D11InputLayout * CreateInputLayoutFromVSBlob(ID3D11Device * pD3DDevice, D3D11_INPUT_ELEMENT_DESC * pLayout, UINT numLayoutElements, ID3DBlob ** const pVSBlob);
        /** Create a d3d buffer object */
        void CreateBuffer(ID3D11Device * pD3DDevice, unsigned int byte_width, unsigned int bind_flags, const void * object_memory, ID3D11Buffer ** buffer);

    };
}

#endif // ALPHA_ARENDER_PASS_H