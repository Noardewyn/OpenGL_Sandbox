#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

struct DirLight {
  vec3 direction;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float intensity;
};

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float intensity;
};

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

out DirLight iDirLight;
out PointLight iPointLights[NR_POINT_LIGHTS];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
    TexCoord = texCoord;
    
    for (int i = 0; i < NR_POINT_LIGHTS; i++) {
      iPointLights[i] = pointLights[i];
      iPointLights[i].position = vec3(view * vec4(pointLights[i].position, 1.0));
    }

    iDirLight = dirLight;
}