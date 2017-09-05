#version 330
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 Texcoords;

out vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    texcoord = Texcoords;
}