#SHADER VERTEX
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
uniform int   lights_count;

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

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    vec4      fillColor;
    vec3      emissionStrength;
    float     shininess;
}; 

uniform Material material;

#define NR_LIGHTS 4
uniform Light lights[NR_LIGHTS];
uniform int   lights_count;
uniform bool calculate_light;
uniform float fog_distance = 0;

vec4 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec4 ambient  = light.ambient  * texture(material.diffuse, TexCoord);
    vec4 diffuse  = light.diffuse  * diff * texture(material.diffuse, TexCoord);
    vec4 specular = light.specular * spec * texture(material.specular, TexCoord);

    diffuse = diffuse * light.intensity;
    specular = specular * light.intensity;

    return (diffuse + specular + ambient);
}  

vec4 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec4 ambient  = light.ambient  * texture(material.diffuse, TexCoord);
    vec4 diffuse  = light.diffuse  * diff * texture(material.diffuse, TexCoord);
    vec4 specular = light.specular * spec * texture(material.specular, TexCoord);
    
    ambient  = ambient * attenuation;
    diffuse  = diffuse * attenuation * light.intensity;
    specular = specular * attenuation * light.intensity;
    return (ambient + diffuse + specular);
} 

vec4 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));

  if (theta <= light.cutOff)
    return light.ambient * texture(material.diffuse, TexCoord);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

  // combine results
  vec4 ambient = light.ambient * texture(material.diffuse, TexCoord);
  vec4 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord);
  vec4 specular = light.specular * spec * texture(material.specular, TexCoord);

  float epsilon = light.cutOff - light.outerCutOff;
  float spot_smooth = smoothstep(1.0, 0.0, (theta - light.outerCutOff) / epsilon);

  diffuse *= spot_smooth;
  specular *= spot_smooth;

  diffuse = diffuse * light.intensity * attenuation;
  specular = specular * light.intensity * attenuation;
  // ambient = ambient * attenuation;

  return (diffuse + specular + ambient);
}

float near = 0.1;

float LinearizeDepth(float depth)
{
  float z = depth * 2.0 - 1.0;
  return (2.0 * near * fog_distance) / (fog_distance + near - z * (fog_distance - near));
}

void main()
{
    if (calculate_light) 
    {
      vec3 norm = normalize(Normal);
      vec3 viewDir = normalize(-FragPos);
      vec4 resultLight = vec4(0.0);

      for (int i = 0; i < lights_count; i++) {
        if (int(lights[i].type) == 1)
          resultLight += CalcPointLight(lights[i], norm, FragPos, viewDir);
        else if (int(lights[i].type) == 2)
          resultLight += CalcSpotLight(lights[i], norm, FragPos, viewDir);
        else if(int(lights[i].type) == 3)
          resultLight += CalcDirLight(lights[i], norm, viewDir);
      }

      FragColor = resultLight + material.fillColor;
    }
    else 
    {
      FragColor = texture(material.diffuse, TexCoord) + material.fillColor;
    } 

    if(fog_distance > 0)
      FragColor += LinearizeDepth(gl_FragCoord.z) / fog_distance;
}
