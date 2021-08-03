#version 330 core

out vec4 out_color;

uniform vec2 iResolution;
uniform float iTime;

float circle(vec2 uv, float r, float blur)
{
	float d = length(uv);
    float c = smoothstep(r, r - blur, d);
    return c;
}

void main()
{
    vec2 uv = gl_FragCoord.xy/iResolution.xy;
    
	uv -= 0.5;
    uv.x *= iResolution.x / iResolution.y;
    
    float c = circle(uv, 0.4, 0.1);
    c -= circle(uv, 0.2, 0.1);
    out_color = vec4(c * (uv.x + 0.5) * cos(iTime), c * (uv.y + 0.5) * cos(iTime), c * sin(iTime), 1.0);
}