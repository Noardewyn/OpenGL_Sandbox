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

void Mesh::draw(Renderer::Shader& shader) {
    shader.bind();

    if (_ibo.count())
        DrawTriangles(_vao, _ibo, shader);
    else
        DrawTriangles(_vao, shader);
}

void Mesh::draw(Renderer::Shader& shader, const Material& material) {
  shader.bind();

  shader.setUniformColor("color", material.diffuse_base);
  shader.setUniformColor("material.diffuse_base", material.diffuse_base);
  shader.setUniformColor("material.specular_base", material.specular_base);
  shader.setUniformColor("material.emission_base", material.emission_base);
  shader.setUniform1f("material.shininess", material.shininess);

  shader.setUniform1i("material.is_diffuse", material.texture_diffuse != nullptr);
  shader.setUniform1i("material.is_specular", material.texture_specular != nullptr);
  shader.setUniform1i("material.is_emission", material.texture_emission != nullptr);
  shader.setUniform1i("material.is_normal", material.texture_normal != nullptr);
  shader.setUniform1i("material.is_displacement", material.texture_displacement != nullptr);

  if(material.texture_diffuse) {
    shader.setUniform1i("material.diffuse", 0);
    material.texture_diffuse->get().bind(0);
  }
  else {
    shader.setUniform1i("material.diffuse", -1);
  }

  if (material.texture_specular) {
    shader.setUniform1i("material.specular", 1);
    material.texture_specular->get().bind(1);
  }
  else {
    shader.setUniform1i("material.specular", -1);
  }

  if (material.texture_emission) {
    shader.setUniform1i("material.emission", 2);
    material.texture_emission->get().bind(2);
  }
  else {
    shader.setUniform1i("material.emission", -1);
  }

  if (material.texture_normal) {
    shader.setUniform1i("material.normal", 3);
    material.texture_normal->get().bind(3);
  }
  else {
    shader.setUniform1i("material.normal", -1);
  }

  if (material.texture_displacement) {
    shader.setUniform1i("material.displacement", 4);
    material.texture_displacement->get().bind(4);
  }
  else {
    shader.setUniform1i("material.displacement", -1);
  }

  if(_ibo.count())
    DrawTriangles(_vao, _ibo, shader);
  else
    DrawTriangles(_vao, shader);

  if (material.texture_diffuse)
    material.texture_diffuse->get().unbind();

  if (material.texture_specular)
    material.texture_specular->get().unbind();

  if (material.texture_emission)
    material.texture_emission->get().unbind();

  if (material.texture_normal)
    material.texture_normal->get().unbind();

  if (material.texture_displacement)
    material.texture_displacement->get().unbind();
}

void Mesh::draw(Renderer::Shader& shader, const Renderer::Texture& cubemap) {
  shader.bind();
  shader.setUniform1i("cubemap", 0);
  cubemap.bind();

  if (_ibo.count())
    DrawTriangles(_vao, _ibo, shader);
  else
    DrawTriangles(_vao, shader);

  shader.unbind();
}

} // namespace engine
