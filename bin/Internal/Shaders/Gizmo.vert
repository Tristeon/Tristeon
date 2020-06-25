#version 330
layout (location = 0) in vec2 pos;

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

void main()
{
    float x = 0;
    float y = 0;

    x -= float(camera.posX) / camera.pixelsX * camera.zoom * 2;
    y -= float(camera.posY) / camera.pixelsY * camera.zoom * 2;

    x += pos.x / camera.pixelsX * camera.zoom * 2;
    y += pos.y / camera.pixelsY * camera.zoom * 2;

    gl_Position = vec4(x, y, 0, 1);
}