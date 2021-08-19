#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

struct Light {
  float type;
  vec3  position;
  vec3  direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float intensity;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define NR_LIGHTS 4
uniform Light lights[NR_LIGHTS];

out Light iLights[NR_LIGHTS];

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
    TexCoord = texCoord;

    for (int i = 0; i < NR_LIGHTS; i++) {
      iLights[i] = lights[i];
      iLights[i].position = vec3(view * vec4(lights[i].position, 1.0));
      iLights[i].direction = mat3(transpose(inverse(view))) * lights[i].direction;
    }
}