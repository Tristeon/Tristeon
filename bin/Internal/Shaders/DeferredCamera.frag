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
    float range;
    int type;
};
uniform Light lights[32];
uniform int lightCount = 0;

in vec2 texCoord;
out vec4 fragColor;

void main()
{
    vec4 albedo_val = texture2D(albedo, texCoord);
 
    if (length(albedo_val.rgb * albedo_val.a) < 0.2f)
        discard;

    vec4 normal_val = texture2D(normals, texCoord);
    vec4 position_val = texture2D(positions, texCoord);

    vec3 frag_color = albedo_val.rgb;
    vec3 world_pos = position_val.rgb;
    vec3 normal = vec3(0, 0, -1);

    vec3 lightDir = normalize(lights[0].position - world_pos);
    vec3 viewDir = normalize(vec3(camera.position, -256) - world_pos);
    vec3 halfway = normalize(lightDir + viewDir);
    vec3 reflectDir = reflect(-lightDir, normal);

    //Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(diff, diff, diff);

    //Specular
    //Attenuation
    float d = length(lights[0].position - world_pos);
    float attenuation = 1.0 / (1.0 + 0.014 * d + 0.000007 * (d * d));   
    
    vec3 result = frag_color * 0.1f + (diffuse * attenuation);
    fragColor = vec4(result, 1.0);
}