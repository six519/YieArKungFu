#ifndef _STAGE_H_
#define _STAGE_H_

#define STAGE_START 0
#define STAGE_VIEW 1
#define STAGE_GAME 2
#define STAGE_BOUNDARY 15

#define VILLAIN_DEFAULT_X 147
#define VILLAIN_DEFAULT_Y 152
#define VILLAIN_MOVE_NONE -1
#define VILLAIN_MOVE_IDLE 0
#define VILLAIN_MOVE_LEFT 1
#define VILLAIN_MOVE_RIGHT 2
#define SPINNING_CHAIN_SPEED 6

#include "game.hpp"
#include "timer.hpp"

using namespace std;

class Game;

class Stage: public Timer
{
    protected:
        virtual void handleKeys()=0;
        virtual void stageDraw()=0;
        virtual void init()=0;
        virtual void onBlinkingDone()=0;
        Game *game;
        bool initialized;
        RenderTexture2D renderTexture;
        map<string, int> framesCounter;
        map<string, int> currentFrame;
    public:
        Stage(Game *gm);
        void draw();
        void run();
        void drawText(string text, int x, int y, bool blink);
        virtual void cleanUp()=0;
        void unloadTexture();
};

class TitleStage: public Stage 
{
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        void onBlinkingDone();
        void onTimeTick();
        bool blinkEnter = false;
        int maxBlink = 4;
        int blinkCount = 0;
    public:
        void cleanUp();
};

class ViewStage: public Stage 
{
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        void onBlinkingDone();
        void onTimeTick();
    public:
        void cleanUp();
};

class GameStage: public Stage 
{
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        void onBlinkingDone();
        void onTimeTick();
    public:
        void cleanUp();
        void reset();
        void run();
        int villainHealth;
        int villainX;
        int villainY;
        int villainCurrentMove = VILLAIN_MOVE_NONE;
        void setVillainSpritesCoordinates();
        void showVillain();
        void flipVillainSprites();
        bool isVillainFlipped = false;
        bool pauseMovement = false;
        int spinningChainX;
        int spinningChainY;
        int haltTime;
};

const string Villains[] = {
    "wang",
    "tao",
    "chen",
    "lang"
};

const string VillainSprites[] = {
    "normal"
};

#endif