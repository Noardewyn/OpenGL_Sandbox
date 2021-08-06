#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 ambientStrength;
uniform vec3 viewPos;
float specularStrength = 0.8;

void main()
{
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    // ambient light
    vec3 ambient = lightColor * ambientStrength;

    vec4 resultLight = vec4(ambient + diffuse + specular, 1.0f);
    
    color = texture(ourTexture, TexCoord) * resultLight;
}