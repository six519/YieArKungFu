#ifndef _GAME_H_
#define _GAME_H_

#define DEFAULT_HEALTH 9
#define LOW_HEALTH = 4

#include <map>
#include <raylib.h>
#include "settings.hpp"
#include "stage.hpp"
#include "sprite.hpp"
#include "player.hpp"

using namespace std;

class TitleStage;
class ViewStage;
class GameStage;
class Player;

class Game
{
    private:
        void cleanUp();
        void loadSprite(string name);
        void loadMusic(string name);
        void loadSound(string name);
    public:
        int state;
        int stage;
        int score;
        TitleStage *titleStage;
        ViewStage *viewStage;
        GameStage *gameStage;
        Player *player;
        map<string, Sprite> sprites;
        map<string, Music> musics;
        map<string, Sound> sounds;
        Game();
        void run();
};

const vector<string> MusicsList = {
    "bg"
};

const vector<string> SoundsList = {
    "attack"
};

#endif