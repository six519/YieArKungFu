#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>
#include <vector>
#include <raylib.h>

using namespace std;

class Sprite
{
    protected:
        Texture2D texture;
    public:
        int x;
        int y;
        Sprite(string imagePath);
        Texture2D getTexture();
        void unload();
        void draw();
};

const vector<string> SpriteImages = {
    "title",
    "konami_logo"
};

#endif