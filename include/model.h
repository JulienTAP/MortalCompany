#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shaderClass.h"

class Model 
{
public:
    // The meshes are now public to be added to the scene graph
    std::vector<Mesh> meshes;

    Model(std::string const &path, Shader &shader);

private:
    std::string directory;
    std::vector<Texture> textures_loaded;
    Shader& shader;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, const aiScene *scene, aiTextureType type, std::string typeName);
};

#endif