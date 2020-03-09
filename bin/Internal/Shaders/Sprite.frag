#version 330
in vec2 texCoord;
out vec4 FragColor;

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
    FragColor = texture2D(mainTex, texCoord);
}

void drawAnimatedSprite()
{
    float x = texCoord.x / animation.cols;
    float y = texCoord.y / animation.rows;

    float u = x - floor(x);
    float v = y - floor(y);

    float frameX = animation.frame % animation.cols;
    float frameY = floor(float(animation.frame) / float(animation.rows));

    u += frameX / animation.cols;
    v += frameY / animation.rows;

    FragColor = texture2D(mainTex, vec2(u, v));
}