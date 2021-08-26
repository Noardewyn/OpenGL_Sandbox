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

#define NR_LIGHTS 4
in Light iLights[NR_LIGHTS];

out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;

    vec4      fillColor;
    vec3      emissionStrength;
    float     shininess;
}; 

uniform Material material;

uniform bool calculate_light;
uniform int  lights_count;

uniform float fog_distance = 0;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = vec3(light.ambient)  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = vec3(light.diffuse)  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = vec3(light.specular) * spec * vec3(texture(material.specular, TexCoord));

    diffuse = diffuse * light.intensity;
    specular = specular * light.intensity;

    return (diffuse + specular + ambient);
}  

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    vec3 ambient  = vec3(light.ambient)  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = vec3(light.diffuse)  * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = vec3(light.specular) * spec * vec3(texture(material.specular, TexCoord));
    
    ambient  = ambient * attenuation;
    diffuse  = diffuse * attenuation * light.intensity;
    specular = specular * attenuation * light.intensity;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));

  if (theta <= light.cutOff)
    return vec3(light.ambient) * vec3(texture(material.diffuse, TexCoord));

  // diffuse shading
  float diff = max(dot(normal, lightDir), 0.0);
  // specular shading
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

  // combine results
  vec3 ambient = vec3(light.ambient) * vec3(texture(material.diffuse, TexCoord));
  vec3 diffuse = vec3(light.diffuse) * diff * vec3(texture(material.diffuse, TexCoord));
  vec3 specular = vec3(light.specular) * spec * vec3(texture(material.specular, TexCoord));

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
      vec3 resultLight = vec3(0.0);

      for (int i = 0; i < lights_count; i++) {
        if (int(iLights[i].type) == 1)
          resultLight += CalcPointLight(iLights[i], norm, FragPos, viewDir);
        else if (int(iLights[i].type) == 2)
          resultLight += CalcSpotLight(iLights[i], norm, FragPos, viewDir);
        else if(int(iLights[i].type) == 3)
          resultLight += CalcDirLight(iLights[i], norm, viewDir);
      }

      color = vec4(resultLight, 1.0) + material.fillColor;
    }
    else 
    {
      color = texture(material.diffuse, TexCoord) + material.fillColor;
    } 

    if(fog_distance > 0)
      color += LinearizeDepth(gl_FragCoord.z) / fog_distance;
}