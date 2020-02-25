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
    int levelWidth = 10;
    int levelHeight = 10;

    int[] data = int[] (
        1, 1, 1
    );

    FragColor = texture2D(tileSet, getTileUV(texCoord, 2, 4));
    return;

    float tileX = texCoord.x * cameraWidth;
    float tileY = texCoord.y * cameraHeight;

    float tileU = (tileX - floor(tileX));
    float tileV = (tileY - floor(tileY));
    
    float tileSetTileX = tileU / 16 + ((int)(tileX) / 16.0f);
    float tileSetTileY = tileV / 16 + ((int)(tileY) / 16.0f);

    float tileSetTileU = (tileSetTileX - floor(tileSetTileX));
    float tileSetTileV = (tileSetTileY - floor(tileSetTileY));

    FragColor = texture2D(tileSet, vec2(tileU, tileV));
}

ivec2 getTileIndexFromData(int[100] data)
{
    int index = 0;

    if (data[index] == -1)
        return ivec2(-1, -1);
}

vec2 getTileUV(vec2 uv, int tileX, int tileY)
{
    float x = uv.x / tileSetWidth;
    float y = uv.y / tileSetHeight;

    float u = (tileX / tileSetWidth) + x;
    float v = (tileY / tileSetHeight) + y;
    return vec2(u, v);
}