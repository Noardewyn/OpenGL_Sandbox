#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.0);
}

#SHADER FRAGMENT
#version 330 core

out vec4 color;

void main()
{      
    gl_FragDepth = gl_FragCoord.z;
    color = vec4(vec3(gl_FragCoord.z), 1.0);
}  
