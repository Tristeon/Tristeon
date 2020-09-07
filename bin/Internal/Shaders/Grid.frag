#version 330
in vec2 texCoord;

struct Grid 
{
    uint tileWidth;
    uint tileHeight;
};

struct CameraData
{
    vec2 position;
    uvec2 displayPixels;
    float zoom;
};

uniform Grid grid;
uniform CameraData camera;

out vec4 FragColor;

void main()
{
    float normalizedTileWidth = float(grid.tileWidth) / (camera.displayPixels.x / camera.zoom); 
    float normalizedTileHeight = float(grid.tileHeight) / (camera.displayPixels.y / camera.zoom);

    vec2 coords = texCoord;
    //Move the coords by -0.5 to center the tiles for accurate zooming
    coords.x -= 0.5f;
    coords.y -= 0.5f;
    //Then slightly adjust to make tiles centered again
    coords.x += normalizedTileWidth / 2.0f;
    coords.y += normalizedTileHeight / 2.0f;

    float x = abs(coords.x * camera.displayPixels.x + camera.position.x * camera.zoom);
    float y = abs(coords.y * camera.displayPixels.y + camera.position.y * camera.zoom);

    float errorMarginX = 1.0f;
    float errorMarginY = 1.0f;

    float pixelIntervalX = float(grid.tileWidth * camera.zoom);
    float pixelIntervalY = float(grid.tileHeight * camera.zoom);

    bool is0Line = x < (errorMarginX * 2) || y < (errorMarginX * 2);
    if (is0Line)
    {
        FragColor = vec4(1, 1, 1, 1);
        return;
    }
    
    bool is10Line = mod(x, (pixelIntervalX * 10)) < errorMarginX || mod(y, (pixelIntervalY * 10)) < errorMarginY;
    if (is10Line)
    {
        FragColor = vec4(1, 1, 1, camera.zoom * 10);
        return;
    }

    if (mod(x, pixelIntervalX) < errorMarginX)
    {
        FragColor = vec4(1, 1, 1, camera.zoom);
        return;
    }
    if (mod(y, pixelIntervalY) < errorMarginY)
    {
        FragColor = vec4(1, 1, 1, camera.zoom);
        return;
    }

    discard;
}