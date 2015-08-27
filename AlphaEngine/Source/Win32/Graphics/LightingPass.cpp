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

#include "Graphics/LightingPass.h"
#include "Graphics/Camera.h"
#include "Graphics/Light.h"
#include "Assets/AssetSystem.h"
#include "Assets/Asset.h"
#include "Math/Matrix_Conversions.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    LightingPass::LightingPass()
        : m_pVertexShader(nullptr)
        , m_pInputLayout(nullptr)
        , m_pPixelShader(nullptr)
        , m_pSamplerState(nullptr)
        , m_pTextureVertexBuffer(nullptr)
        , m_pTextureIndexBuffer(nullptr)
        , m_pMatrixBuffer(nullptr)
        , m_pLightBuffer(nullptr)
    { }
    LightingPass::~LightingPass() { }

    bool LightingPass::VInitialize(AssetSystem * const pAssetSystem, ID3D11Device * pD3DDevice, int windowWidth, int windowHeight)
    {
        float left, right, top, bottom;

        left = static_cast<float>((windowWidth / 2) * -1);
        right = left + static_cast<float>(windowWidth);
        top = static_cast<float>(windowHeight / 2);
        bottom = top - static_cast<float>(windowHeight);

        LightingVertex vertices[6];
        unsigned long indices[6];
        
        // first triagle
        vertices[0].position = Vector3(left, top, 0.0f);  // Top left.
        vertices[0].texture = Vector2(0.0f, 0.0f);
        vertices[1].position = Vector3(right, bottom, 0.0f);  // Bottom right.
        vertices[1].texture = Vector2(1.0f, 1.0f);
        vertices[2].position = Vector3(left, bottom, 0.0f);  // Bottom left.
        vertices[2].texture = Vector2(0.0f, 1.0f);
        // Second triangle.
        vertices[3].position = Vector3(left, top, 0.0f);  // Top left.
        vertices[3].texture = Vector2(0.0f, 0.0f);
        vertices[4].position = Vector3(right, top, 0.0f);  // Top right.
        vertices[4].texture = Vector2(1.0f, 0.0f);
        vertices[5].position = Vector3(right, bottom, 0.0f);  // Bottom right.
        vertices[5].texture = Vector2(1.0f, 1.0f);
        

        for (int i = 0; i < 6; ++i) { indices[i] = i; }

        this->CreateBuffer(pD3DDevice, sizeof(LightingVertex) * 6, D3D11_BIND_VERTEX_BUFFER, &vertices[0], &m_pTextureVertexBuffer);
        this->CreateBuffer(pD3DDevice, sizeof(unsigned int) * 6, D3D11_BIND_INDEX_BUFFER, &indices[0], &m_pTextureIndexBuffer);

        // load vertex and pixel shaders
        auto pVSShader = pAssetSystem->GetAsset("Shaders/dx_lighting_vs.hlsl");
        auto pPSShader = pAssetSystem->GetAsset("Shaders/dx_lighting_ps.hlsl");

        // create vertex shader
        ID3DBlob* pVSBlob = nullptr;
        m_pVertexShader = this->CreateVertexShaderFromAsset(pD3DDevice, pVSShader, "VS", &pVSBlob);
        // vertex input layout
        if (pVSBlob != nullptr)
        {
            D3D11_INPUT_ELEMENT_DESC layout[] =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            };
            UINT numElements = ARRAYSIZE(layout);
            m_pInputLayout = this->CreateInputLayoutFromVSBlob(pD3DDevice, layout, numElements, &pVSBlob);
            pVSBlob->Release();
        }
        // pixel shader
        m_pPixelShader = this->CreatePixelShaderFromAsset(pD3DDevice, pPSShader, "PS");

        // Create the texture sampler state for gbuffer textures
        D3D11_SAMPLER_DESC sampler_desc;
        sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampler_desc.MipLODBias = 0.0f;
        sampler_desc.MaxAnisotropy = 1;
        sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        sampler_desc.BorderColor[0] = 0;
        sampler_desc.BorderColor[1] = 0;
        sampler_desc.BorderColor[2] = 0;
        sampler_desc.BorderColor[3] = 0;
        sampler_desc.MinLOD = 0;
        sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
        HRESULT hr = pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSamplerState);
        if (FAILED(hr)) { return false; }

        // create pixel/vertex shader constant buffers
        this->CreateBuffer(pD3DDevice, sizeof(MatrixBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr, &m_pMatrixBuffer);
        this->CreateBuffer(pD3DDevice, sizeof(LightBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr, &m_pLightBuffer);
        this->CreateBuffer(pD3DDevice, sizeof(CameraBuffer), D3D11_BIND_CONSTANT_BUFFER, nullptr, &m_pCameraBuffer);

        return true;
    }

    bool LightingPass::VShutdown()
    {
        m_pointLights.clear();
        m_directionalLights.clear();

        if (m_pCameraBuffer)
        {
            m_pCameraBuffer->Release();
            m_pCameraBuffer = nullptr;
        }
        if (m_pMatrixBuffer)
        {
            m_pMatrixBuffer->Release();
            m_pMatrixBuffer = nullptr;
        }
        if (m_pLightBuffer)
        {
            m_pLightBuffer->Release();
            m_pLightBuffer = nullptr;
        }

        if (m_pTextureVertexBuffer)
        {
            m_pTextureVertexBuffer->Release();
            m_pTextureVertexBuffer = nullptr;
        }
        if (m_pTextureIndexBuffer)
        {
            m_pTextureIndexBuffer->Release();
            m_pTextureIndexBuffer = nullptr;
        }

        if (m_pSamplerState)
        {
            m_pSamplerState->Release();
            m_pSamplerState = nullptr;
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

    void LightingPass::VSetRenderTarget(ID3D11DeviceContext * /*pDeviceContext*/)
    {

    }

    void LightingPass::VClearRenderTarget(ID3D11DeviceContext * /*pDeviceContext*/)
    {

    }

    void LightingPass::VRender(ID3D11DeviceContext * pDeviceContext, std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights)
    {
        CreateLightBufferData(lights);

        // render the window sized quad, using the geometry buffer information as the texture for the quad
        unsigned int stride = sizeof(LightingVertex);
        unsigned int offset = 0;
        pDeviceContext->IASetVertexBuffers(0, 1, &m_pTextureVertexBuffer, &stride, &offset);
        pDeviceContext->IASetIndexBuffer(m_pTextureIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // setup shader buffer data
        MatrixBuffer mb;
        mb.mWorld = DirectX::XMMatrixTranspose(MatrixToXMMATRIX(Matrix()));
        mb.mView = DirectX::XMMatrixTranspose(MatrixToXMMATRIX(pCamera->GetBaseView()));
        mb.mProjection = DirectX::XMMatrixTranspose(MatrixToXMMATRIX(pCamera->GetOrthoProjection()));
        pDeviceContext->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &mb, 0, 0);

        LightBuffer lb;
        lb.pointLight[0] = m_pointLights[0];            // point light 1
        lb.pointLight[1] = m_pointLights[1];            // point light 2
        lb.directionalLight = m_directionalLights[0];   // directional light
        pDeviceContext->UpdateSubresource(m_pLightBuffer, 0, nullptr, &lb, 0, 0);

        CameraBuffer cb;
        cb.cameraPosition = pCamera->GetView().Position();
        pDeviceContext->UpdateSubresource(m_pCameraBuffer, 0, nullptr, &cb, 0, 0);

        // set gbuffer textures on the pixel shader
        pDeviceContext->PSSetShaderResources(0, 1, &m_pShaderResourceViews[GBUFFER_POSITION]);
        pDeviceContext->PSSetShaderResources(1, 1, &m_pShaderResourceViews[GBUFFER_NORMAL]);
        pDeviceContext->PSSetShaderResources(2, 1, &m_pShaderResourceViews[GBUFFER_ALBEDOSPEC]);

        // set constant buffers
        pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);
        pDeviceContext->PSSetConstantBuffers(0, 1, &m_pLightBuffer);
        pDeviceContext->PSSetConstantBuffers(1, 1, &m_pCameraBuffer);

        // set shader layout
        pDeviceContext->IASetInputLayout(m_pInputLayout);

        // set vertex shader
        pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
        // set pixel shader
        pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

        // set sampler state on pixel shader
        pDeviceContext->PSSetSamplers(0, 1, &m_pSamplerState);

        // render the quad data to the screen
        pDeviceContext->DrawIndexed(6, 0, 0);
    }

    void LightingPass::AttachShaderResourceViews(ID3D11ShaderResourceView * pShaderResourceViews[GBUFFER_TEXTURE_COUNT])
    {
        for (int i = 0; i < GBUFFER_TEXTURE_COUNT; ++i)
        {
            m_pShaderResourceViews[i] = pShaderResourceViews[i];
        }
    }

    void LightingPass::CreateLightBufferData(const std::vector<Light *> & lights)
    {
        m_pointLights.clear();
        m_directionalLights.clear();

        for (auto light : lights)
        {
            switch (light->GetLightType())
            {
            case LightType::DIRECTIONAL:
                m_directionalLights.push_back(DirectionalLight());

                m_directionalLights[m_directionalLights.size() - 1].direction = Vector4(light->GetLightDirection(), 1.f);
                m_directionalLights[m_directionalLights.size() - 1].ambient = light->GetAmbientLight();
                m_directionalLights[m_directionalLights.size() - 1].diffuse = light->GetDiffuseLight();
                m_directionalLights[m_directionalLights.size() - 1].specular = light->GetSpecularLight();
                break;

            case LightType::POINT:
                m_pointLights.push_back(PointLight());

                m_pointLights[m_pointLights.size() - 1].position = Vector4(light->worldTransform.Position(), 1.f);
                m_pointLights[m_pointLights.size() - 1].ambient = light->GetAmbientLight();
                m_pointLights[m_pointLights.size() - 1].diffuse = light->GetDiffuseLight();
                m_pointLights[m_pointLights.size() - 1].specular = light->GetSpecularLight();
                m_pointLights[m_pointLights.size() - 1].attenuationConstant = light->GetAttenuationConstant();
                m_pointLights[m_pointLights.size() - 1].attenuationLinear = light->GetAttenuationLinear();
                m_pointLights[m_pointLights.size() - 1].attenuationQuadratic = light->GetAttenuationQuadratic();
                break;

            default:
                LOG_WARN("Unkown light type, unable to render: ", light->GetLightType());
                break;
            }
        }

        // XXX Hack, make sure at least one directional light exists, and at least 2 point lights exist
        switch (m_directionalLights.size())
        {
        case 0:
            // XXX add at least one directional light
            m_directionalLights.push_back(DirectionalLight());
            break;
        }

        switch (m_pointLights.size())
        {
        case 1:
            // XXX add second black light so the shader doesn't need to be smart
            //PointLight pl;
            m_pointLights.push_back(PointLight());
            m_pointLights[m_pointLights.size() - 1].attenuationConstant = 1.f;
            m_pointLights[m_pointLights.size() - 1].attenuationLinear = 0.045f;
            m_pointLights[m_pointLights.size() - 1].attenuationQuadratic = 0.0075f;
            break;
        case 0:
            // XXX add two lights
            for (int i = 0; i < 2; i++)
            {
                m_pointLights.push_back(PointLight());
                m_pointLights[m_pointLights.size() - 1].attenuationConstant = 1.f;
                m_pointLights[m_pointLights.size() - 1].attenuationLinear = 0.045f;
                m_pointLights[m_pointLights.size() - 1].attenuationQuadratic = 0.0075f;
            }
            break;
        }
    }
}
