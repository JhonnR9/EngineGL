#version 330 core

in vec4 vColor;
in vec2 vLocalPos;
flat in int vShapeType;

out vec4 FragColor;

void main()
{
    if (vShapeType == 1)
    {
        vec2 p = vLocalPos * 2.0;

        float distSq = dot(p, p);
        float d = sqrt(distSq);

        // AA adaptativo
        float aa = fwidth(d);

        float edge = smoothstep(1.0, 1.0 - aa, d);

        if (d > 1.0)
        discard;

        FragColor = vec4(vColor.rgb, vColor.a * edge);
    }
    else
    {
        FragColor = vColor;
    }
}
