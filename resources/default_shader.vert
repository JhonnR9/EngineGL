#version 330 core
layout(location = 1) in vec2 aTexCoord;
layout(location = 0) in vec2 aPos;

uniform mat4 MVP;
out vec2 texCoord;
void main()
{
    gl_Position = MVP * vec4(aPos, 0.0, 1.0);
    texCoord = aTexCoord;
}