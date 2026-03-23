#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 3) in vec2 aTranslation;
layout(location = 4) in vec2 aOrigin;
layout(location = 5) in float aRotation;
layout(location = 6) in vec2 aScale;
layout(location = 7) in vec4 aInstanceColor;
layout(location = 8) in vec4 aRegion;
layout(location = 9) in float aTexIndex;
layout(location = 10) in int aFlip;

uniform mat4 uProjection;

out vec2 texCoord;
out vec4 vColor;
out float vTexIndex;

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


    texCoord = aTexCoord * aRegion.zw + aRegion.xy;


    if ((aFlip & 1) == 1) {
        texCoord.x = aRegion.x + aRegion.z - (texCoord.x - aRegion.x);
    }


    if ((aFlip & 2) == 2) {
        texCoord.y = aRegion.y + aRegion.w - (texCoord.y - aRegion.y);
    }

    vColor = aInstanceColor;
    vTexIndex = aTexIndex;
}
