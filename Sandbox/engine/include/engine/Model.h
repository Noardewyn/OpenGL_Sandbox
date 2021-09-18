#ifndef __MODEL_H__
#define __MODEL_H__

#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Render/Texture.h"
#include "Render/Shader.h"
#include "engine/Mesh.h"

namespace engine {
  class Model : public IRenderable {
  public:
    std::string directory;
    std::string obj_path;
    std::vector<std::unique_ptr<Mesh>> meshes;
    std::vector<Material> _materials;
    std::unordered_map<std::string, Renderer::Texture> loaded_textures;

    Model();
    Model(const std::string& path);
    void draw(Renderer::Shader& shader, const Material& material) override;

  private:
    void loadModel(const std::string& path);
    void loadMaterials(const aiScene* scene);
    void processNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
    engine::TextureAsset* Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type);
  };
} // namespace engine

#endif // __MODEL_H__
