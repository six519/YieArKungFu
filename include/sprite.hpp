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
        bool paused = false;
        Sprite(string imagePath);
        Texture2D getTexture();
        void unload();
        void draw();
        bool play();
        void drawByIndex(int index);
        void setTileCount(int count);
        int getTileCount();
        void flipHorizontal();
        void overrideFrameSpeed(int speed);
        void resetCurrentFrame();
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
    "player_high_kick",
    "player_flying_kick",
    "life",
    "health_hud",
    "health_green",
    "health_red",
    "wang_normal",
    "tao_normal",
    "chen_normal",
    "lang_normal",
    "spinning_chain",
    "hit",
    "wang_dead",
    "tao_dead",
    "chen_dead",
    "lang_dead",
    "player_smile",
    "mu_normal",
    "mu_dead",
    "wang_kick",
    "tao_kick",
    "chen_kick",
    "lang_kick",
    "mu_kick",
    "wang_other",
    "tao_other",
    "chen_other",
    "lang_other",
    "mu_other",
    "wang_hit",
    "tao_hit",
    "chen_hit",
    "lang_hit",
    "mu_hit",
    "player_dead"
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