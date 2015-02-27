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
    {
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
