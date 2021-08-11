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
  Mesh(Renderer::VertexBuffer&& vbo, const Renderer::VertexBufferLayout &layout);
  Mesh(Renderer::VertexBuffer&& vbo, Renderer::IndexBuffer&& ibo, const Renderer::VertexBufferLayout &layout);

  void draw(Renderer::Shader &shader, const Material &material);

private:
  Renderer::VertexArray _vao;
  Renderer::VertexBuffer _vbo;
  Renderer::IndexBuffer _ibo;
  Renderer::VertexBufferLayout _layout;
};

} // namespace engine

#endif // __MESH_H__
