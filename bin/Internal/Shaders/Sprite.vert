#version 150

//Sprite
struct Sprite
{
    int width;
    int height;
    int renderMode;
};
uniform Sprite sprite;

struct Actor
{
    vec2 position;
    vec2 scale;
    float rotation;
};
uniform Actor actor;

//Camera
struct CameraData
{
    int posX;
    int posY;

    int pixelsX;
    int pixelsY;

    float zoom;
};
uniform CameraData camera;
 
out vec2 texCoord;
void main()
{
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    texCoord.x = (x+1.0)*0.5;
    texCoord.y = (y+1.0)*0.5;

    x = x / float(camera.pixelsX / camera.zoom) * sprite.width;
    y = y / float(camera.pixelsY / camera.zoom) * sprite.height;

    x -= float(camera.posX) / camera.pixelsX * camera.zoom * 2;
    y -= float(camera.posY) / camera.pixelsY * camera.zoom * 2;

    x += actor.position.x / camera.pixelsX * camera.zoom * 2;
    y += actor.position.y / camera.pixelsY * camera.zoom * 2;

    gl_Position = vec4(x, y, 0, 1);
}