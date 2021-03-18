#version 140
#include Lighting.incl
#include Includes/Sprite.incl

uniform sampler2D texture;
uniform float intensity;

in vec2 texCoord;
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

    vec4 tex = texture2D(texture, coords);
    vec3 albedo = tex.rgb * sprite.colour.rgb;

    fragColor = vec4(albedo * tex.a * intensity, intensity);
}