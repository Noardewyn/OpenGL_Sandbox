﻿#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Render/Logger.h"
#include "Render/Renderer.h"

#include "Render/Shader.h"

namespace Renderer {

Shader::Shader(const std::string& fragment_path, const std::string& vertex_path)
  : _fragment_path(fragment_path),
  _vertex_path(vertex_path) {
  const std::string& fragment_source = loadShaderSource(fragment_path);
  const std::string& vertex_source = loadShaderSource(vertex_path);
  createShaderProgram(fragment_source, vertex_source);
}

Shader::~Shader() {
  LOG_CORE_TRACE("Deleting Shader with id={}", _shader_id);
  glDeleteProgram(_shader_id);
}

void Shader::bind() const {
  glUseProgram(_shader_id);
}

void Shader::unbind() const {
  glUseProgram(0);
}

void Shader::setUniformColor(const std::string& name, const Color& color) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform4f(uniform, color.r, color.g, color.b, color.a);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform4f(uniform, v0, v1, v2, v3);
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform3f(uniform, v0, v1, v2);
}

void Shader::setUniformMatrix4f(const std::string& name, const glm::mat4& matrix) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform2f(const std::string& name, float v0, float v1) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform2f(uniform, v0, v1);
}

void Shader::setUniform1f(const std::string& name, float value) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform1f(uniform, value);
}

void Shader::setUniform1i(const std::string& name, int value) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform1i(uniform, value);
}

void Shader::setUniform1ui(const std::string& name, uint32_t value) {
  GLuint uniform = glGetUniformLocation(_shader_id, name.c_str());
  glUniform1ui(uniform, value);
}

std::string Shader::loadShaderSource(const std::string &filepath) {
  std::stringstream shader_source;
  std::ifstream shader_file(filepath);

  if (shader_file.is_open()) {
    shader_source << shader_file.rdbuf();
    shader_file.close();
  }
  else {
    LOG_CORE_ERROR("Unable to open shader file: {}", filepath);
  }

  return shader_source.str();
}

bool Shader::checkCompileShadersErrors(unsigned int id, const shader_type_t& shader_type) const {
  int  success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];

    glGetShaderInfoLog(id, 512, nullptr, infoLog);

    std::string shader_type_str;

    if (shader_type == shader_type_t::FRAGMENT)
      shader_type_str = "Fragment";
    else if (shader_type == shader_type_t::VERTEX)
      shader_type_str = "Vertex";

    LOG_CORE_ERROR("{} shader, compilation failed.\nError message:\n{}", shader_type_str, infoLog);
  }

  return success;
}

uint32_t Shader::compileShader(const std::string& source, const shader_type_t& shader_type) {
  GLuint id = 0;

  if (shader_type == shader_type_t::FRAGMENT)
    id = glCreateShader(GL_FRAGMENT_SHADER);
  else if (shader_type == shader_type_t::VERTEX)
    id = glCreateShader(GL_VERTEX_SHADER);

  const GLchar* csource = source.c_str();
  const GLint size = source.length();
  glShaderSource(id, 1, &csource, &size);
  glCompileShader(id);

  checkCompileShadersErrors(id, shader_type);

  return id;
}

bool Shader::createShaderProgram(const std::string& fragment_source, const std::string& vertex_source) {
  GLuint vertex_id = compileShader(vertex_source, shader_type_t::VERTEX);
  GLuint fragment_id = compileShader(fragment_source, shader_type_t::FRAGMENT);

  if(!fragment_id || !vertex_id)
    return false;

  _shader_id = glCreateProgram();

  glAttachShader(_shader_id, vertex_id);
  glAttachShader(_shader_id, fragment_id);
  glLinkProgram(_shader_id);
  //glValidateProgram(_shader_id);

  int success;
  glGetProgramiv(_shader_id, GL_LINK_STATUS, &success);

  if (!success) {
    char info_log[GL_INFO_LOG_LENGTH];
    glGetProgramInfoLog(_shader_id, GL_INFO_LOG_LENGTH, nullptr, info_log);
    LOG_CORE_ERROR("Shader program linking failed\nError message:\n{}", info_log);
    return false;
  }

  glDeleteShader(fragment_id);
  glDeleteShader(vertex_id);

  return true;
}

} // namespace Renderer
