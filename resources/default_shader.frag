#version 330 core
in vec2 texCoord;
in vec4 vColor;

out vec4 FragColor;

uniform sampler2D texture1;


void main()
{
    FragColor = texture(texture1, texCoord) * vColor;
}