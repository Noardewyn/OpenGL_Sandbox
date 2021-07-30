#version 330 core
layout (location = 0) in vec3 position;

out vec4 ourColor;

uniform vec4 color;

void main()
{
    gl_Position = vec4(position, 1.0f);
    ourColor = color;
}