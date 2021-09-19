#SHADER VERTEX
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    TexCoord = texCoord;
}

#SHADER FRAGMENT
#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

const float offset = 1.0 / 300.0;

void main()
{
  vec2 offsets[9] = vec2[](
    vec2(-offset, offset), // top-left
    vec2(0.0f, offset), // top-center
    vec2(offset, offset), // top-right
    vec2(-offset, 0.0f),   // center-left
    vec2(0.0f, 0.0f),   // center-center
    vec2(offset, 0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2(0.0f, -offset), // bottom-center
    vec2(offset, -offset)  // bottom-right    
    );

  float kernel[9] = float[](
    1, 1, 1,
    1, -8, 1,
    1, 1, 1
    );

  vec3 sampleTex[9];
  for (int i = 0; i < 9; i++)
  {
    sampleTex[i] = vec3(texture(ourTexture, TexCoord.st + offsets[i]));
  }
  vec3 col = vec3(0.0);
  for (int i = 0; i < 9; i++)
    col += sampleTex[i] * kernel[i];

  color = vec4(col, 1.0);
}