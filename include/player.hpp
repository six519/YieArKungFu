#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_IDLE 0
#define PLAYER_IDLE_2 1
#define PLAYER_LEFT 2
#define PLAYER_RIGHT 3
#define PLAYER_DOWN 4
#define PLAYER_STAND_PUNCH 5
#define PLAYER_SPEED 1
#define PLAYER_FRAME_SPEED 15
#define PLAYER_DEFAULT_X 40
#define PLAYER_DEFAULT_Y 160
#define PLAYER_DEFAULT_LIVES 2

#include "game.hpp"
#include "timer.hpp"

class Game;

class Player: public Timer
{
    private:
        int haltTime;
        void setSpritesCoordinates();
    protected:
        Game *game;
        void onTimeTick();
    public:
        int x;
        int y;
        int lives;
        int currentMovement;
        bool inputDisabled;
        bool canStandPunch;
        Player(Game *gm);
        void clear();
        void play();
};

const vector<string> PlayerSprites = {
    "player_normal",
    "player_down",
    "player_stand_punch"
};

#endif