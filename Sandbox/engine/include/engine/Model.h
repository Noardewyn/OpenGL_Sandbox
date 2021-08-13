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

    Model(const std::string& path);
    void draw(Renderer::Shader& shader, const std::vector<Material*>& materials);

  private:
    std::string _directory;
    std::vector<std::string> _loaded_textures;

    std::vector<Renderer::Texture> diffuseMaps;
    std::vector<Renderer::Texture> specularMaps;
    std::vector<Renderer::Texture> normalMaps;
    std::vector<Renderer::Texture> heightMaps;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Renderer::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
  };
} // namespace engine

#endif // __MODEL_H__
