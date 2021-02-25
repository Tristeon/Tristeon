#version 140
#include Lighting.incl
#include Includes/Actor.incl
#include Includes/Sprite.incl
#include Includes/SpriteTextures.incl
#include Includes/Camera.incl

in vec2 texCoord;
in vec2 worldPos;

out vec4 fragColor;

struct Spacing
{
    uint left;
    uint right;
    uint top;
    uint bottom;

    uint horizontalFrame;
    uint verticalFrame;
};
uniform Spacing spacing;

struct Animation
{
    uint cols;
    uint rows;
    uint frame;
};
uniform Animation animation;

void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    ivec2 texSize = textureSize(albedoMap, 0);

    uint frameX = animation.frame % animation.cols;
    uint frameY = animation.rows - uint(1) - (animation.frame / animation.cols);

    //Determine the amount of pixels per tile
    uint framePixelsX = (uint(texSize.x) - ((spacing.left + spacing.right) + ((animation.cols - uint(1)) * spacing.horizontalFrame))) / animation.cols;
    uint framePixelsY = (uint(texSize.y) - ((spacing.top + spacing.bottom) + ((animation.rows - uint(1)) * spacing.verticalFrame))) / animation.rows;

    //Determine the start point of the tile depending on spacing
    uint startX = spacing.left + (frameX * framePixelsX) + (frameX * spacing.horizontalFrame);
    uint startY = spacing.bottom + (frameY * framePixelsY) + (frameY * spacing.verticalFrame);

    //Scale UV to tile coords, then normalize into texture coords
    float x = ((texCoord.x * framePixelsX) / float(texSize.x));
    if (sprite.flipX)
        x = (framePixelsX / float(texSize.x)) - x;
    float y = ((texCoord.y * framePixelsY) / float(texSize.y));
    if (sprite.flipY)
        y = (framePixelsY / float(texSize.y)) - y;

    //Add start pixels, also scaled into normalized texture coords
    float u = x + (startX / float(texSize.x));
    float v = y + (startY / float(texSize.y));
    vec2 coords = vec2(u, v);

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
    {
        vec4 mask_val = texture2D(lightMask, coords);
        mask = mask_val.r;
    }

    fragColor = calculateLights(albedo, normal, worldPos, camera.position, mask);
}