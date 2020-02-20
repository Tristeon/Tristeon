#version 150
in vec2 texCoord;

uniform sampler2D tileSet;
uniform int tileSetWidth;
uniform int tileSetHeight;

uniform int levelWidth;
uniform int levelHeight;

uniform int[] levelData;

uniform vec2 cameraPos;
uniform float cameraWidth;
uniform float cameraHeight;

uniform int screenWidth;
uniform int screenHeight;

out vec4 FragColor;

vec2 getTileUV(vec2 uv, int x, int y);
void main()
{
    int[] data = int[](
        0, 0, 0, 18, 19, 20, 21, 0, 0, 0,
        0, 18, 19, 20, 19, 20, 19, 20, 21, 0,
        0, 49, 50, 50, 50, 50, 50, 50, 54, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    );

    FragColor = texture2D(tileSet, getTileUV(texCoord, 4, 4));

    // float tileX = texCoord.x * cameraWidth;
    // float tileY = texCoord.y * cameraHeight;

    // float tileU = (tileX - floor(tileX));
    // float tileV = (tileY - floor(tileY));
    
    // float tileSetTileX = tileU / 16 + ((int)(tileX) / 16.0f);
    // float tileSetTileY = tileV / 16 + ((int)(tileY) / 16.0f);

    // float tileSetTileU = (tileSetTileX - floor(tileSetTileX));
    // float tileSetTileV = (tileSetTileY - floor(tileSetTileY));

    // FragColor = texture2D(tileSet, vec2(tileSetTileU, tileSetTileV));
}

int getTileIndex(int tilePosX, int tilePosY)
{
    return 0;
}

vec2 getTileUV(vec2 uv, int tileX, int tileY)
{
    float x = uv.x * tileSetWidth;
    float y = uv.y * tileSetHeight;

    float u = (x - floor(x));
    float v = (y - floor(y));
    return vec2(u, v);
}