#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_NONE -1
#define PLAYER_IDLE 0
#define PLAYER_IDLE_2 1
#define PLAYER_LEFT 2
#define PLAYER_RIGHT 3
#define PLAYER_DOWN 4
#define PLAYER_STAND_PUNCH 5
#define PLAYER_SIT_PUNCH 6
#define PLAYER_STAND_KICK 7
#define PLAYER_SIT_KICK 8
#define PLAYER_SPEED 1
#define PLAYER_FRAME_SPEED 15
#define PLAYER_DEFAULT_X 40
#define PLAYER_DEFAULT_Y 159
#define PLAYER_DEFAULT_LIVES 2

#include "game.hpp"
#include "timer.hpp"

class Game;

class Player: public Timer
{
    private:
        int haltTime;
        void setSpritesCoordinates();
        int lastMovement;
    protected:
        Game *game;
        void onTimeTick();
    public:
        int x;
        int y;
        int lives;
        int currentMovement;
        bool inputDisabled;
        bool canAttack;
        Player(Game *gm);
        void clear();
        void play();
        void setMovement(int move);
        void handleKeys();
};

const vector<string> PlayerSprites = {
    "player_normal",
    "player_down",
    "player_stand_punch",
    "player_sit_punch",
    "player_stand_kick",
    "player_sit_kick"
};

#endif