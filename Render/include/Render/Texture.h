#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
#include <GL/glew.h>

namespace Renderer {

class Texture {
public:
  int width, height, nrChannels;
  std::string image_path;

  Texture();
  Texture(const std::string &image_path, GLenum format = GL_RGBA);

  void bind(int unit = 0) const;
  void unbind() const;


private:
  uint32_t _render_id;
};

} // namespace Renderer

#endif // __TEXTURE_H__
