#version 140
in vec2 texCoord;

out vec4 outAlbedo;
out vec4 outNormal;
out vec4 outPosition;

//Sprite
uniform sampler2D mainTex;
struct Sprite
{
    uint width;
    uint height;

    vec4 colour;

    bool flipX;
    bool flipY;
};
uniform Sprite sprite;

void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    vec2 coords = texCoord;
    if (sprite.flipX)
        coords.x = 1 - coords.x;
    if (sprite.flipY)
        coords.y = 1 - coords.y;

    outAlbedo = texture2D(mainTex, coords) * sprite.colour;
    outNormal = vec4(1, 1, 1, 1);
    outPosition = vec4(1, 1, 1, 1); 
}