#version 330 core
in vec2 texCoord;
in vec3 vColor;

out vec4 FragColor;

uniform sampler2D texture1;
//uniform sampler2D textures[16];


void main()
{
    FragColor = texture(texture1, texCoord) * vec4(vColor, 1.0);
}