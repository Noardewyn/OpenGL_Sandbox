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
uniform mat4 lightSpaceMatrix;

#define NR_LIGHTS 4
uniform Light lights[NR_LIGHTS];
uniform int   lights_count;

out VS_OUT{
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
    vec4 FragPosLightSpace;
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

    vec3 T = normalize(mat3(transpose(inverse(view * model))) * tangent);
    vec3 B = normalize(mat3(transpose(inverse(view * model))) * bitangent);
    vec3 N = normalize(Normal);

    if (dot(cross(N, T), B) < 0.0)
        T = T * -1.0;

    vs_out.TBN = transpose(mat3(T, B, N));
    vs_out.Tangent = T;
    vs_out.Bitangent = B;

    vs_out.FragPosLightSpace = lightSpaceMatrix * model * vec4(position, 1.0f);
}

#SHADER FRAGMENT
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

in VS_OUT{
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
    vec4 FragPosLightSpace;
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
  float range;

  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  bool hasShadowMap;
  sampler2D shadowMap;

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
uniform float fog_distance = 0;
uniform int  displayMode;
uniform bool calculate_light;
uniform bool debug_show_normal_maps;

float ShadowCalculation(Light light, vec4 fragPosLightSpace, float bias)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(light.shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
    const int halfkernelWidth = 4;
    for(int x = -halfkernelWidth; x <= halfkernelWidth; ++x)
    {
	    for(int y = -halfkernelWidth; y <= halfkernelWidth; ++y)
	    {
		    float pcfDepth = texture(light.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
		    shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
	    }
    }
    shadow /= ((halfkernelWidth*2+1)*(halfkernelWidth*2+1));

    return shadow;
}  

vec4 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = fs_in.TBN * normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    if (diff == 0.0)
        spec = 0.0;

    // combine results
    vec4 ambient = light.ambient * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 diffuse = light.diffuse * diff * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 specular = light.specular * spec * (material.is_specular ? texture(material.specular, TexCoord) : material.specular_base);

    diffuse = diffuse * light.intensity;
    specular = specular * light.intensity;

    float shadow = 0.0;

    if(light.hasShadowMap) {
        float bias = max(0.09 * (1.0 - dot(normal, lightDir)), 0.005);      
        shadow = ShadowCalculation(light, fs_in.FragPosLightSpace, bias);  
    }

    return vec4(vec3(ambient + (1.0 - shadow) * (specular + diffuse)), 1.0);
}  

vec4 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = fs_in.TBN * normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance,2.0));    


    if (diff == 0.0)
        spec = 0.0;

    // combine results
    vec4 ambient  = light.ambient  * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 diffuse  = light.diffuse  * diff * (material.is_diffuse ? texture(material.diffuse, TexCoord) : material.diffuse_base);
    vec4 specular = light.specular * spec * (material.is_specular ? texture(material.specular, TexCoord) : material.specular_base);
    
    float spot_smooth = smoothstep(0.0, 1.0, (light.range - distance) / 10);
    diffuse *= spot_smooth;
    specular *= spot_smooth;

    ambient  = ambient * attenuation;
    diffuse  = diffuse * attenuation * light.intensity;
    specular = specular * attenuation * light.intensity;
    return vec4(vec3(diffuse + specular + ambient), 1.0);
} 

vec4 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
  vec3 lightDir = fs_in.TBN * normalize(light.position - fragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  
  float theta = dot(lightDir, fs_in.TBN * normalize(-light.direction));

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

  if (diff == 0.0)
      spec = 0.0;

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

  return vec4(vec3(diffuse + specular), 1.0);
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
      vec3 viewDir = fs_in.TBN * normalize(-FragPos);
      vec4 result = vec4(0.0);

      // normal map processing
      if (material.is_normal) {
        norm = normalize(texture(material.normal, TexCoord).rgb * 2.0 - 1.0);
      }
      
      for (int i = 0; i < lights_count; i++) {
        if (int(lights[i].type) == 1)
            result += CalcPointLight(lights[i], norm, FragPos, viewDir);
        else if (int(lights[i].type) == 2)
            result += CalcSpotLight(lights[i], norm, FragPos, viewDir);
        else if(int(lights[i].type) == 3)
            result += CalcDirLight(lights[i], norm, viewDir);
      }

      result.w = texture(material.diffuse, TexCoord).w;

      switch (displayMode)
      {
          case 0: FragColor = result; break; // Scene View
          case 1: FragColor = vec4(norm * 0.5 + 0.5, 1.0); break; // Normals_TS
          case 2: FragColor = vec4(Normal, 1.0); break; // Normals_WS
          case 3: FragColor = vec4(texture(material.normal, TexCoord).rgb, 1.0); break; // Normal Texture
          case 4: FragColor = vec4(fs_in.Tangent * 0.5 + 0.5, 1.0); break; // Tangents
          case 5: FragColor = vec4(fs_in.Bitangent * 0.5 + 0.5, 1.0); break; // Bitangents
      }
    }
    else 
    {
      if(material.is_diffuse)
        FragColor = texture(material.diffuse, TexCoord);// + material.diffuse_base;
      else
        FragColor = material.diffuse_base;
    } 

    
    if (fog_distance > 0)
        FragColor += LinearizeDepth(gl_FragCoord.z) / fog_distance;

    float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
}
