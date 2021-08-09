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
    float intensity;
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
    
    diffuse = diffuse * light.intensity;
    specular = diffuse * light.intensity;

    vec4 resultLight = vec4(emission + ambient + diffuse + specular, 1.0f);
    
    if(useFillColor) {
        color = fillColor * resultLight;
    }
    else {
        color = texture(ourTexture, TexCoord) * resultLight;
    }
}