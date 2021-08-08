#ifndef __MESH_H__
#define __MESH_H__

#include "Render/VertexArray.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Shader.h"

#include "engine/Material.h"

namespace engine {

class Mesh {
public:
  Material *material;

  Mesh(const Renderer::VertexBuffer &vbo, const Renderer::VertexBufferLayout &layout);
  Mesh(const Renderer::VertexBuffer &vbo, const Renderer::IndexBuffer &ibo, const Renderer::VertexBufferLayout &layout);

  void draw(Renderer::Shader &shader);

private:
  Renderer::VertexArray _vao;
  Renderer::VertexBuffer _vbo;
  Renderer::VertexBufferLayout _layout;
  Renderer::IndexBuffer _ibo;
};

} // namespace engine

#endif // __MESH_H__
