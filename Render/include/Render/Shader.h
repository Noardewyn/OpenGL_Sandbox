#ifndef __SHADER_H__
#define __SHADER_H__

#include <iostream>
#include <glm/glm.hpp>

#include "Render/Color.h"

namespace Renderer {

class Shader {
public:
  Shader() {};
  Shader(const std::string& fragment_path, const std::string& vertex_path);
  Shader(const std::string& shader_path);
  ~Shader();
  
  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  void bind() const;
  void unbind() const;

  void setUniformColor(const std::string& name, const Color& color);
  void setUniform1i(const std::string& name, int value);
  void setUniform1ui(const std::string& name, uint32_t value);
  void setUniform1f(const std::string& name, float value);
  void setUniform2f(const std::string& name, float v0, float v1);
  void setUniform3f(const std::string& name, float v0, float v1, float v2);
  void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void setUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

  void recompile();

private:
  enum class shader_type_t { UNDEFINED = -1, VERTEX = 0, FRAGMENT = 1 };

  std::string _fragment_path;
  std::string _vertex_path;
  std::string _shader_path;

  uint32_t _shader_id = -1;

  std::pair<std::string, std::string> getShadersSource(const std::string& shader_path);
  uint32_t compileShader(const std::string& source, const shader_type_t& shader_type);
  bool createShaderProgram(const std::string& fragment_source, const std::string& vertex_source);
  std::string loadShaderSource(const std::string& filepath);
  bool checkCompileShadersErrors(unsigned int id, const shader_type_t& shader_type) const;
};

} // namespace Renderer

#endif // __SHADER_H__
