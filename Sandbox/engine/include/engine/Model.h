#ifndef __MODEL_H__
#define __MODEL_H__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Render/Shader.h"
#include "engine/Mesh.h"

namespace engine {
  class Model
  {
  public:
    std::vector<std::unique_ptr<Mesh>> _meshes;
    std::vector<Material> _materials;
    std::string obj_path;

    Model(const std::string& path);
    void draw(Renderer::Shader& shader);

  private:
    std::string _directory;
    std::unordered_map<std::string, Renderer::Texture> _loaded_textures;

    void loadModel(const std::string& path);
    void loadMaterials(const aiScene* scene);
    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    Renderer::Texture* Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type);
  };
} // namespace engine

#endif // __MODEL_H__
