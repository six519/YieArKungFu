#include "sprite.hpp"

Sprite::Sprite(string imagePath)
{
    texture = LoadTexture(imagePath.c_str());
    x = 0;
    y = 0;
    decreaseWidth = 0;
}

void Sprite::unload()
{
    UnloadTexture(texture);
}

void Sprite::draw()
{
    DrawTexture(texture, x, y, WHITE);
}

void Sprite::setTileCount(int count, int decreaseWidth)
{
    tileCount = count;
    this->decreaseWidth = decreaseWidth;
    frameRect = (Rectangle){ 0.0, 0.0, (float)(texture.width - decreaseWidth)/tileCount, (float)texture.height };
}

void Sprite::drawByIndex(int ind)
{
    frameRect.x = (float)ind*(float)(texture.width - decreaseWidth)/tileCount;
    DrawTextureRec(texture, frameRect, (Vector2){ (float)x,(float)y }, WHITE);
}

Texture2D Sprite::getTexture()
{
    return texture;
}