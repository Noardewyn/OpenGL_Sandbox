#ifndef __SHADER_H__
#define __SHADER_H__

#include <iostream>
#include <glm/glm.hpp>

namespace Renderer {

class Shader {
public:
  Shader() {};
  Shader(const std::string& fragment_path, const std::string& vertex_path);
  ~Shader();
  
  void bind() const;
  void unbind() const;

  void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
  void setUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

private:
  enum class shader_type_t { VERTEX, FRAGMENT };

  std::string _fragment_path;
  std::string _vertex_path;

  uint32_t _shader_id = -1;

  uint32_t compileShader(const std::string& source, const shader_type_t& shader_type);
  bool createShaderProgram(const std::string& fragment_source, const std::string& vertex_source);
  std::string loadShaderSource(const std::string& filepath);
  bool checkCompileShadersErrors(unsigned int id, const shader_type_t& shader_type) const;
};

} // namespace Renderer

#endif // __SHADER_H__
