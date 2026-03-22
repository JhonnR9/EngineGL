#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 3) in mat4 aInstanceMVP;
layout(location = 7) in vec4 aInstanceColor;
layout(location = 8) in vec4 aRegion;
layout(location = 9) in float aTexIndex;

out vec2 texCoord;
out vec4 vColor;
out float vTexIndex;

void main()
{
    gl_Position = aInstanceMVP * vec4(aPos, 0.0, 1.0);

    texCoord = (aTexCoord * aRegion.zw) + aRegion.xy;

    vColor = aInstanceColor;

    vTexIndex = aTexIndex;
}