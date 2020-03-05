#version 150
in vec2 texCoord;

uniform sampler2D tileSet;
uniform int tileSetCols;
uniform int tileSetRows;

uniform int[] levelData;

struct CameraData
{
    int posX;
    int posY;

    int pixelsX;
    int pixelsY;

    float zoom;
};
uniform CameraData camera;

out vec4 FragColor;

vec2 getTileUV(vec2 uv, int x, int y);
ivec2 tileTo2DIndex(int tile);
void main()
{
    //Level
    int levelWidth = 10;
    int levelHeight = 6;
    int[] data = int[] (
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         4,  4, -1, -1, -1, -1, -1, -1, -1, -1,
         9, 10, -1, -1, -1, -1,  4,  4, 14, 14,
        -1, -1, -1, -1, -1,  4, 10,  9,  4, 14,
         0,  1,  2,  0,  8,  3,  6,  3,  6,  8,
         3,  6,  3,  6,  3,  6,  3,  6,  3,  6
    );

    //Determine which tile we're on using the camera's properties
    ivec2 tileSetSize = textureSize(tileSet, 0);
    //Normalized tile... is the size of tiles within the 0..1 range of the screen
    float normalizedTileWidth = (float)tileSetSize.x / tileSetCols / (camera.pixelsX / camera.zoom); 
    float normalizedTileHeight = (float)tileSetSize.y / tileSetRows / (camera.pixelsY / camera.zoom);

    vec2 coords = texCoord;
    //Move the coords by -0.5 to center the tiles for accurate zooming
    coords.x -= 0.5f;
    coords.y -= 0.5f;
    //Move the coords by the camera position (scaled by zoom)
    coords.x += (float)camera.posX / (camera.pixelsX / camera.zoom);
    coords.y += (float)camera.posY / (camera.pixelsY / camera.zoom);
    
    //Calculate tile x,y by dividing the adjusted texcoords by the 0..1 tile size
    float tileX = (coords.x / normalizedTileWidth);
    float tileY = (coords.y / normalizedTileHeight);

    //Calculate tile u,v by taking the leftover decimals in tileX and tileY
    float tileU = (tileX - floor(tileX));
    float tileV = (tileY - floor(tileY));

    //Calculate data index based on tileX and tileY
    int dataX = (int)floor(tileX);
    int dataY = levelHeight - (int)floor(tileY) - 1; //Y needs to be flipped, -1 to start at "0"
    int dataIndex = dataY * levelWidth + dataX;
    if (dataX > levelWidth - 1 || dataY > levelHeight - 1 || dataX < 0 || dataY < 0) 
        discard; //Discard all out of map tiles

    //Convert data tile to tileset index
    ivec2 tileIndex = tileTo2DIndex(data[dataY * levelWidth + dataX]);
    if (tileIndex.x == -1 || tileIndex.y == -1)
        discard; //Discard empty tiles

    //Convert UVs to tileset space
    vec2 tileSetUV = getTileUV(vec2(tileU, tileV), tileIndex.x, tileIndex.y);
    FragColor = texture2D(tileSet, tileSetUV);
}

ivec2 tileTo2DIndex(int tile)
{
    if (tile == -1)
        return ivec2(-1, -1);

    int x = tile % tileSetCols;
    int y = tile / tileSetCols;

    return ivec2(x, y);
}

vec2 getTileUV(vec2 uv, int tileX, int tileY)
{
    float x = uv.x / tileSetCols;
    float y = uv.y / tileSetRows;

    float u = ((tileX) * 1.0 / tileSetCols) + x;
    float v = ((tileSetRows - tileY - 1) * 1.0 / tileSetRows) + y;
    return vec2(u, v);
}