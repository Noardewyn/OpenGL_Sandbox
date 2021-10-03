#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
#include <vector>
#include <GL/glew.h>

namespace Renderer {

class Texture {
public:
  int width, height, nrChannels;
  bool has_alpha = false;
  bool is_cubemap = false;
  std::string image_path;

  Texture();
  Texture(const std::string &image_path, GLenum format = GL_RGBA);
  Texture(const std::vector<std::string>& cubemap, GLenum format = GL_RGB);

  void bind(int unit = 0) const;
  void unbind() const;

private:
  uint32_t _render_id;
};

} // namespace Renderer

#endif // __TEXTURE_H__
