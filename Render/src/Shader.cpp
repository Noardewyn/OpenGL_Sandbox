#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Render/Logger.h"

#include "Render/Shader.h"

namespace Renderer {

Shader::Shader(const std::string& fragment_path, const std::string& vertex_path)
  : _fragment_path(fragment_path),
  _vertex_path(vertex_path) {
  const std::string fragment_source = loadShaderSource(fragment_path);
  const std::string vertex_source = loadShaderSource(vertex_path);
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

std::string Shader::loadShaderSource(const std::string &filepath) {
  std::string shader_source;
  std::ifstream shader_file(filepath);

  if (shader_file.is_open()) {
    std::string line;

    shader_file >> shader_source;

    while (std::getline(shader_file, line)) {
      shader_source += line + '\n';
    }

    shader_file.close();
  }
  else {
    std::cout << "Unable to open shader file: " << filepath << std::endl;
  }

  return shader_source;
}

bool Shader::checkCompileShadersErrors(unsigned int id, const shader_type_t& shader_type) const {
  int  success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];

    glGetShaderInfoLog(id, 512, nullptr, infoLog);

    std::string shader_type_str;

    if (shader_type == shader_type_t::FRAGMENT)
      shader_type_str = "FRAGMENT";
    else if (shader_type == shader_type_t::VERTEX)
      shader_type_str = "VERTEX";

    std::cout << "ERROR::SHADER::" << shader_type_str << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return success;
}

uint32_t Shader::compileShader(const std::string& source, const shader_type_t& shader_type) {
  GLuint id = 0;

  if (shader_type == shader_type_t::FRAGMENT)
    id = glCreateShader(GL_FRAGMENT_SHADER);
  else if (shader_type == shader_type_t::VERTEX)
    id = glCreateShader(GL_VERTEX_SHADER);

  const char* csource = source.c_str();

  glShaderSource(id, 1, &csource, nullptr);
  glCompileShader(id);

  checkCompileShadersErrors(id, shader_type);

  return id;
}

bool Shader::createShaderProgram(const std::string& fragment_source, const std::string& vertex_source) {
  GLuint fragment_id = compileShader(fragment_source, shader_type_t::FRAGMENT);
  GLuint vertex_id = compileShader(vertex_source, shader_type_t::VERTEX);

  if(!fragment_id || !vertex_id)
    return false;

  _shader_id = glCreateProgram();

  glAttachShader(_shader_id, vertex_id);
  glAttachShader(_shader_id, fragment_id);
  glLinkProgram(_shader_id);
  glValidateProgram(_shader_id);

  glDeleteShader(fragment_id);
  glDeleteShader(vertex_id);

  int success;
  glGetProgramiv(_shader_id, GL_LINK_STATUS, &success);

  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(_shader_id, 512, nullptr, info_log);
    std::cout << "ERROR PROGRAM LINKING FAILED\n" << info_log << std::endl;

    return false;
  }

  return true;
}

} // namespace Renderer
