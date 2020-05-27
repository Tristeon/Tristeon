#version 330
in vec2 texCoord;

struct TileSet
{
    sampler2D texture;
    int cols;
    int rows;

    int spacingLeft;
    int spacingRight;
    int spacingTop;
    int spacingBottom;

    int horizontalSpacing;
    int verticalSpacing;

    int id;
};
uniform TileSet tileSet;

struct Level
{
    samplerBuffer data;
    int width;
    int height;
    
    int tileRenderWidth;
    int tileRenderHeight;
};
uniform Level level;

struct CameraData
{
    int posX;
    int posY;

    int pixelsX;
    int pixelsY;

    //The actual width/height of the game view
    int displayPixelsX;
    int displayPixelsY;

    float zoom;

    bool showGrid;
};
uniform CameraData camera;

out vec4 FragColor;

bool showGrid();
vec2 getTileUV(vec2 uv, int x, int y);
ivec2 tileTo2DIndex(int tile);
void main()
{
    //Determine which tile we're on using the camera's properties
    //Normalized tile... is the size of tiles within the 0..1 range of the screen
    float normalizedTileWidth = (float)level.tileRenderWidth / (camera.pixelsX / camera.zoom); 
    float normalizedTileHeight = (float)level.tileRenderHeight / (camera.pixelsY / camera.zoom);

    vec2 coords = texCoord;
    //Move the coords by -0.5 to center the tiles for accurate zooming
    coords.x -= 0.5f;
    coords.y -= 0.5f;
    //Then slightly adjust to make tiles centered again
    coords.x += normalizedTileWidth / 2.0f;
    coords.y += normalizedTileHeight / 2.0f;

    //Move the coords by the camera position (scaled by zoom)
    coords.x += (float)camera.posX / (camera.pixelsX / camera.zoom);
    coords.y += (float)camera.posY / (camera.pixelsY / camera.zoom);
    
    //Calculate tile x,y by dividing the adjusted texcoords by the 0..1 tile size
    float tileX = (coords.x / normalizedTileWidth);
    float tileY = (coords.y / normalizedTileHeight);

    //Calculate data index based on tileX and tileY
    int dataX = (int)floor(tileX);
    int dataY = (int)floor(tileY);
    int dataIndex = dataY * level.width + dataX;
    if (dataX >= level.width || dataY >= level.height || dataX < 0 || dataY < 0) 
        discard; //Discard all out of map tiles

    if (camera.showGrid && showGrid())
        return;

    //Convert data tile to tileset index (data stored in a 1D array of 2 integers per tile, first describes the index, the second describes the tileset id)
    int tileSetValue = floatBitsToInt(texelFetch(level.data, dataIndex * 2 + 1).r);
    if (tileSetValue != tileSet.id)
        discard;
    int dataValue = floatBitsToInt(texelFetch(level.data, dataIndex * 2).r);
    ivec2 tileIndex = tileTo2DIndex(dataValue);
    if (tileIndex.x == -1 || tileIndex.y == -1)
        discard; //Discard empty tiles

    //Calculate tile u,v by taking the leftover decimals in tileX and tileY
    float tileU = mod(tileX, 1);
    float tileV = mod(tileY, 1);

    //Convert UVs to tileset space
    vec2 tileSetUV = getTileUV(vec2(tileU, tileV), tileIndex.x, tileIndex.y);
    FragColor = texture2D(tileSet.texture, tileSetUV);
}

bool showGrid()
{
    float normalizedTileWidth = (float)level.tileRenderWidth / (camera.pixelsX / camera.zoom); 
    float normalizedTileHeight = (float)level.tileRenderHeight / (camera.pixelsY / camera.zoom);

    vec2 coords = texCoord;
    //Move the coords by -0.5 to center the tiles for accurate zooming
    coords.x -= 0.5f;
    coords.y -= 0.5f;
    //Then slightly adjust to make tiles centered again
    coords.x += normalizedTileWidth / 2.0f;
    coords.y += normalizedTileHeight / 2.0f;

    int x = abs(int(coords.x * camera.pixelsX + camera.posX * camera.zoom));
    int y = abs(int(coords.y * camera.pixelsY + camera.posY * camera.zoom));

    float errorMarginX = 1.0f / (camera.displayPixelsX / float(camera.pixelsX));
    float errorMarginY = 1.0f / (camera.displayPixelsY / float(camera.pixelsY));

    int pixelIntervalX = int(level.tileRenderWidth * camera.zoom);
    int pixelIntervalY = int(level.tileRenderHeight * camera.zoom);
    bool result = false;

    bool is0Line = x < (errorMarginX * 2) || y < (errorMarginX * 2);
    bool is10Line = x % (pixelIntervalX * 10) < errorMarginX || y % (pixelIntervalY * 10) < errorMarginY;
    if (is0Line || is10Line)
    {
        FragColor = vec4(1, 1, 1, 1);
        result = true;
        return true;
    }

    if (camera.zoom < 0.15f)
        return false;

    if (x % pixelIntervalX < errorMarginX && y % (pixelIntervalY / 10) < errorMarginX)
    {
        FragColor = vec4(1, 1, 1, 1);
        result = true;
    }
    if (y % pixelIntervalY < errorMarginY && x % (pixelIntervalX / 10) < errorMarginY)
    {
        FragColor = vec4(1, 1, 1, 1);
        result = true;      
    }

    return result;
}

ivec2 tileTo2DIndex(int tile)
{
    if (tile == -1)
        return ivec2(-1, -1);

    int x = tile % tileSet.cols;
    int y = tile / tileSet.cols;

    return ivec2(x, y);
}

vec2 getTileUV(vec2 uv, int tileX, int tileY)
{
    //Coords beyond our tileset mess up spacing so we clamp them
    tileX = tileX % tileSet.cols;
    tileY = tileSet.rows - 1 - (tileY % tileSet.rows);
    ivec2 texSize = textureSize(tileSet.texture, 0);

    //Determine the amount of pixels per tile
    int tilePixelsX = (texSize.x - ((tileSet.spacingLeft + tileSet.spacingRight) + ((tileSet.cols - 1) * tileSet.horizontalSpacing))) / tileSet.cols;
    int tilePixelsY = (texSize.y - ((tileSet.spacingTop + tileSet.spacingBottom) + ((tileSet.rows - 1) * tileSet.verticalSpacing))) / tileSet.rows;

    //Determine the start point of the tile depending on spacing
    int startX = tileSet.spacingLeft + (tileX * tilePixelsX) + (tileX * tileSet.horizontalSpacing);
    int startY = tileSet.spacingBottom + (tileY * tilePixelsY) + (tileY * tileSet.verticalSpacing);

    //Scale UV to tile coords, then normalize into texture coords
    float x = ((uv.x * tilePixelsX) / texSize.x);
    float y = ((uv.y * tilePixelsY) / texSize.y);

    //Add start pixels, also scaled into normalized texture coords
    float u = x + (startX / (float)texSize.x);
    float v = y + (startY / (float)texSize.y);
    
    return vec2(u, v);
}