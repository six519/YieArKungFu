#ifndef _STAGE_H_
#define _STAGE_H_

#include "game.hpp"

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
    public:
        Stage(Game *gm);
        void draw();
        void run();
        virtual void cleanUp()=0;
};

class TitleStage: public Stage {
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
    public:
        void cleanUp();
};

#endif