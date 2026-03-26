#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTranslation;
layout(location = 2) in vec2 aOrigin;
layout(location = 3) in float aRotation;
layout(location = 4) in vec2 aScale;
layout(location = 5) in vec4 aInstanceColor;
layout(location = 6) in int aShapeType;
layout(location = 7) in float aZIndex;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vLocalPos;
flat out int vShapeType;
out vec2 vScale;

void main()
{
    vec2 pivotOffset = (aOrigin - vec2(0.5)) * aScale;
    vec2 pos = aPos * aScale - pivotOffset;

    float rad = radians(aRotation);
    float cosR = cos(rad);
    float sinR = sin(rad);

    vec2 rotatedPos;
    rotatedPos.x = pos.x * cosR - pos.y * sinR;
    rotatedPos.y = pos.x * sinR + pos.y * cosR;

    vec2 finalPos = rotatedPos + aTranslation;
    gl_Position = uViewProjection * vec4(finalPos, aZIndex, 1.0);

    vColor = aInstanceColor;
    vLocalPos = aPos; // -0.5 → 0.5
    vShapeType = aShapeType;
    vScale = aScale;
}
