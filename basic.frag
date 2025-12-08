#version 330 core

in vec2 uv;
in vec3 vColor;

out vec4 outColor;

uniform sampler2D tex;
uniform bool useTexture;
uniform vec3 solidColor;

void main()
{
    if (useTexture)
        outColor = texture(tex, uv);
    else
        outColor = vec4(solidColor, 1.0);
}
