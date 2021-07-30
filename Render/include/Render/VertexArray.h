#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include <iostream>

namespace Renderer {

  class VertexArray {
  public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

  private:
    uint32_t _array_id;
  };

} // namespace Renderer

#endif // __VERTEX_ARRAY_H__
