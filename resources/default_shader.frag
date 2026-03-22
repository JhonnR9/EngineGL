#version 330 core

in vec2 texCoord;
in vec4 vColor;
in float vTexIndex;

out vec4 FragColor;


uniform sampler2D u_Textures[16];

void main()
{
    int index = int(vTexIndex);

    FragColor = texture(u_Textures[index], texCoord) * vColor;
}