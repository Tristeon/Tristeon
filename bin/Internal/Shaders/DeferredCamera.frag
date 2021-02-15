#version 140

uniform sampler2D albedo;
uniform sampler2D normals;
uniform sampler2D positions;

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
    vec4 normal_val = texture2D(normals, texCoord);
    vec4 position_val = texture2D(positions, texCoord);

    vec3 frag_color = albedo_val.rgb;
    vec3 world_pos = position_val.rgb;
    vec3 normal = normal_val.rgb;

    vec3 result = frag_color * 0.1f;
    vec3 diffuse = vec3(distance(world_pos, lights[0].position) / 1024.0f);
    result += diffuse;
    fragColor = vec4(result, 1.0);
}