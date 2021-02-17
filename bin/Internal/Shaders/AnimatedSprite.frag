#version 140
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

//Sprite
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform bool normalMapEnabled;
struct Sprite
{
    uint width;
    uint height;

    vec4 colour;

    bool flipX;
    bool flipY;
};
uniform Sprite sprite;

//Animation
struct Animation
{
    uint cols;
    uint rows;
    uint frame;
};
uniform Animation animation;

//Lighting
struct Light
{
    vec3 position;
    float intensity;
    vec3 color;
};
uniform Light lights[32];
uniform int lightCount;
uniform int disableLighting;

//Camera
struct CameraData
{
    vec2 position;
    uvec2 displayPixels;
    float zoom;
};
uniform CameraData camera;

vec4 calculateLights(vec4 albedo, vec3 normal, vec3 world_pos);

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
    }
    else
    {
        normal = vec3(0, 0, -1);
    }

    fragColor = calculateLights(albedo, normal, vec3(worldPos, 0));
}

vec4 calculateLights(vec4 albedo, vec3 normal, vec3 world_pos)
{
    if (disableLighting == 1)
        return albedo;

    vec3 light;
    for (int i = 0; i < lightCount && i < 32; i++)
    {
        vec3 lightDir = normalize(lights[i].position - world_pos);
        vec3 viewDir = normalize(vec3(camera.position, -256) - world_pos);
        vec3 halfway = normalize(lightDir + viewDir);
        vec3 reflectDir = reflect(-lightDir, normal);

        //Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = vec3(diff, diff, diff);

        //Attenuation
        float d = length(lights[i].position - world_pos);
        float attenuation = 1.0 / (1.0 + 0.014 * d + 0.000007 * (d * d)); 

        light += diffuse * attenuation * lights[i].intensity * lights[i].color;  
    }
    
    return albedo * 0.1f + albedo * vec4(light, 1);
}