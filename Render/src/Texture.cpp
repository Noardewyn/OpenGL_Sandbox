﻿#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Render/Logger.h"

#include "Render/Texture.h"

namespace Renderer {
  Texture::Texture()
    : _render_id(0), width(0), height(0), nrChannels(0) {
    
  }

  Texture::Texture(const std::string& image_path, GLenum format)
    : image_path(image_path), _render_id(0), width(0), height(0), nrChannels(0) {
    
    if (format == GL_RED)
      nrChannels = nrChannels;
    else if (format == GL_RGB)
      nrChannels = 3;
    else if (format == GL_RGBA)
      nrChannels = 4;
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, nrChannels);

    //if(stbi_failure_reason())
    //  LOG_CORE_WARN("Texture load: {}", stbi_failure_reason());

    if(!data){
      LOG_CORE_ERROR("Failed to load texture: '{}'", image_path);
      return;
    }

    glGenTextures(1, &_render_id);
    glBindTexture(GL_TEXTURE_2D, _render_id);

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
