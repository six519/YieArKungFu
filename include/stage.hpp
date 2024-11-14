#ifndef _STAGE_H_
#define _STAGE_H_

#include "game.hpp"

using namespace std;

class Game;

class Stage
{
    protected:
        virtual void handleKeys()=0;
        virtual void stageDraw()=0;
        virtual void init()=0;
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
};

class TitleStage: public Stage {
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        bool blinkEnter = false;
    public:
        void cleanUp();
};

#endif