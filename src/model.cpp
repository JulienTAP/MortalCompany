#include "model.h"
#include <iostream>

Model::Model(std::string const &path, Shader &shader) : shader(shader)
{
    loadModel(path);
}


void Model::loadModel(std::string const &path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    std::cout << "Model loaded successfully: " << path << std::endl;
    std::cout << "Meshes found: " << scene->mNumMeshes << std::endl;
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;

            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
        }
        else
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        
        vertex.color = glm::vec3(1.0f, 1.0f, 1.0f); // Default color

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, scene, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, scene, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures, shader);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, const aiScene *scene, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].type, str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {
            // If texture path starts with '*', it's an embedded texture
            if (const aiTexture* ai_texture = scene->GetEmbeddedTexture(str.C_Str()))
            {
                // Load embedded texture
                std::cout << "Loading embedded texture: " << str.C_Str() << std::endl;
                Texture texture(reinterpret_cast<const unsigned char*>(ai_texture->pcData), ai_texture->mWidth, typeName.c_str(), textures_loaded.size());
                textures.push_back(texture);
            }
            else
            {
                // It's an external texture file
                std::string filename = std::string(str.C_Str());
                filename = directory + '/' + filename;
                std::cout << "Loading external texture: " << filename << std::endl;

                GLenum format = (typeName == "diffuse") ? GL_RGBA : GL_RED;
                Texture texture(filename.c_str(), typeName.c_str(), textures_loaded.size(), format, GL_UNSIGNED_BYTE);
                textures.push_back(texture);
            }

            textures_loaded.push_back(textures.back());
        }
    }
    return textures;
}