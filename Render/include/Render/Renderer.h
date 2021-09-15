#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Render/Color.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/IndexBuffer.h"
#include "Render/VertexArray.h"
#include "Render/VertexBufferLayout.h"
#include "Render/Texture.h"
#include "Render/Camera.h"
#include "Render/Transform.h"
#include "Render/InputManager.h"
#include "Render/Logger.h"

namespace Renderer {

  void DrawTriangles(const VertexArray& vao, const Shader& shader);
  void DrawTriangles(const VertexArray& vao, const IndexBuffer &ibo, const Shader& shader);

  void setDefaultParams();

} // namespace Renderer


#endif // __RENDERER_H__
