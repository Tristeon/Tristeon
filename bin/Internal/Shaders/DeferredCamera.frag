#version 140

uniform sampler2D albedo;
uniform sampler2D normals;
uniform sampler2D positions;

in vec2 texCoord;
out vec4 fragColor;

void main()
{
    fragColor = texture(albedo, texCoord);
}