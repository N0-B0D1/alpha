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

#include <fstream>
#include <string>
#include <sstream>

#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Graphics/Renderable.h"

namespace alpha
{
    Model::Model(std::vector<Renderable *> meshes)
        : m_meshes(meshes)
    { }

    std::vector<Renderable *> Model::GetRenderables()
    {
        return m_meshes;
    }

    void Model::Serialize(std::ostream & stream) const
    {
        // serializing a model amounts to serializing all meshes

        // output the number of meshes
        char * sizeBuf = new char[sizeof(unsigned int)];
        sprintf(sizeBuf, "%d", (unsigned int)m_meshes.size());
        stream.write(sizeBuf, sizeof(unsigned int));

        // serialize each mesh
        for (Renderable * r : this->m_meshes)
        {
            auto mesh = dynamic_cast<const Mesh *>(r);
            mesh->Serialize(stream);
        }
    }

    Model * Model::Deserialize(std::istream & stream)
    {
        // first data point should be the number of meshes to make
        unsigned int numMeshes;
        char * buf = new char[sizeof(unsigned int)];
        stream.read(buf, sizeof(unsigned int));
        std::stringstream str(buf);
        str >> numMeshes;

        // then deserialize a mesh up to numMeshes
        std::vector<Renderable *> meshes;
        for (unsigned int i = 0; i < numMeshes; ++i)
        {
            meshes.push_back(Mesh::Deserialize(stream));
        }

        return new Model(meshes);
    }
}
