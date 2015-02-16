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

#include "Graphics/RenderData.h"

namespace alpha
{
    RenderData::RenderData(std::string psEntryPoint /*= "PS"*/)
        : m_psEntryPoint(psEntryPoint)
        , m_pVertexShader(nullptr)
        , m_pInputLayout(nullptr)
        , m_pPixelShader(nullptr)
        , m_pVertexBuffer(nullptr)
        , m_pIndexBuffer(nullptr)
        , m_pConstantBuffer(nullptr)
    {
        // set verticies for a cube
        m_vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f) });
        m_vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f) });

        m_vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f) });
        m_vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f) });

        m_vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) });
        m_vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) });
        m_vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, 0.0f, 0.0f) });
        m_vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f), Vector3(-1.0f, 0.0f, 0.0f) });

        m_vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) });
        m_vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f), Vector3(1.0f, 0.0f, 0.0f) });

        m_vertices.push_back({ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) });
        m_vertices.push_back({ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) });
        m_vertices.push_back({ Vector3(1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) });
        m_vertices.push_back({ Vector3(-1.0f, 1.0f, -1.0f), Vector3(0.0f, 0.0f, -1.0f) });

        m_vertices.push_back({ Vector3(-1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f) });
        m_vertices.push_back({ Vector3(1.0f, -1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f) });
        m_vertices.push_back({ Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f) });
        m_vertices.push_back({ Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f) });

        // set indices for the cube
        m_indices.push_back(3);
        m_indices.push_back(1);
        m_indices.push_back(0);
        m_indices.push_back(2);
        m_indices.push_back(1);
        m_indices.push_back(3);

        m_indices.push_back(6);
        m_indices.push_back(4);
        m_indices.push_back(5);
        m_indices.push_back(7);
        m_indices.push_back(4);
        m_indices.push_back(6);

        m_indices.push_back(11);
        m_indices.push_back(9);
        m_indices.push_back(8);
        m_indices.push_back(10);
        m_indices.push_back(9);
        m_indices.push_back(11);

        m_indices.push_back(14);
        m_indices.push_back(12);
        m_indices.push_back(13);
        m_indices.push_back(15);
        m_indices.push_back(12);
        m_indices.push_back(14);

        m_indices.push_back(19);
        m_indices.push_back(17);
        m_indices.push_back(16);
        m_indices.push_back(18);
        m_indices.push_back(17);
        m_indices.push_back(19);

        m_indices.push_back(22);
        m_indices.push_back(20);
        m_indices.push_back(21);
        m_indices.push_back(23);
        m_indices.push_back(20);
        m_indices.push_back(22);
    }

    RenderData::~RenderData()
    {
        if (m_pConstantBuffer) m_pConstantBuffer->Release();
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();
        if (m_pInputLayout) m_pInputLayout->Release();
        if (m_pVertexShader) m_pVertexShader->Release();
        if (m_pPixelShader) m_pPixelShader->Release();
    }

    void RenderData::SetPosition(const Vector3 & pos)
    {
        //XMMATRIX m = XMMatrixTranslationFromVector(XMLoadFloat3(&m_pSceneComponent->GetPosition()));
        XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
        XMMATRIX world = translation * XMLoadFloat4x4(&this->m_world);
        XMStoreFloat4x4(&this->m_world, world);
    }

    void RenderData::SetScale(const Vector3 & scale)
    {
        //Vector3 scale = m_pSceneComponent->GetScale();
        //XMMATRIX mscale = XMMatrixScaling(scale.x, scale.y, scale.z);
        XMMATRIX scale_transform = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
        XMMATRIX world = scale_transform * XMLoadFloat4x4(&this->m_world);
        XMStoreFloat4x4(&this->m_world, world);
    }
}