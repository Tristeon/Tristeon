#version 330
in vec2 texCoord;
out vec4 FragColor;

struct Spacing
{
    int left;
    int right;
    int top;
    int bottom;

    int horizontalFrame;
    int verticalFrame;
};
uniform Spacing spacing;

//Sprite
uniform sampler2D mainTex;
struct Sprite
{
    int width;
    int height;

    vec4 colour;

    bool flipX;
    bool flipY;
};
uniform Sprite sprite;

//Animation
struct Animation
{
    int cols;
    int rows;
    int frame;
};
uniform Animation animation;

void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    ivec2 texSize = textureSize(mainTex, 0);

    int frameX = animation.frame % animation.cols;
    int frameY = animation.rows - 1 - (animation.frame / animation.cols);

    //Determine the amount of pixels per tile
    int framePixelsX = (texSize.x - ((spacing.left + spacing.right) + ((animation.cols - 1) * spacing.horizontalFrame))) / animation.cols;
    int framePixelsY = (texSize.y - ((spacing.top + spacing.bottom) + ((animation.rows - 1) * spacing.verticalFrame))) / animation.rows;

    //Determine the start point of the tile depending on spacing
    int startX = spacing.left + (frameX * framePixelsX) + (frameX * spacing.horizontalFrame);
    int startY = spacing.bottom + (frameY * framePixelsY) + (frameY * spacing.verticalFrame);

    //Scale UV to tile coords, then normalize into texture coords
    float x = ((texCoord.x * framePixelsX) / (float)texSize.x);
    if (sprite.flipX)
        x = (framePixelsX / (float)texSize.x) - x;
    float y = ((texCoord.y * framePixelsY) / (float)texSize.y);
    if (sprite.flipY)
        y = (framePixelsY / (float)texSize.y) - y;

    //Add start pixels, also scaled into normalized texture coords
    float u = x + (startX / (float)texSize.x);
    float v = y + (startY / (float)texSize.y);
    
    FragColor = texture2D(mainTex, vec2(u, v)) * sprite.colour;
}