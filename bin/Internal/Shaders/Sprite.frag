#version 140
#include Lighting.incl
#include Includes/Actor.incl
#include Includes/Sprite.incl
#include Includes/SpriteTextures.incl
#include Includes/Camera.incl

in vec2 texCoord;
in vec2 worldPos;

out vec4 fragColor;

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

    fragColor = calculateLights(albedo, normal, worldPos, camera.position, mask, camera.displayPixels, sprite.renderMask);
}