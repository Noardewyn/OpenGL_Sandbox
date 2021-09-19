#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
    TexCoord = texCoord;
}

#SHADER FRAGMENT
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;

  vec4      fillColor;
  vec3      emissionStrength;
  float     shininess;
};

uniform Material material;
uniform float fog_distance = 0;

out vec4 FragColor;

float near = 0.1;

float LinearizeDepth(float depth)
{
  float z = depth * 2.0 - 1.0;
  return (2.0 * near * fog_distance) / (fog_distance + near - z * (fog_distance - near));
}

void main()
{
  FragColor = texture(material.diffuse, TexCoord) + material.fillColor;

  if (fog_distance > 0)
    FragColor += LinearizeDepth(gl_FragCoord.z) / fog_distance;
}
