#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>

namespace Renderer {

class Texture {
public:
  int width, height, nrChannels;
  std::string image_path;

  Texture();
  Texture(const std::string &image_path);

  void bind(int unit = 0) const;
  void unbind() const;


private:
  uint32_t _render_id;
};

} // namespace Renderer

#endif // __TEXTURE_H__
