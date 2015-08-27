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

#include "Graphics/ARenderPass.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    ARenderPass::ARenderPass() { }
    ARenderPass::~ARenderPass() { }

    bool ARenderPass::CompileShaderFromAsset(std::shared_ptr<Asset> asset, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
    {
        HRESULT hr = S_OK;
        ID3DBlob* pErrorBlob = nullptr;
        DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
        std::vector<unsigned char> data = asset->GetData();

#ifdef ALPHA_DEBUG
        // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
        // Setting this flag improves the shader debugging experience, but still allows 
        // the shaders to be optimized and to run exactly the way they will run in 
        // the release configuration of this program.
        dwShaderFlags |= D3DCOMPILE_DEBUG;

        // Disable optimizations to further improve shader debugging
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

        hr = D3DCompile(&data[0], sizeof(char) * data.size(), nullptr, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
        if (FAILED(hr))
        {
            if (pErrorBlob)
            {
                auto bufptr = reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer());
                LOG_ERR("SHADER Failed to compile shader, ", bufptr);
                pErrorBlob->Release();
            }
            return false;
        }

        if (pErrorBlob)
        {
            pErrorBlob->Release();
        }

        return true;
    }

    ID3D11VertexShader * ARenderPass::CreateVertexShaderFromAsset(ID3D11Device * pD3DDevice, std::shared_ptr<Asset> vsAsset, const std::string & sEntryPoint, ID3DBlob** ppVSBlobOut)
    {
        // TODO: store created shader, and retrieve if used again.

        HRESULT hr = S_OK;
        //ID3DBlob* pVSBlob = nullptr;
        ID3D11VertexShader * vs = nullptr;

        // compile shader
        if (!this->CompileShaderFromAsset(vsAsset, sEntryPoint.c_str(), "vs_4_0", ppVSBlobOut))
        {
            LOG_WARN("GraphicsRenderer > Failed to compile pixel shader");
            return nullptr;
        }

        // Create the vertex shader
        hr = pD3DDevice->CreateVertexShader((*ppVSBlobOut)->GetBufferPointer(), (*ppVSBlobOut)->GetBufferSize(), nullptr, &vs);
        if (FAILED(hr))
        {
            LOG_WARN("GraphicsRenderer > Failed to create pixel shader.");
            (*ppVSBlobOut)->Release();
            return nullptr;
        }

        return vs;
    }

    ID3D11PixelShader * ARenderPass::CreatePixelShaderFromAsset(ID3D11Device * pD3DDevice, std::shared_ptr<Asset> psAsset, const std::string & sEntryPoint)
    {
        // TODO: store created shader, and retrieve if used again.

        HRESULT hr = S_OK;
        ID3DBlob* pPSBlob = nullptr;
        ID3D11PixelShader * ps = nullptr;

        // Compile the pixel shader
        pPSBlob = nullptr;
        if (!this->CompileShaderFromAsset(psAsset, sEntryPoint.c_str(), "ps_4_0", &pPSBlob))
        {
            LOG_WARN("GraphicsRenderer > Failed to compile pixel shader");
            return nullptr;
        }

        // Create the pixel shader
        hr = pD3DDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &ps);
        pPSBlob->Release();
        if (FAILED(hr))
        {
            LOG_WARN("GraphicsRenderer > Failed to create pixel shader.");
            //pPSBlob->Release();
            return nullptr;
        }

        return ps;
    }

    ID3D11InputLayout * ARenderPass::CreateInputLayoutFromVSBlob(ID3D11Device * pD3DDevice, D3D11_INPUT_ELEMENT_DESC * pLayout, UINT numLayoutElements, ID3DBlob ** const pVSBlob)
    {
        // TODO: store created vertex shader with layout data, and retrieve if used again.

        HRESULT hr = S_OK;
        ID3D11InputLayout * vsLayout = nullptr;

        // Create the input layout
        hr = pD3DDevice->CreateInputLayout(pLayout, numLayoutElements, (*pVSBlob)->GetBufferPointer(), (*pVSBlob)->GetBufferSize(), &vsLayout);
        //pVSBlob->Release();
        if (FAILED(hr))
        {
            return nullptr;
        }

        return vsLayout;
    }

    void ARenderPass::CreateBuffer(ID3D11Device * pD3DDevice, unsigned int byte_width, unsigned int bind_flags, const void * object_memory, ID3D11Buffer ** buffer)
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
            hr = pD3DDevice->CreateBuffer(&bd, &InitData, buffer);
        }
        else
        {
            hr = pD3DDevice->CreateBuffer(&bd, nullptr, buffer);
        }
    }
}