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

out vec4 FragColor;

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(-FragPos);

  float dotEyeNormal = dot(viewDir, norm);
  dotEyeNormal = abs(dotEyeNormal);

  vec3 color = vec3(0.0, 1.0, 0.0);

  FragColor = mix(vec4(color, 1.0), vec4(0.0), smoothstep(0.2, 0.3,  dotEyeNormal));

  //if (dotEyeNormal >= 0 && dotEyeNormal <= 0.5) {
  //  FragColor = vec4(color, 1 - dotEyeNormal);
  //}
  //else
  //{
  //  FragColor = vec4(color, 0.0);
  //}
}
