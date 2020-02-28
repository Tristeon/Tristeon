#version 150
in vec2 texCoord;
out vec4 FragColor;

//Sprite
uniform sampler2D mainTex;
uniform int renderMode;
uniform int width;
uniform int height;

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

    if (renderMode == 0)
        drawSprite();
    else if (renderMode == 1)
        drawAnimatedSprite();
}

void drawSprite()
{
    FragColor = texture2D(mainTex, texCoord);
}

void drawAnimatedSprite()
{
    

}