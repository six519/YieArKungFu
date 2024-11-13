#ifndef _GAME_H_
#define _GAME_H_

#include <raylib.h>
#include "settings.hpp"
#include "stage.hpp"

//using namespace std;

class TitleStage;

class Game
{
    private:
        TitleStage *titleStage;
        void cleanUp();
    public:
        int state;
        Game();
        void run();
};

#endif