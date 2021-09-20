#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoord = position;
    gl_Position = projection * view * vec4(position, 1.0);
}  

#SHADER FRAGMENT
#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube cubemap;

void main()
{    
    FragColor = texture(cubemap, TexCoord);
}
