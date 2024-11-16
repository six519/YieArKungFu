#ifndef _SPRITE_H_
#define _SPRITE_H_

#define LETTER_WIDTH 8

#include <string>
#include <vector>
#include <raylib.h>
#include "settings.hpp"

using namespace std;

class Sprite
{
    protected:
        Texture2D texture;
        int tileCount;
        int currentFrame;
        int framesCounter;
        Rectangle frameRect;
        int frameSpeed;
    public:
        int x;
        int y;
        Sprite(string imagePath);
        Texture2D getTexture();
        void unload();
        void draw();
        void play();
        void drawByIndex(int index);
        void setTileCount(int count);
        void flipHorizontal();
        void overrideFrameSpeed(int speed);
};

const vector<string> SpriteImages = {
    "title",
    "konami_logo",
    "letters",
    "game_bg",
    "player_normal",
    "player_down",
    "player_stand_punch",
    "player_sit_punch",
    "player_stand_kick",
    "player_sit_kick",
    "player_high_kick"
};

const string CopyrightText = "* 1985 konami";
const string OtherText = "* 2024 silva";
const string ToStartText = "press enter to start";

const vector<char> SpriteLetters = {
    'a',
    'b',
    'c',
    'd',
    'e',
    'f',
    'g',
    'h',
    'i',
    'k',
    'l',
    'm',
    'n',
    'o',
    'p',
    'q',
    'r',
    's',
    't',
    'u',
    'v',
    'w',
    'y',
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '-',
    ' ',
    '*'
};

#endif