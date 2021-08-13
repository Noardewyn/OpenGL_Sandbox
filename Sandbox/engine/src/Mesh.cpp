#include "Render/Renderer.h"

#include "engine/Mesh.h"

namespace engine {

Mesh::Mesh(Renderer::VertexBuffer&& vbo, const Renderer::VertexBufferLayout& layout)
  : _vbo(std::move(vbo)), _layout(layout) {
  _vao.addBuffer(_vbo, _layout);
}

Mesh::Mesh(Renderer::VertexBuffer&& vbo, Renderer::IndexBuffer&& ibo, const Renderer::VertexBufferLayout& layout)
  : _vbo(std::move(vbo)), _ibo(std::move(ibo)), _layout(layout) {
  _vao.bind();
  _ibo.bind();
  _vao.addBuffer(_vbo, _layout);
  _vao.unbind();
}

void Mesh::draw(Renderer::Shader& shader, const std::vector<Material*>& materials) {

  shader.bind();

  const auto& material = *materials[0];

  shader.setUniform3f("material.emissionStrength", material.emission_strength.r, material.emission_strength.g, material.emission_strength.b);
  shader.setUniform1f("material.shininess", material.shininess);
  shader.setUniformColor("material.fillColor", material.color);
  shader.setUniformColor("color", material.color);

  if(material.texture_diffuse) {
    shader.setUniform1i("material.diffuse", 0);
    material.texture_diffuse->bind(0);
  }
  else {
    shader.setUniform1i("material.diffuse", 10);
  }

  if (material.texture_specular) {
    shader.setUniform1i("material.specular", 1);
    material.texture_specular->bind(1);
  }
  else {
    shader.setUniform1i("material.specular", 10);
  }

  if (material.texture_emission) {
    shader.setUniform1i("material.emission", 2);
    material.texture_emission->bind(2);
  }
  else {
    shader.setUniform1i("material.emission", 10);
  }

  if (material.texture_normal) {
    shader.setUniform1i("material.normal", 3);
    material.texture_normal->bind(3);
  }
  else {
    shader.setUniform1i("material.normal", 10);
  }

  if (material.texture_height) {
    shader.setUniform1i("material.height", 4);
    material.texture_height->bind(4);
  }
  else {
    shader.setUniform1i("material.height", 10);
  }

  if(_ibo.count())
    DrawTriangles(_vao, _ibo, shader);
  else
    DrawTriangles(_vao, shader);

  if (material.texture_diffuse)
    material.texture_diffuse->unbind();

  if (material.texture_specular)
    material.texture_specular->unbind();

  if (material.texture_emission)
    material.texture_emission->unbind();
}

} // namespace engine
