#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

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

in DirLight iDirLight;

#define NR_POINT_LIGHTS 4
in PointLight iPointLights[NR_POINT_LIGHTS];

out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    vec3      emissionStrength;
    float     shininess;
}; 

uniform bool useFillColor;
uniform vec4 fillColor;

uniform int point_light_count;
uniform int directional_light_count;

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
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

    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);
    
    vec3 resultLight = vec3(0.0);

    resultLight += CalcDirLight(iDirLight, norm, viewDir);

    for(int i = 0; i < point_light_count; i++)
        resultLight += CalcPointLight(iPointLights[i], norm, FragPos, viewDir);

    color = vec4(resultLight, 1.0);
    
}