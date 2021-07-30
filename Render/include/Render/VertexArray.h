#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include <iostream>
#include "Render/VertexBuffer.h"
#include "Render/VertexBufferLayout.h"

namespace Renderer {

  class VertexArray {
  public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addBuffer(const VertexBuffer &vbo, const VertexBufferLayout &layout);
  private:
    uint32_t _array_id;
  };

} // namespace Renderer

#endif // __VERTEX_ARRAY_H__
