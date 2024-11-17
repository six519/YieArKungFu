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
#define PLAYER_HIGH_KICK 9
#define PLAYER_UP 10
#define PLAYER_COMING_DOWN 11
#define PLAYER_SPEED 1
#define PLAYER_FRAME_SPEED 15
#define PLAYER_DEFAULT_X 40
#define PLAYER_DEFAULT_Y 159
#define PLAYER_DEFAULT_LIVES 2
#define PLAYER_JUMP_HEIGHT 115
#define PLAYER_JUMP_SPEED 2
#define PLAYER_JUMP_ACCELERATION_FRAME_SPEED 55
#define PLAYER_JUMP_TOWARDS_NONE 0
#define PLAYER_JUMP_TOWARDS_LEFT 1
#define PLAYER_JUMP_TOWARDS_RIGHT 2

#include "game.hpp"
#include "timer.hpp"

class Game;

class Player: public Timer
{
    private:
        int haltTime;
        int haltTimeJump;
        void setSpritesCoordinates();
        int lastMovement;
        int jumpFramesCounter = 0;
        int accelerationSpeed = 0;
        int jumpTowards;
        bool isFlyingKick = false;
        bool canFlyingKick = true;
    protected:
        Game *game;
        void onTimeTick();
    public:
        int x;
        int y;
        int lives;
        int health;
        int currentMovement;
        bool inputDisabled;
        bool canAttack;
        Player(Game *gm);
        void clear();
        void play();
        void setMovement(int move);
        void handleKeys();
        void handleAttack(bool condition, int movement);
        void handleJump();
        void handleTowardsJump();
};

const vector<string> PlayerSprites = {
    "player_normal",
    "player_down",
    "player_stand_punch",
    "player_sit_punch",
    "player_stand_kick",
    "player_sit_kick",
    "player_high_kick",
    "player_flying_kick"
};

#endif