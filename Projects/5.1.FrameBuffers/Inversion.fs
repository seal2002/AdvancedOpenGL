# version 330 core
in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    FragColor = vec4(vec3(1.0 - texture(screenTexture, texcoord)), 1.0);
}