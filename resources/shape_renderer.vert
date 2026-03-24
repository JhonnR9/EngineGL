#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTranslation;
layout(location = 2) in vec2 aOrigin;
layout(location = 3) in float aRotation;
layout(location = 4) in vec2 aScale;
layout(location = 5) in vec4 aInstanceColor;

uniform mat4 uProjection;

out vec4 vColor;

void main()
{
    vec2 pivotOffset = aOrigin * aScale;
    vec2 pos = aPos * aScale - pivotOffset;

    float rad = radians(aRotation);
    float cosR = cos(rad);
    float sinR = sin(rad);

    vec2 rotatedPos;
    rotatedPos.x = pos.x * cosR - pos.y * sinR;
    rotatedPos.y = pos.x * sinR + pos.y * cosR;

    vec2 finalPos = rotatedPos + aTranslation;
    gl_Position = uProjection * vec4(finalPos, 0.0, 1.0);

    vColor = aInstanceColor;
}