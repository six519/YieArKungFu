#include "sprite.hpp"

Sprite::Sprite(string imagePath)
{
    texture = LoadTexture(imagePath.c_str());
    x = 0;
    y = 0;
    frameRect = (Rectangle){ 0.0, 0.0, (float)texture.width, (float)texture.height };
}

void Sprite::unload()
{
    UnloadTexture(texture);
}

void Sprite::draw()
{
    DrawTextureRec(texture, frameRect, (Vector2){ (float)x,(float)y }, WHITE);
}

void Sprite::setTileCount(int count)
{
    tileCount = count;
    frameRect = (Rectangle){ 0.0, 0.0, (float)texture.width/tileCount, (float)texture.height };
}

void Sprite::drawByIndex(int ind)
{
    frameRect.x = (float)ind*(float)texture.width/tileCount;
    draw();
}

Texture2D Sprite::getTexture()
{
    return texture;
}

void Sprite::flipHorizontal()
{
    frameRect.width = -frameRect.width;
}