#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inCol;

out vec2 uv;
out vec3 vColor;

uniform vec2 uPos;
uniform vec2 uSize;

void main()
{
    vec2 scaled = inPos * uSize;
    vec2 moved  = scaled + uPos;

    gl_Position = vec4(moved, 0.0, 1.0);
    uv     = inUV;
    vColor = inCol;
}
