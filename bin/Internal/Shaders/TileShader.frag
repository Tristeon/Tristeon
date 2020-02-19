#version 150
in vec2 texCoord;

uniform sampler2D tileSet;

out vec4 FragColor;
void main()
{   
    FragColor = texture2D(tileSet, texCoord);
}