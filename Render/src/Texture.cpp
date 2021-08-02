#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Render/Texture.h"

namespace Renderer {
  Texture::Texture(const std::string& image_path) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

    if(!data){
      std::cout << "Failed to load image: " << image_path << std::endl;
    }


    glGenTextures(1, &_render_id);
    glBindTexture(GL_TEXTURE_2D, _render_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

  void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _render_id);
  }

  void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

} // namespace Renderer
