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

#include <GL/gl.h>

#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace alpha
{
    struct Matrix;

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

        void SetPosition(const Vector3 & pos);
        void SetScale(const Vector3 & scale);

        /** The pixel shader entry point */
        std::string m_psEntryPoint;

        // setup variables
        //
        /** objects world transform matrix */
        Matrix m_world;

        // opengl variables
        std::vector<GLfloat> m_vertices;
        std::vector<GLuint> m_indices;

        GLuint m_vertexBuffer;
        GLuint m_vertexAttribute;
        GLuint m_elementBuffer;
        GLuint m_shaderProgram;
    };
}

#endif // ALPHA_RENDER_DATA_H
