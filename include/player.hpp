#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_IDLE 0
#define PLAYER_LEFT 1
#define PLAYER_RIGHT 2
#define PLAYER_SPEED 1
#define PLAYER_FRAME_SPEED 15
#define PLAYER_DEFAULT_X 40
#define PLAYER_DEFAULT_Y 160

#include "game.hpp"

class Game;

class Player
{
    protected:
        Game *game;
    public:
        int x;
        int y;
        int currentMovement;
        Player(Game *gm);
        void unload();
        void play();
};

#endif