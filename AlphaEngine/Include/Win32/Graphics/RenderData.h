#ifndef ALPHA_RENDER_DATA_H
#define ALPHA_RENDER_DATA_H

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
#include <vector>

#include <d3d11_1.h>
#include <DirectXMath.h>

#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace alpha
{
    struct Matrix;

    // define some D3D helper structures, TODO make not static
    typedef struct SimpleVertex
    {
        Vector3 Pos;
        Vector3 Normal;
    } SimpleVertex;
    typedef struct ConstantBuffer
    {
        DirectX::XMMATRIX mWorld;
        DirectX::XMMATRIX mView;
        DirectX::XMMATRIX mProjection;
        Vector4 vLightDir[2];
        Vector4 vLightColor[2];
        Vector4 ambient;
        Vector4 vOutputColor;
    } ConstantBuffer;

    /**
     * \brief RenderData is a containing that gets passed along the Render Pipeline
     *
     * As the RenderData object is passed along the render pipeline data is preped
     * for rendering, and once it gets the render stage, the renderer can simply grab
     * the various data and render it immediately.
     */
    class RenderData
    {
    public:
        explicit RenderData(std::string psEntryPoint = "PS");
        virtual ~RenderData();

        /** The pixel shader entry point */
        std::string m_psEntryPoint;

        // setup variables
        //
        /** objects world transform matrix */
        Matrix m_world;

        /** vertex vertices array */
        std::vector<SimpleVertex> m_vertices;
        std::vector<unsigned short> m_indices;

        /** D3D11 data structures */
        ID3D11VertexShader * m_pVertexShader;
        ID3D11InputLayout * m_pInputLayout;
        ID3D11PixelShader * m_pPixelShader;
        ID3D11Buffer * m_pVertexBuffer;
        ID3D11Buffer * m_pIndexBuffer;
        ID3D11Buffer * m_pConstantBuffer;
    };
}

#endif // ALPHA_RENDER_DATA_H