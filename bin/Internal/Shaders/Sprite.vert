#version 330

//Sprite
struct Sprite
{
    int width;
    int height;

    vec4 colour;

    bool flipX;
    bool flipY;
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

const float PI = 3.14159265;
const float toRAD = PI / 180.0f;

void main()
{
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    texCoord.x = (x+1.0)*0.5;
    texCoord.y = (y+1.0)*0.5;

    float rad = actor.rotation * toRAD;
    float rotX = x * cos(rad) - y * sin(rad);
    float rotY = y * cos(rad) + x * sin(rad);
    x = rotX;
    y = rotY;

    x = x / float(camera.pixelsX / camera.zoom) * sprite.width * actor.scale.x;
    y = y / float(camera.pixelsY / camera.zoom) * sprite.height * actor.scale.y;

    x -= float(camera.posX) / camera.pixelsX * camera.zoom * 2;
    y -= float(camera.posY) / camera.pixelsY * camera.zoom * 2;

    x += actor.position.x / camera.pixelsX * camera.zoom * 2;
    y += actor.position.y / camera.pixelsY * camera.zoom * 2;

    gl_Position = vec4(x, y, 0, 1);
}