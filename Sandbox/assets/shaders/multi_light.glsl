#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

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

out VS_OUT{
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} vs_out;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(view * model * vec4(position, 1.0));
    TexCoord = texCoord;
    Normal = mat3(transpose(inverse(view * model))) * normal;

    vec3 T = normalize(vec3(view * model * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(view * model * vec4(bitangent, 0.0)));
    vec3 N = Normal; //vec3(view * model * normalize(vec4(normal, 0.0)));
    vs_out.TBN = mat3(T, B, N);
}

#SHADER FRAGMENT
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

in VS_OUT{
    vec3 FragPos;
    vec2 TexCoord;
    mat3 TBN;
} fs_in;

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
    sampler2D normal;
    sampler2D displacement;

    bool is_diffuse;
    bool is_specular;
    bool is_emission;
    bool is_normal;
    bool is_displacement;

    vec4  diffuse_base;
    vec4  specular_base;
    vec4  emmision_base;

    float shininess;
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
    vec4 ambient = light.ambient * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 diffuse = light.diffuse * diff * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 specular = light.specular * spec * (material.is_specular ? texture(material.specular, TexCoord) : material.specular_base);

    diffuse = diffuse * light.intensity;
    specular = specular * light.intensity;

    return (diffuse + specular + ambient);
}  

vec4 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec4 ambient  = light.ambient  * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 diffuse  = light.diffuse  * diff * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 specular = light.specular * spec * (material.is_specular ? texture(material.specular, TexCoord) : material.specular_base);
    
    ambient  = ambient * attenuation;
    diffuse  = diffuse * attenuation * light.intensity;
    specular = specular * attenuation * light.intensity;
    return (ambient + diffuse + specular);
} 

vec4 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  
  float theta = dot(lightDir, normalize(-light.direction));

  if (theta <= light.cutOff)
    return light.ambient * texture(material.diffuse, TexCoord);

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

  // combine results
  vec4 ambient = light.ambient * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
  vec4 diffuse = light.diffuse * diff * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
  vec4 specular = light.specular * spec * (material.is_specular ? texture(material.specular, TexCoord) : material.specular_base);

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

      // normal map processing
      if (material.is_normal) {
        norm = normalize(fs_in.TBN * texture(material.normal, TexCoord).rgb * 2.0 - 1.0);
      }
      
      for (int i = 0; i < lights_count; i++) {
        if (int(lights[i].type) == 1)
          resultLight += CalcPointLight(lights[i], norm, FragPos, viewDir);
        else if (int(lights[i].type) == 2)
          resultLight += CalcSpotLight(lights[i], norm, FragPos, viewDir);
        else if(int(lights[i].type) == 3)
          resultLight += CalcDirLight(lights[i], norm, viewDir);
      }

      FragColor = resultLight;
    }
    else 
    {
      FragColor = texture(material.diffuse, TexCoord) + material.diffuse_base;
    } 

    if(fog_distance > 0)
      FragColor += LinearizeDepth(gl_FragCoord.z) / fog_distance;
}
