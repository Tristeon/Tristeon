#version 140
in vec2 texCoord;
uniform sampler2D original;
uniform sampler2D previous;

out vec4 FragColor;

void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    vec4 colour = texture2D(previous, texCoord);
    float grey = colour.r * 0.3 + colour.g * 0.59 + colour.b * 0.11;
    FragColor = vec4(grey, grey, grey, 1.0);
}