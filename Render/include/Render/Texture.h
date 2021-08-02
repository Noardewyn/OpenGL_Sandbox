#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>

namespace Renderer {

class Texture {
public:
  Texture(const std::string &image_path);

  void bind() const;
  void unbind() const;

  int width, height, nrChannels;

private:
  uint32_t _render_id;
};

} // namespace Renderer

#endif // __TEXTURE_H__
