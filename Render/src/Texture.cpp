﻿#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Render/Logger.h"

#include "Render/Texture.h"

namespace Renderer {
  Texture::Texture(const std::string& image_path) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);

    if(!data){
      LOG_CORE_ERROR("Failed to load texture: '{}'", image_path);
      return;
    }

    glGenTextures(1, &_render_id);
    glBindTexture(GL_TEXTURE_2D, _render_id);

    GLenum format;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 3)
      format = GL_RGB;
    else if (nrChannels == 4)
      format = GL_RGBA;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }

  void Texture::bind(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _render_id);
  }

  void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

} // namespace Renderer
