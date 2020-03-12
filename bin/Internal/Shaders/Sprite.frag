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
    int renderMode;
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

void drawSprite();
void drawAnimatedSprite();

void main()
{
    if (texCoord.x > 1 || texCoord.y > 1 || texCoord.x < 0 || texCoord.y < 0)
        discard;

    if (sprite.renderMode == 0)
        drawSprite();
    else if (sprite.renderMode == 1)
        drawAnimatedSprite();
}

void drawSprite()
{
    ivec2 texSize = textureSize(mainTex, 0);

    //Determine the amount of pixels per tile
    int pixelsX = texSize.x - (spacing.left + spacing.right);
    int pixelsY = texSize.y - (spacing.top + spacing.bottom);

    //Scale UV to frame coords then normalize into texture coords, then add left/bottom spacing
    float x = ((texCoord.x * pixelsX) / texSize.x) + (spacing.left / (float)texSize.x);
    float y = ((texCoord.y * pixelsY) / texSize.y) + (spacing.bottom / (float)texSize.y);

    FragColor = texture2D(mainTex, vec2(x, y));
}

void drawAnimatedSprite()
{
    ivec2 texSize = textureSize(mainTex, 0);

    int frameX = animation.frame % animation.cols;
    int frameY = animation.rows - 1 - (animation.frame / animation.rows);

    //Determine the amount of pixels per tile
    int framePixelsX = (texSize.x - ((spacing.left + spacing.right) + animation.cols - 1)) / animation.cols;
    int framePixelsY = (texSize.y - ((spacing.top + spacing.bottom) + animation.rows - 1)) / animation.rows;

    //Determine the start point of the tile depending on spacing
    int startX = spacing.left + (frameX * framePixelsX) + (frameX * spacing.horizontalFrame);
    int startY = spacing.bottom + (frameY * framePixelsY) + (frameY * spacing.verticalFrame);

    //Scale UV to tile coords, then normalize into texture coords
    float x = ((texCoord.x * framePixelsX) / (float)texSize.x);
    float y = ((texCoord.y * framePixelsY) / (float)texSize.y);

    //Add start pixels, also scaled into normalized texture coords
    float u = x + (startX / (float)texSize.x);
    float v = y + (startY / (float)texSize.y);
    
    FragColor = texture2D(mainTex, vec2(u, v));
}