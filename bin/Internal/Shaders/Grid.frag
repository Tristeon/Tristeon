#version 330
in vec2 texCoord;

struct Grid 
{
    int tileWidth;
    int tileHeight;
};

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
};

uniform Grid grid;
uniform CameraData camera;

out vec4 FragColor;

void main()
{
    float normalizedTileWidth = (float)grid.tileWidth / (camera.pixelsX / camera.zoom); 
    float normalizedTileHeight = (float)grid.tileHeight / (camera.pixelsY / camera.zoom);

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

    int pixelIntervalX = int(grid.tileWidth * camera.zoom);
    int pixelIntervalY = int(grid.tileHeight * camera.zoom);

    bool is0Line = x < (errorMarginX * 2) || y < (errorMarginX * 2);
    if (is0Line)
    {
        FragColor = vec4(1, 1, 1, 1);
        return;
    }
    
    bool is10Line = x % (pixelIntervalX * 10) < errorMarginX || y % (pixelIntervalY * 10) < errorMarginY;
    if (is10Line)
    {
        FragColor = vec4(1, 1, 1, camera.zoom * 10);
        return;
    }

    if (x % pixelIntervalX < errorMarginX)
    {
        FragColor = vec4(1, 1, 1, camera.zoom);
        return;
    }
    if (y % pixelIntervalY < errorMarginY)
    {
        FragColor = vec4(1, 1, 1, camera.zoom);
        return;
    }

    discard;
}