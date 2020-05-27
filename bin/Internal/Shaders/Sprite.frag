#version 330
in vec2 texCoord;
out vec4 FragColor;

//Sprite
uniform sampler2D mainTex;
struct Sprite
{
    int width;
    int height;

    vec4 colour;

    bool flipX;
    bool flipY;
};
uniform Sprite sprite;

void drawSprite();
void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    vec2 coords = texCoord;
    if (sprite.flipX)
        coords.x = 1 - coords.x;
    if (sprite.flipY)
        coords.y = 1 - coords.y;

    FragColor = texture2D(mainTex, coords) * sprite.colour;
}