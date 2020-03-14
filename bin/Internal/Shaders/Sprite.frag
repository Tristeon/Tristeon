#version 330
in vec2 texCoord;
out vec4 FragColor;

//Sprite
uniform sampler2D mainTex;
struct Sprite
{
    int width;
    int height;
};
uniform Sprite sprite;

void drawSprite();
void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    ivec2 texSize = textureSize(mainTex, 0);

    FragColor = texture2D(mainTex, texCoord);
}