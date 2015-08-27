#ifndef ALPHA_RENDERABLE_H
#define ALPHA_RENDERABLE_H

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

#include <vector>

#include <d3d11_1.h>
#include <DirectXMath.h>

#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace alpha
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
    };

    /**
     * The Renderable object represents the smallest subset of data
     * to be rendered by the rendering engine, and is platform specific.
     *
     * e.g.: A Mesh is a Renderable, and a Model contains a set of
     * Renderable Meshes.
     * A SceneNode represents a Model and therefore will produce a
     * list of Renderable Meshes when needed.
     */
    class Renderable
    {
    public:
        Renderable(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        virtual ~Renderable();

        /** vertex vertices array */
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        /** D3D11 data structures */
        ID3D11Buffer * m_pVertexBuffer;
        ID3D11Buffer * m_pIndexBuffer;
    };
}

#endif // ALPHA_RENDERABLE_H
