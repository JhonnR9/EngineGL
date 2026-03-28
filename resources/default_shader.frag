#version 330 core

in vec2 texCoord;
in vec4 vColor;
in float vTexIndex;

in vec2 vLocalPos;
flat in int vShapeType;

out vec4 FragColor;

uniform sampler2D u_Textures[16];

void main()
{
    if (vShapeType == 1) // Circle
    {
        vec2 p = vLocalPos * 2.0;

        float d = length(p);

        float aa = fwidth(d);
        float edge = smoothstep(1.0, 1.0 - aa, d);

        if (d > 1.0)
        discard;

        FragColor = vec4(vColor.rgb, vColor.a * edge);
        return;
    }
    else if (vShapeType == 2) // Line
    {
        float dist = abs(vLocalPos.y);
        float halfThickness = 0.5;

        float aa = fwidth(dist);
        float edge = smoothstep(halfThickness, halfThickness - aa, dist);

        if (dist > halfThickness)
        discard;

        FragColor = vec4(vColor.rgb, vColor.a * edge);
        return;
    }

    int index = int(vTexIndex);
    FragColor = texture(u_Textures[index], texCoord) * vColor;
}
