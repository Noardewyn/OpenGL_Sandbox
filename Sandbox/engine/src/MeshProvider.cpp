#include <corecrt_math_defines.h>

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

static GLfloat plane_vertices[] = {
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
};

static GLuint plane_indices[] = {
    0, 1, 3,
    1, 2, 3
};

namespace engine {

  std::unique_ptr<Mesh> generateCubeMesh() {
    Renderer::VertexBuffer vbo(cube_vertices, sizeof(cube_vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    auto mesh = std::make_unique<Mesh>(std::move(vbo), layout);

    return std::move(mesh);
  }

  std::unique_ptr<Mesh> generatePlaneMesh() {
    Renderer::IndexBuffer ibo(plane_indices, sizeof(plane_indices) / sizeof(plane_indices[0]));
    Renderer::VertexBuffer vbo(plane_vertices, sizeof(plane_vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    auto mesh = std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);

    return std::move(mesh);
  }

  std::unique_ptr<Mesh> generateSphereMesh(int detalization) {
    std::vector<float> vertices;

    int sector_count = detalization * 2;
    int stack_count = detalization;

    float inverse_normals = 1.0;
    float sector_step = 2.0 * M_PI / sector_count;
    float stack_step = M_PI / stack_count;

    for(int i = 0; i <= stack_count; i++) {
      float stack_angle = M_PI / 2 - i * stack_step;
      float xy = cos(stack_angle);
      float z = sin(stack_angle);

      for (int j = 0; j <= sector_count; j++) {
        float sector_angle = j * sector_step;

        float x = xy * cos(sector_angle);
        float y = xy * sin(sector_angle);

        // vertices
        vertices.push_back(x);
        vertices.push_back(z);
        vertices.push_back(y);

        // tex coords
        vertices.push_back((float)j / sector_count);
        vertices.push_back((float)i / stack_count);

        //normals
        vertices.push_back(inverse_normals * x);
        vertices.push_back(inverse_normals * z);
        vertices.push_back(inverse_normals * y);
      }
    }

    std::vector<uint32_t> indices;

    for (int i = 0; i < stack_count; i++) {
      float k1 = i * (sector_count + 1); //begin of current stack
      float k2 = k1 + sector_count + 1; // begin of the next stack

      for (int j = 0; j < sector_count; j++, k1++, k2++) {
        if (i != 0) {
          indices.push_back(k1);
          indices.push_back(k2);
          indices.push_back(k1 + 1);
        }

        // k1+1 => k2 => k2+1
        if (i != (stack_count - 1)) {
          indices.push_back(k1 + 1);
          indices.push_back(k2);
          indices.push_back(k2 + 1);
        }
      }
    }

    Renderer::IndexBuffer ibo(indices.data(), indices.size());
    Renderer::VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(float));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    auto mesh = std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);

    return std::move(mesh);
  }
} // namespace engine
