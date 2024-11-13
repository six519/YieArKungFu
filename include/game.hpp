#ifndef _GAME_H_
#define _GAME_H_

#include <map>
#include <raylib.h>
#include "settings.hpp"
#include "stage.hpp"
#include "sprite.hpp"

using namespace std;

class TitleStage;

class Game
{
    private:
        TitleStage *titleStage;
        void cleanUp();
        void loadSprite(string name);
    public:
        int state;
        map<string, Sprite> sprites;
        Game();
        void run();
};

#endif