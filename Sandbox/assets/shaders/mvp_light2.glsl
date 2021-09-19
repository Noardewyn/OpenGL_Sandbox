#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 Normal;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * normal;
    LightPos = vec3(view * vec4(lightPos, 1.0));
    TexCoord = texCoord;
}

#SHADER FRAGMENT
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    vec3      emissionStrength;
    float     shininess;
}; 

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform bool useFillColor;
uniform vec4 fillColor;

uniform sampler2D ourTexture;
uniform Material material;
uniform Light light;

void main()
{
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));  

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)); 

    // ambient light
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

    // emission light
    
    vec3 useEmission = step(vec3(texture(material.specular, TexCoord)), vec3(0.0f, 0.0f, 0.0f));
    
    vec3 emission = useEmission * material.emissionStrength * vec3(texture(material.emission, TexCoord));

    vec4 resultLight = vec4(emission + ambient + diffuse + specular, 1.0f);
    
    if(useFillColor) {
        color = fillColor * resultLight;
    }
    else {
        color = texture(ourTexture, TexCoord) * resultLight;
    }
}