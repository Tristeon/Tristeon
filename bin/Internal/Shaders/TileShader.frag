#version 140
#include Lighting.incl

in vec2 texCoord;

out vec4 fragColor;

struct Tileset
{
    sampler2D albedoMap;
    sampler2D normalMap;
    bool normalMapEnabled;

    uint cols;
    uint rows;

    uint spacingLeft;
    uint spacingRight;
    uint spacingTop;
    uint spacingBottom;

    uint horizontalSpacing;
    uint verticalSpacing;

    uint id;
};
uniform Tileset tileset;

struct Level
{
    isamplerBuffer data;
    uint columns;
    uint rows;
    
    uint tileRenderWidth;
    uint tileRenderHeight;
};
uniform Level level;

struct CameraData
{
    vec2 position;
    uvec2 displayPixels;
    float zoom;
};
uniform CameraData camera;

vec2 getTileUV(vec2 uv, uint x, uint y);
ivec2 tileTo2DIndex(int tile);
void main()
{
    //Determine which tile we're on using the camera's properties
    //Normalized tile... is the size of tiles within the 0..1 range of the screen
    float normalizedTileWidth = float(level.tileRenderWidth) / (camera.displayPixels.x / camera.zoom); 
    float normalizedTileHeight = float(level.tileRenderHeight) / (camera.displayPixels.y / camera.zoom);

    vec2 coords = texCoord;
    //Move the coords by -0.5 to center the tiles for accurate zooming
    coords.x -= 0.5f;
    coords.y -= 0.5f;
    //Then slightly adjust to make tiles centered again
    coords.x += normalizedTileWidth / 2.0f;
    coords.y += normalizedTileHeight / 2.0f;

    //Move the coords by the camera position (scaled by zoom)
    coords.x += float(camera.position.x) / (camera.displayPixels.x / camera.zoom);
    coords.y += float(camera.position.y) / (camera.displayPixels.y / camera.zoom);
    
    //Calculate tile x,y by dividing the adjusted texcoords by the 0..1 tile size
    float tileX = (coords.x / normalizedTileWidth);
    float tileY = (coords.y / normalizedTileHeight);
    if (tileX >= level.columns || tileY >= level.rows || tileX < 0 || tileY < 0) 
        discard; //Discard all out of map tiles

    //Calculate data index based on tileX and tileY
    uint dataX = uint(floor(tileX));
    uint dataY = uint(floor(tileY));
    uint dataIndex = dataY * level.columns + dataX;

    //Convert data tile to tileset index (data stored in a 1D array of 2 integers per tile, first describes the index, the second describes the tileset id)
    int tilesetValue = texelFetch(level.data, int(dataIndex) * 2 + 1).r;
    if (tilesetValue != int(tileset.id))
        discard;
    int dataValue = texelFetch(level.data, int(dataIndex) * 2).r;
    ivec2 tileIndex = tileTo2DIndex(dataValue);
    if (tileIndex.x == -1 || tileIndex.y == -1)
        discard; //Discard empty tiles

    //Calculate tile u,v by taking the leftover decimals in tileX and tileY
    float tileU = mod(tileX, 1.0f);
    float tileV = mod(tileY, 1.0f);

    //Convert UVs to tileset space
    vec2 tilesetUV = getTileUV(vec2(tileU, tileV), uint(tileIndex.x), uint(tileIndex.y));

    vec4 albedo = texture2D(tileset.albedoMap, tilesetUV);

    vec3 normal;
    if (tileset.normalMapEnabled)
    {
        normal = 2 * texture2D(tileset.normalMap, tilesetUV).rgb - 1.0;
        normal.z *= -1;
    }
    else
    {
        normal = vec3(0, 0, -1);
    }

    vec2 worldPos = vec2(tileX * level.tileRenderWidth - level.tileRenderWidth / 2.0f, tileY * level.tileRenderHeight - level.tileRenderHeight / 2.0f);

    fragColor = calculateLights(albedo, normal, vec3(worldPos, 0), camera.position);
}

ivec2 tileTo2DIndex(int tile)
{
    if (tile == -1)
        return ivec2(-1, -1);

    int x = tile % int(tileset.cols);
    int y = tile / int(tileset.cols);

    return ivec2(x, y);
}

vec2 getTileUV(vec2 uv, uint tileX, uint tileY)
{
    //Coords beyond our tileset mess up spacing so we clamp them
    tileX = tileX % tileset.cols;
    tileY = tileset.rows - uint(1) - (tileY % tileset.rows);
    ivec2 texSize = textureSize(tileset.albedoMap, 0);

    //Determine the amount of pixels per tile
    uint tilePixelsX = (uint(texSize.x) - ((tileset.spacingLeft + tileset.spacingRight) + ((tileset.cols - uint(1)) * tileset.horizontalSpacing))) / tileset.cols;
    uint tilePixelsY = (uint(texSize.y) - ((tileset.spacingTop + tileset.spacingBottom) + ((tileset.rows - uint(1)) * tileset.verticalSpacing))) / tileset.rows;

    //Determine the start point of the tile depending on spacing
    uint startX = tileset.spacingLeft + (tileX * tilePixelsX) + (tileX * tileset.horizontalSpacing);
    uint startY = tileset.spacingBottom + (tileY * tilePixelsY) + (tileY * tileset.verticalSpacing);

    //Scale UV to tile coords, then normalize into texture coords
    float x = ((uv.x * tilePixelsX) / texSize.x);
    float y = ((uv.y * tilePixelsY) / texSize.y);

    //Add start pixels, also scaled into normalized texture coords
    float u = x + (startX / float(texSize.x));
    float v = y + (startY / float(texSize.y));
    
    return vec2(u, v);
}