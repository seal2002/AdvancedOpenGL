# version 330 core
in vec2 texcoord;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    FragColor = vec4(texture(screenTexture, texcoord));
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
}