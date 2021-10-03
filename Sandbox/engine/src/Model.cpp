#include <assimp/postprocess.h>

#include "engine/assets/MaterialAsset.h"
#include "engine/assets/TextureAsset.h"
#include "engine/assets/AssetManager.h"
#include "Render/Logger.h"
#include "engine/Model.h"

namespace engine {
  Model::Model()
    : obj_path("") {
  }

  Model::Model(const std::string& path)
    : obj_path(path) {
    loadModel(path);
  }

  void Model::draw(Renderer::Shader& shader, const Material& material) {
    for(auto& mesh : meshes) {
      if(mesh->material) {
        const auto& mat = *mesh->material;
        mesh->draw(shader, mat);
      }
      else {
        Material mat("Invalid material");
        mesh->draw(shader, mat);
        LOG_CORE_WARN("No available material for object: {}, mesh: {}", obj_path, mesh->name);
      }
    }
  }

  void Model::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | 
      aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      LOG_CORE_ERROR("Load model assimp error: {}", import.GetErrorString());
      return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    loadMaterials(scene);

    processNode(scene->mRootNode, scene);
  }

  void Model::loadMaterials(const aiScene* scene) {
    for(unsigned i = 0; i < scene->mNumMaterials; i++) {
      aiMaterial* material = scene->mMaterials[i];

      //auto mesh_mat = AssetManager::loadAsset<engine::MaterialAsset>(obj_path + ":" + material->GetName().C_Str());
      Material mat(material->GetName().C_Str());

      aiColor3D color(0.f, 0.f, 0.f);
      material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
      mat.diffuse_base = { color.r, color.g, color.b };

      //material->Get(AI_MATKEY_COLOR_SPECULAR, color);
      mat.specular_base = { color.r, color.g, color.b, 0.0f };

      material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
      mat.emission_base = { color.r, color.g, color.b };

      auto tempDiffuseMap = loadMaterialTexture(material, aiTextureType_DIFFUSE);
      if(tempDiffuseMap) {
        mat.texture_diffuse = tempDiffuseMap;
      }

      auto tempSpecularMap = loadMaterialTexture(material, aiTextureType_SPECULAR);
      if(tempSpecularMap) {
        mat.texture_specular = tempSpecularMap;
      }

      auto tempNormalMap = loadMaterialTexture(material, aiTextureType_HEIGHT);
      if (tempNormalMap) {
        mat.texture_normal = tempNormalMap;
      }

      auto tempHeightMap = loadMaterialTexture(material, aiTextureType_AMBIENT);
      if (tempHeightMap) {
        mat.texture_normal = tempHeightMap;
      }

      _materials.push_back(mat);
    }
  }

  void Model::processNode(aiNode* node, const aiScene* scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      auto final_mesh = processMesh(mesh, scene);
      final_mesh->material = &_materials[mesh->mMaterialIndex];
      final_mesh->name = mesh->mName.C_Str();
      meshes.push_back(std::move(final_mesh));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  std::unique_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Renderer::Texture> textures;

    // walk through each of the mesh's vertices
    for(unsigned i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vector;
      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.position = vector;

      // normals
      if(mesh->HasNormals()) {
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
      }

      // texture coordinates
      if(mesh->mTextureCoords[0]) {
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.texCoords = vec;

        // tangent
        if(mesh->mTangents) {
          vector.x = mesh->mTangents[i].x;
          vector.y = mesh->mTangents[i].y;
          vector.z = mesh->mTangents[i].z;
          vertex.tangent = vector;
        }

        // bitangent
        if (mesh->mBitangents) {
          vector.x = mesh->mBitangents[i].x;
          vector.y = mesh->mBitangents[i].y;
          vector.z = mesh->mBitangents[i].z;
          vertex.bitangent = vector;
        }
      }
      else {
        vertex.texCoords = glm::vec2(0.0f, 0.0f);
      }

      vertices.push_back(vertex);
    }

    // indices
    for(unsigned i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];
      for (unsigned j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }

    Renderer::IndexBuffer ibo(indices.data(), indices.size());
    Renderer::VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(Vertex));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3); // position
    layout.push<float>(2); // uv
    layout.push<float>(3); // normal
    layout.push<float>(3); // tangent
    layout.push<float>(3); // bitangent

    return std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);
  }

  Renderer::Texture TextureFromFile(const std::string& filename, const std::string& directory) {
    std::string filepath = filename;
    filepath = directory + '/' + filename;

    return Renderer::Texture(filepath);
  }

  engine::TextureAsset* Model::loadMaterialTexture(aiMaterial* mat, aiTextureType type) {
    engine::TextureAsset* out_texture = nullptr;

    for(unsigned i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      bool skip = false;

      ai_real blend_mode;
      
      mat->GetTexture(type, i, &str);
      const std::string& texture_path = directory + '/' + str.C_Str();
      const std::string& asset_path = AssetManager::truncateBasePath(texture_path);

      auto texture_asset = AssetManager::getAsset<engine::TextureAsset>(asset_path);

      if (texture_asset) {
        out_texture = texture_asset;
        skip = true;
        break;
      }

      if (!skip) {
        engine::TextureAsset* texture_asset = new engine::TextureAsset(asset_path, false);
        uint32_t format = type == aiTextureType_DIFFUSE ? GL_SRGB_ALPHA : GL_RGB;
        texture_asset->get() = Renderer::Texture(texture_path, format);
        auto texture_asset = AssetManager::addAsset<engine::TextureAsset>(texture_asset);
        out_texture = texture_asset;
      }
    }

    return out_texture;
  }
  
} // namespace engine
