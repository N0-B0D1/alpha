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

#include <GL/gl.h>

#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace alpha
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
    };

    typedef struct PointLight
    {
        Vector4 position;

        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;

        float constant;
        float linear;
        float quadratic;
    } PointLight;

    typedef struct DirectionalLight
    {
        Vector4 direction;

        Vector4 ambient;
        Vector4 diffuse;
        Vector4 specular;
    } DirectionalLight;

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

        /** platform agnostic list of vertices */
        std::vector<Vertex> vertices;
        /** platform agnostic list of indices */
        std::vector<unsigned int> indices;

        /** OpenGL specific variables */
        GLuint m_vertexBuffer;
        GLuint m_vertexAttribute;
        GLuint m_elementBuffer;
    };
}

#endif // ALPHA_RENDERABLE_H
