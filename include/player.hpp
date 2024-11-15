#ifndef _PLAYER_H_
#define _PLAYER_H_

#define PLAYER_IDLE 0
#define PLAYER_IDLE_2 1
#define PLAYER_LEFT 2
#define PLAYER_RIGHT 3
#define PLAYER_SPEED 1
#define PLAYER_FRAME_SPEED 15
#define PLAYER_DEFAULT_X 40
#define PLAYER_DEFAULT_Y 160
#define PLAYER_DEFAULT_LIVES 2

#include "game.hpp"

class Game;

class Player
{
    protected:
        Game *game;
    public:
        int x;
        int y;
        int lives;
        int currentMovement;
        Player(Game *gm);
        void clear();
        void play();
};

#endif