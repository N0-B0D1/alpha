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
#include "Math/Vector3.h"

namespace alpha
{
    RenderData::RenderData(std::string psEntryPoint /*= "PS"*/)
        : m_psEntryPoint(psEntryPoint)
        , m_vertexBuffer(0)
        , m_vertexAttribute(0)
        , m_elementBuffer(0)
        , m_shaderProgram(0)
    {
        /*
        // default to s square for now
        m_vertices = {
             0.5f,  0.5f, 0.0f,  // Top Right
             0.5f, -0.5f, 0.0f,  // Bottom Right
            -0.5f, -0.5f, 0.0f,  // Bottom Left
            -0.5f,  0.5f, 0.0f   // Top Left 
        };
        m_indices = {    // Note that we start from 0!
            0, 1, 3,            // First Triangle
            1, 2, 3             // Second Triangle
        };
        */

        // set verticies for a cube
        m_vertices = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
        };

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

    }

    void RenderData::SetPosition(const Vector3 & /*pos*/)
    {

    }

    void RenderData::SetScale(const Vector3 & /*scale*/)
    {

    }
}
