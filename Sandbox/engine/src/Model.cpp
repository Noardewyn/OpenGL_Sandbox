#include <assimp/postprocess.h>

#include "Render/Logger.h"
#include "engine/Model.h"

namespace engine {
  Model::Model(const std::string& path) {
    loadModel(path);

    _materials.emplace_back("Model");

    if (!diffuseMaps.empty())
      _materials[0].texture_diffuse = &diffuseMaps[0];

    if (!specularMaps.empty())
      _materials[0].texture_specular = &specularMaps[0];

    if (!normalMaps.empty())
      _materials[0].texture_normal = &normalMaps[0];

    if (!heightMaps.empty())
      _materials[0].texture_height = &heightMaps[0];
  }

  void Model::draw(Renderer::Shader& shader, const std::vector<Material*>& materials) {
    for(auto& mesh : _meshes) {
      mesh->draw(shader, materials);
    }
  }

  void Model::loadModel(const std::string& path) {
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      LOG_CORE_ERROR("Load model assimp error: {}", import.GetErrorString());
      return;
    }

    _directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
  }

  void Model::processNode(aiNode* node, const aiScene* scene) {
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      auto final_mesh = processMesh(mesh, scene);
      const auto offset = node->mTransformation;

      final_mesh->offset[0] = { offset.a1, offset.a2, offset.a3, offset.a4 };
      final_mesh->offset[1] = { offset.b1, offset.b2, offset.b3, offset.b4 };
      final_mesh->offset[2] = { offset.c1, offset.c2, offset.c3, offset.c4 };
      final_mesh->offset[3] = { offset.d1, offset.d2, offset.d3, offset.d4 };

      _meshes.push_back(std::move(final_mesh));
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

    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // 1. diffuse maps
    std::vector<Renderer::Texture> tempDiffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    if(!tempDiffuseMaps.empty()) diffuseMaps = tempDiffuseMaps;

    // 2. specular maps
    std::vector<Renderer::Texture> tempSpecularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
    if (!tempSpecularMaps.empty()) specularMaps = tempSpecularMaps;

    // 3. normal maps
    std::vector<Renderer::Texture> tempNormalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT);
    if (!tempNormalMaps.empty()) normalMaps = tempNormalMaps;

    // 4. height maps
    std::vector<Renderer::Texture> tempHeightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT);
    if (!tempHeightMaps.empty()) heightMaps = tempHeightMaps;

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

  std::vector<Renderer::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type) {
    std::vector<Renderer::Texture> textures;

    for(unsigned i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      bool skip = false;

      mat->GetTexture(type, i, &str);

      for(unsigned j = 0; j < _loaded_textures.size(); j++) {
        if(_loaded_textures[j] == str.C_Str()) {
          skip = true;
          break;
        }
      }

      if (!skip) {
        Renderer::Texture texture = TextureFromFile(str.C_Str(), _directory);
        textures.push_back(texture);
        _loaded_textures.emplace_back(str.C_Str());
      }
    }

    return textures;
  }
  
} // namespace engine
