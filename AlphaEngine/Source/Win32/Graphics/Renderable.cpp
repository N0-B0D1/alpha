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

#include "Graphics/Renderable.h"

namespace alpha
{
    Renderable::Renderable(std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
        : vertices(vertexList)
        , indices(indexList)
        , m_pVertexShader(nullptr)
        , m_pInputLayout(nullptr)
        , m_pPixelShader(nullptr)
        , m_pVertexBuffer(nullptr)
        , m_pIndexBuffer(nullptr)
        , m_pMatrixBuffer(nullptr)
        , m_pConstantBuffer(nullptr)
    { }
    Renderable::~Renderable()
    {
        if (m_pConstantBuffer) m_pConstantBuffer->Release();
        if (m_pMatrixBuffer) m_pMatrixBuffer->Release();
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
        if (m_pIndexBuffer) m_pIndexBuffer->Release();
        if (m_pInputLayout) m_pInputLayout->Release();
        if (m_pVertexShader) m_pVertexShader->Release();
        if (m_pPixelShader) m_pPixelShader->Release();
    }
}
