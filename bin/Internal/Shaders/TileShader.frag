#version 330
in vec2 texCoord;

struct TileSet
{
    sampler2D texture;
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
uniform TileSet tileSet;

struct Level
{
    isamplerBuffer data;
    uint width;
    uint height;
    
    uint tileRenderWidth;
    uint tileRenderHeight;
};
uniform Level level;

struct CameraData
{
    int posX;
    int posY;

    uint pixelsX;
    uint pixelsY;

    uint displayPixelsX;
    uint displayPixelsY;

    float zoom;
};
uniform CameraData camera;

out vec4 FragColor;

vec2 getTileUV(vec2 uv, uint x, uint y);
ivec2 tileTo2DIndex(int tile);
void main()
{
    //Determine which tile we're on using the camera's properties
    //Normalized tile... is the size of tiles within the 0..1 range of the screen
    float normalizedTileWidth = float(level.tileRenderWidth) / (camera.pixelsX / camera.zoom); 
    float normalizedTileHeight = float(level.tileRenderHeight) / (camera.pixelsY / camera.zoom);

    vec2 coords = texCoord;
    //Move the coords by -0.5 to center the tiles for accurate zooming
    coords.x -= 0.5f;
    coords.y -= 0.5f;
    //Then slightly adjust to make tiles centered again
    coords.x += normalizedTileWidth / 2.0f;
    coords.y += normalizedTileHeight / 2.0f;

    //Move the coords by the camera position (scaled by zoom)
    coords.x += float(camera.posX) / (camera.pixelsX / camera.zoom);
    coords.y += float(camera.posY) / (camera.pixelsY / camera.zoom);
    
    //Calculate tile x,y by dividing the adjusted texcoords by the 0..1 tile size
    float tileX = (coords.x / normalizedTileWidth);
    float tileY = (coords.y / normalizedTileHeight);
    if (tileX >= level.width || tileY >= level.height || tileX < 0 || tileY < 0) 
        discard; //Discard all out of map tiles

    //Calculate data index based on tileX and tileY
    uint dataX = uint(floor(tileX));
    uint dataY = uint(floor(tileY));
    uint dataIndex = dataY * level.width + dataX;

    //Convert data tile to tileset index (data stored in a 1D array of 2 integers per tile, first describes the index, the second describes the tileset id)
    int tileSetValue = texelFetch(level.data, int(dataIndex) * 2 + 1).r;
    if (tileSetValue != int(tileSet.id))
        discard;
    int dataValue = texelFetch(level.data, int(dataIndex) * 2).r;
    ivec2 tileIndex = tileTo2DIndex(dataValue);
    if (tileIndex.x == -1 || tileIndex.y == -1)
        discard; //Discard empty tiles

    //Calculate tile u,v by taking the leftover decimals in tileX and tileY
    float tileU = mod(tileX, 1.0f);
    float tileV = mod(tileY, 1.0f);

    //Convert UVs to tileset space
    vec2 tileSetUV = getTileUV(vec2(tileU, tileV), uint(tileIndex.x), uint(tileIndex.y));
    FragColor = texture(tileSet.texture, tileSetUV);
}

ivec2 tileTo2DIndex(int tile)
{
    if (tile == -1)
        return ivec2(-1, -1);

    int x = tile % int(tileSet.cols);
    int y = tile / int(tileSet.cols);

    return ivec2(x, y);
}

vec2 getTileUV(vec2 uv, uint tileX, uint tileY)
{
    //Coords beyond our tileset mess up spacing so we clamp them
    tileX = tileX % tileSet.cols;
    tileY = tileSet.rows - uint(1) - (tileY % tileSet.rows);
    ivec2 texSize = textureSize(tileSet.texture, 0);

    //Determine the amount of pixels per tile
    uint tilePixelsX = (uint(texSize.x) - ((tileSet.spacingLeft + tileSet.spacingRight) + ((tileSet.cols - uint(1)) * tileSet.horizontalSpacing))) / tileSet.cols;
    uint tilePixelsY = (uint(texSize.y) - ((tileSet.spacingTop + tileSet.spacingBottom) + ((tileSet.rows - uint(1)) * tileSet.verticalSpacing))) / tileSet.rows;

    //Determine the start point of the tile depending on spacing
    uint startX = tileSet.spacingLeft + (tileX * tilePixelsX) + (tileX * tileSet.horizontalSpacing);
    uint startY = tileSet.spacingBottom + (tileY * tilePixelsY) + (tileY * tileSet.verticalSpacing);

    //Scale UV to tile coords, then normalize into texture coords
    float x = ((uv.x * tilePixelsX) / texSize.x);
    float y = ((uv.y * tilePixelsY) / texSize.y);

    //Add start pixels, also scaled into normalized texture coords
    float u = x + (startX / float(texSize.x));
    float v = y + (startY / float(texSize.y));
    
    return vec2(u, v);
}
