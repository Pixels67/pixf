#version 330 core

@VERTEX

@pos in vec3 aPos;
@uv in vec2 aUv;

out vec2 uv;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    uv = aUv;
}

@FRAGMENT

uniform sampler2D tex;

in vec2 uv;
out vec4 FragColor;

void main()
{
    FragColor = texture(tex, uv);
}