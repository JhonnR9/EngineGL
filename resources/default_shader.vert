#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aInstanceColor;

layout(location = 3) in mat4 aInstanceMVP;

out vec2 texCoord;
out vec3 vColor;

void main()
{
    gl_Position = aInstanceMVP * vec4(aPos, 0.0, 1.0);
    texCoord = aTexCoord;
    vColor = aInstanceColor;
}