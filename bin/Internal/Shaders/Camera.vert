#version 140

uniform vec2 screenSize;
uniform vec2 screenCoordinates;

out vec2 texCoord;
 
void main()
{
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);
    texCoord.x = (x+1.0)*0.5;
    texCoord.y = (y+1.0)*0.5;

    x += 1;
    y += 1;

    x *= screenSize.x;
    y *= screenSize.y;

    x += screenCoordinates.x;
    y += screenCoordinates.y;

    gl_Position = vec4(x, y, 0, 1);
}