#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
}

#SHADER FRAGMENT
#version 330 core

out vec4 outColor;

uniform vec4 color;

void main()
{
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}
