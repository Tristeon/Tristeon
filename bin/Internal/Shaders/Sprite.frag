#version 140

in vec2 texCoord;
in vec2 worldPos;

out vec4 fragColor;

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

//Camera
struct CameraData
{
    vec2 position;
    uvec2 displayPixels;
    float zoom;
};
uniform CameraData camera;

struct Light
{
    vec3 position;
    float intensity;
    vec3 color;
};
uniform Light lights[32];
uniform int lightCount;
uniform int disableLighting;

vec4 calculateLights(vec4 albedo, vec3 normal, vec3 world_pos);

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