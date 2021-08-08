#include "Render/Renderer.h"

#include "engine/Mesh.h"

namespace engine {

Mesh::Mesh(const Renderer::VertexBuffer &vbo, const Renderer::VertexBufferLayout &layout)
  : _vbo(vbo), _layout(layout) {
  _vao.bind();
  _vao.addBuffer(_vbo, _layout);
  _vao.unbind();
}

Mesh::Mesh(const Renderer::VertexBuffer& vbo, const Renderer::IndexBuffer& ibo, const Renderer::VertexBufferLayout &layout)
  : _vbo(vbo), _layout(layout), _ibo(ibo) {
  _vao.bind();
  _ibo.bind();
  _vao.addBuffer(_vbo, _layout);
  _vao.unbind();
}

void Mesh::draw(Renderer::Shader& shader) {
  shader.bind();
  shader.setUniform1i("material.diffuse", 0);
  shader.setUniform1i("material.specular", 1);
  shader.setUniform1i("material.emission", 2);
  material->texture_diffuse->bind(0);
  material->texture_specular->bind(1);
  material->texture_emission->bind(2);
  Renderer::DrawTriangles(_vao, shader);
}
} // namespace engine
