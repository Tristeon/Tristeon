#version 140
in vec2 pos;

struct CameraData
{
    vec2 position;
    uvec2 displayPixels;
    float zoom;
};
uniform CameraData camera;

void main()
{
    float x = 0;
    float y = 0;

    x -= float(camera.position.x) / camera.displayPixels.x * camera.zoom * 2;
    y -= float(camera.position.y) / camera.displayPixels.y * camera.zoom * 2;

    x += pos.x / camera.displayPixels.x * camera.zoom * 2;
    y += pos.y / camera.displayPixels.y * camera.zoom * 2;

    gl_Position = vec4(x, y, 0, 1);
}