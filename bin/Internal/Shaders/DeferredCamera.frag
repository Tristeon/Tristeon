#version 140

uniform sampler2D albedo;
uniform sampler2D normals;
uniform sampler2D positions;

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

in vec2 texCoord;
out vec4 fragColor;

void main()
{
    vec4 albedo_val = texture2D(albedo, texCoord);
    if (disableLighting == 1)
    {
        fragColor = albedo_val;
        return;
    }

    vec4 normal_val = texture2D(normals, texCoord);
    vec4 position_val = texture2D(positions, texCoord);

    vec3 frag_color = albedo_val.rgb;
    vec3 world_pos = position_val.rgb;
    vec3 normal = normal_val.rgb;

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
    
    vec3 result = frag_color * 0.1f + frag_color * light;
    fragColor = vec4(result, 1.0);
}