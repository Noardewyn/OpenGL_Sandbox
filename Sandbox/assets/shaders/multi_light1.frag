#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

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

#define NR_LIGHTS 4
in Light iLights[NR_LIGHTS];

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
