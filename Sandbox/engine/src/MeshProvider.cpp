#include "Render/Renderer.h"

#include "engine/MeshProvider.h"

static float cube_vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,-1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f
};

namespace engine {

  std::unique_ptr<Mesh> generateCubeMesh() {
    Renderer::VertexBuffer vbo(cube_vertices, sizeof(cube_vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    auto mesh = std::make_unique<engine::Mesh>(vbo, layout);

    return mesh;
  }

} // namespace engine
