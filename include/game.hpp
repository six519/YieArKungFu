#ifndef _GAME_H_
#define _GAME_H_

#include <map>
#include <raylib.h>
#include "settings.hpp"
#include "stage.hpp"
#include "sprite.hpp"

using namespace std;

class TitleStage;
class ViewStage;

class Game
{
    private:
        TitleStage *titleStage;
        ViewStage *viewStage;
        void cleanUp();
        void loadSprite(string name);
        void loadMusic(string name);
    public:
        int state;
        int stage;
        map<string, Sprite> sprites;
        map<string, Music> musics;
        Game();
        void run();
};

const vector<string> MusicsList = {
    "bg"
};

#endif