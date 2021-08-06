#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * material.specular * spec;  

    // ambient light
    vec3 ambient = light.ambient * material.ambient;

    vec4 resultLight = vec4(ambient + diffuse + specular, 1.0f);
    
    if(useFillColor) {
        color = fillColor * resultLight;
    }
    else {
        color = texture(ourTexture, TexCoord) * resultLight;
    }
}