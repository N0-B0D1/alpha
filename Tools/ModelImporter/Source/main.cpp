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
#include <string>
#include <iostream>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Graphics/ModelFile.h"

/**
 * Create a Mesh for each aiMesh
 */
alpha::Mesh processMesh(aiMesh * mesh, const aiScene * /*scene*/)
{
    //printf("parsing a mesh.\r\n");

    std::vector<alpha::Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        alpha::Vertex v;

        // process vertex positions
        //printf("%f, %f, %f,\r\n", mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // load mesh vertex position
        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;

        // load mesh normal coordinates
        if (mesh->mNormals)
        {
            v.normal.x = mesh->mNormals[i].x;
            v.normal.y = mesh->mNormals[i].y;
            v.normal.z = mesh->mNormals[i].z;
        }
        else
        {
            v.normal.x = 0.f;
            v.normal.y = 0.f;
            v.normal.z = 0.f;
        }

        vertices.push_back(v);
    }

    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int k = 0; k < face.mNumIndices; ++k)
        {
            //printf("%u, ", face.mIndices[k]);
            indices.push_back(face.mIndices[k]);
        }
        //printf("\r\n");
    }
    
    // process material
    
    //printf("done parsing mesh\r\n");

    return alpha::Mesh(vertices, indices);
}

/**
 * Process all meshes at this node, an recurse through child nodes.
 */
void processNode(aiNode * node, const aiScene * scene, std::vector<alpha::Mesh> & meshes)
{
    //printf("parsing a node\r\n");

    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene, meshes);
    }

    //printf("done parsing node.\r\n");
}

void ConvertModel(const std::string & source, const std::string & target)
{
    printf("Importing model: %s\r\n", source.c_str());

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(source.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        printf("Error !! %s\r\n", importer.GetErrorString());
    }
    else
    {
        //printf("Loading model ...\r\n");
        std::vector<alpha::Mesh> meshes;
        processNode(scene->mRootNode, scene, meshes);
        alpha::Model mod(meshes);

        std::ofstream out;
        out.open(target, std::ios::out | std::ios::binary);
        if (out.is_open())
        {
            alpha::SerializeModel(&mod, out);
            out.close();
        }

        //// TEST
        /*
        std::ifstream infile;
        infile.open(target, std::ios::in | std::ios::binary);
        if (infile.is_open())
        {
            alpha::Model * mod = alpha::DeserializeModel(infile);
            if (mod != nullptr)
            {
                delete mod;
            }
        }
        */
        ////
    }
}

/**
 * Import the model path provided by the user, and process the root node.
 */
int main(int argc, char * argv[])
{
    if (argc > 2)
    {
        std::string source = argv[1];
        std::string target = argv[2];

        //// test
        //std::string source = "E:/Models/cube.fbx";
        //std::string target = "E:/Models/output/cube.am";

        ConvertModel(source, target);
    }
    else
    {
        printf("AssimpModelImporter Usage:");
        printf("AssimpModelImport {source-input-path} {target-output-path}");
    }

    return 0;
}
