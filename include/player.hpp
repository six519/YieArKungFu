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
#define PLAYER_SMILE 12
#define PLAYER_DEAD 13
#define PLAYER_VERY_DEAD 14
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
#define PLAYER_SHAKE_FORCE 2
#define PLAYER_CAN_ATTACK_TIME 5

#include "game.hpp"
#include "other.hpp"

class Game;

class Player: public Timer
{
    private:
        int haltTime;
        int haltTimeJump;
        int activateTime = 0;
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
        bool isFlipped = false;
        int x;
        int y;
        int lives;
        int health;
        int currentMovement;
        bool inputDisabled;
        bool canAttack;
        bool activateAttack;
        int oldX = 0;
        bool shake = false;
        bool addX = true;
        int kuyakoy = 0;
        Player(Game *gm);
        void clear();
        void play();
        void flipSprites();
        void setMovement(int move);
        void handleKeys();
        void handleAttack(bool condition, int movement);
        void handleJump();
        void handleTowardsJump();
        void isCollidedWithVillain();
        void checkCollisionWithVillain(int *playerX, int *playerY, int *playerBoxWidth, int *playerBoxHeight, CollisionInfo collisionInfo);
        bool showHit = false;
};

const vector<string> PlayerSprites = {
    "player_normal",
    "player_down",
    "player_stand_punch",
    "player_sit_punch",
    "player_stand_kick",
    "player_sit_kick",
    "player_high_kick",
    "player_flying_kick",
    "player_smile",
    "player_dead"
};

const CollisionInfo collisionInfoSitPunch = {
    28, 0, 19, 3, 3, 0
};

const CollisionInfo collisionInfoStandKick = {
    25, 0, 24, 6, 5, 0
};

const CollisionInfo collisionInfoSitKick = {
    30, 0, 27, 6, 5, 0
};

const CollisionInfo collisionInfoHighKick = {
    27, 0, 3, 5, 4, 0
};

const CollisionInfo collisionInfoAir = {
    31, 0, 24, 4, 5, 0
};

const CollisionInfo collisionInfoStandPunch = {
    25, 0, 14, 3, 3, 0
};

const CollisionInfo playerCollisionInfo = {
    8, 10, 1, 10, 32, 0
};

#endif