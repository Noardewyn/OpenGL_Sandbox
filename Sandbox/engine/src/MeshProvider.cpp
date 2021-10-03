#include <corecrt_math_defines.h>

#include "Render/Renderer.h"

#include "engine/MeshProvider.h"

static float cube_vertices[] = {
     // FRONT FACE
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
     // LEFT FACE                            
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
     // BACK FACE                            
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
     // RIGHT FACE                           
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
     // TOP FACE                                           
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  -1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  -1.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  -1.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  -1.0f,  0.0f,
     // BOTTOM FACE
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
};

static GLuint cube_indices[] = {
    0, 2, 3,     0, 1, 2,    // FRONT
    4, 6, 7,     4, 5, 6,    // LEFT
    8, 10, 11,   8, 9, 10,   // BACK
    12, 14, 15,  12, 13, 14, // RIGHT
    16, 18, 19,  16, 17, 18, // TOP
    20, 22, 23,  20, 21, 22, // BOTTOM
};

static GLfloat plane_vertices[] = {
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
};

static GLfloat plane_vertices_tangent[] = {
     0.5f,  0.0f,  0.5f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.0f, -0.5f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.0f, -0.5f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.0f,  0.5f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,    1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
};

static GLuint plane_indices[] = {
    0, 1, 3,
    1, 2, 3
};

static float skybox_vertices[] = {
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};

namespace engine {

  std::unique_ptr<Mesh> generateCubeMesh() {
    Renderer::IndexBuffer ibo(cube_indices, sizeof(cube_indices) / sizeof(cube_indices[0]));
    Renderer::VertexBuffer vbo(cube_vertices, sizeof(cube_vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    auto mesh = std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);

    return std::move(mesh);
  }

  std::unique_ptr<Mesh> generateMatrixMesh(float width, float height, uint32_t segments_x, uint32_t segments_y) {
    
    uint32_t segments_count_width = segments_x;
    uint32_t segments_count_height = segments_y;

    float half_width = width / 2.0f;
    float half_height = height / 2.0f;

    uint32_t lines_width = segments_count_width + 1;
    uint32_t lines_height = segments_count_height + 1;
    uint32_t vertices_count = lines_width * lines_height;
    uint32_t triangles_count = segments_count_width * segments_count_height * 2;

    glm::vec3 normal = {0.0f, 0.0f, 1.0f};
    glm::vec3 tangent = { 1.0f, 0.0f, 0.0f };
    glm::vec3 bitangent = { 0.0f, 1.0f, 0.0f };

    std::vector<Vertex> vertices;
    //vertices.reserve(vertices_count);
    std::vector<uint32_t> indexes;
    //vertices.reserve(triangles_count * 3);

    float z = 0.0f;

    float width_step = width / segments_count_width;
    float height_step = -height / segments_count_height;

    float current_height = half_height;
    float current_width = -half_width;

    float width_uv_step = 2.0f / segments_count_width;
    float height_uv_step = 2.0f / segments_count_height;

    float current_height_uv = 1.0f;
    float current_width_uv = -1.0f;

    uint32_t index = 0;

    for (int h = 0; h < lines_height; h++) {
      for (int w = 0; w < lines_width; w++) {
        Vertex new_vertex;
        new_vertex.position   = {current_width, current_height, z};
        new_vertex.texCoords  = {current_width_uv, current_height_uv};
        new_vertex.normal     = normal;
        new_vertex.tangent    = tangent;
        new_vertex.bitangent  = bitangent;

        vertices.push_back(new_vertex);

        current_width += width_step;
        current_width_uv += width_uv_step;

        if (h < lines_height - 1 && w < lines_width - 1) {
            indexes.push_back(lines_width * h + w);
            indexes.push_back(lines_width * (h + 1) + w);
            indexes.push_back(lines_width * h + w + 1);

            indexes.push_back(lines_width * h + w + 1);
            indexes.push_back(lines_width * (h + 1) + w);
            indexes.push_back(lines_width * (h + 1) + w + 1);
        }
      }

      current_height += height_step;
      current_height_uv += height_uv_step;
      current_width = -half_width;
      current_width_uv = -1.0f;
    }

    Renderer::IndexBuffer ibo(indexes.data(), indexes.size());
    Renderer::VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(Vertex));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3); // position
    layout.push<float>(2); // uv
    layout.push<float>(3); // normal
    layout.push<float>(3); // tangent
    layout.push<float>(3); // bitangent

    return std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);
  }

  std::unique_ptr<Mesh> generatePlaneMesh(bool tangent_space) {

    Renderer::IndexBuffer ibo(plane_indices, sizeof(plane_indices) / sizeof(plane_indices[0]));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(2);
    layout.push<float>(3);

    if (tangent_space) {
      Renderer::VertexBuffer vbo(plane_vertices_tangent, sizeof(plane_vertices_tangent));
      Renderer::VertexBufferLayout layout;
      layout.push<float>(3);
      layout.push<float>(2);
      layout.push<float>(3);
      layout.push<float>(3);
      layout.push<float>(3);

      return std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);
    }
    else {
      Renderer::VertexBuffer vbo(plane_vertices, sizeof(plane_vertices));
      return std::make_unique<Mesh>(std::move(vbo), std::move(ibo), layout);
    }
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

  std::unique_ptr<Mesh> generateSkyBox() {
    Renderer::VertexBuffer vbo(skybox_vertices, sizeof(skybox_vertices));
    Renderer::VertexBufferLayout layout;
    layout.push<float>(3);

    auto mesh = std::make_unique<Mesh>(std::move(vbo), layout);

    return std::move(mesh);
  }
} // namespace engine
