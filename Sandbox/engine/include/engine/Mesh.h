#ifndef __MESH_H__
#define __MESH_H__

#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Shader.h"
#include "engine/Material.h"
#include "engine/IRenderable.h"

namespace engine {

  struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
  };

  class Mesh : public IRenderable {
  public:
    std::string name;
    Material *material;

    Mesh(Renderer::VertexBuffer&& vbo, const Renderer::VertexBufferLayout &layout);
    Mesh(Renderer::VertexBuffer&& vbo, Renderer::IndexBuffer&& ibo, const Renderer::VertexBufferLayout &layout);

    void draw(Renderer::Shader& shader) override;
    void draw(Renderer::Shader& shader, const Material& material) override;
    void draw(Renderer::Shader& shader, const Renderer::Texture& cubemap) override;

  private:
    Renderer::VertexArray _vao;
    Renderer::VertexBuffer _vbo;
    Renderer::IndexBuffer _ibo;
    Renderer::VertexBufferLayout _layout;
  };

} // namespace engine

#endif // __MESH_H__
