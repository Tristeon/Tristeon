#version 140
#include Lighting.incl

in vec2 texCoord;
in vec2 worldPos;

out vec4 fragColor;

//Sprite
uniform sampler2D albedoMap;

uniform sampler2D normalMap;
uniform bool normalMapEnabled;
uniform float normalMapStrength;

uniform sampler2D lightMask;
uniform bool lightMaskEnabled;

struct Sprite
{
    uint width;
    uint height;

    vec4 colour;

    bool flipX;
    bool flipY;
};
uniform Sprite sprite;

//Camera
struct CameraData
{
    vec2 position;
    uvec2 displayPixels;
    float zoom;
};
uniform CameraData camera;

void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    vec2 coords = texCoord;
    if (sprite.flipX)
        coords.x = 1 - coords.x;
    if (sprite.flipY)
        coords.y = 1 - coords.y;

    vec4 tex = texture2D(albedoMap, coords);
    vec4 albedo = tex * sprite.colour;
    
    vec3 normal;
    if (normalMapEnabled)
    {
        normal = 2 * texture2D(normalMap, coords).rgb - 1.0;
        normal.z *= -1;

        normal = mix(vec3(0, 0, -1), normal, normalMapStrength);
    }
    else
    {
        normal = vec3(0, 0, -1);
    }

    float mask = 1;
    if (lightMaskEnabled)
        mask = texture2D(lightMask, coords).r;

    fragColor = calculateLights(albedo, normal, vec3(worldPos, 0), camera.position, mask);
}