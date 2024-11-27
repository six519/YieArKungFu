#ifndef _STAGE_H_
#define _STAGE_H_

#define STAGE_START 0
#define STAGE_VIEW 1
#define STAGE_GAME 2
#define STAGE_BOUNDARY 15

#define VILLAIN_DEFAULT_X 147
#define VILLAIN_DEFAULT_Y 152
#define VILLAIN_MOVE_NONE -1
#define VILLAIN_MOVE_IDLE 0
#define VILLAIN_MOVE_LEFT 1
#define VILLAIN_MOVE_RIGHT 2
#define VILLAIN_MOVE_DEAD 3
#define VILLAIN_MOVE_KICK 4
#define VILLAIN_MOVE_OTHER 5
#define VILLAIN_MOVE_SPECIAL 6
#define VILLAIN_MOVE_PAUSE 7
#define VILLAIN_FRAME_SPEED 21
#define VILLAIN_SPRITE_FRAME_SPEED 3
#define SPINNING_CHAIN_SPEED 6

#define MOVE_STATE_FOLLOW_PLAYER 0
#define MOVE_STATE_FORWARD_WITH_ATTACK 1

#define END_STATE_START 0
#define END_STATE_PLAY_SOUND 1
#define END_STATE_SHOWTIME 2
#define END_STATE_SHOWTIME_HK1 3
#define END_STATE_SHOWTIME_LK1 4
#define END_STATE_SHOWTIME_LK2 5
#define END_STATE_SHOWTIME_HK2 6
#define END_STATE_SHOWTIME_P 7
#define END_STATE_SMILE 8
#define END_STATE_COUNT_LIFE 9
#define END_STATE_END 10
#define END_STATE_GAME_OVER 11

#define HIGH_TIME 2
#define LOW_TIME 1

#include "game.hpp"
#include "other.hpp"

using namespace std;

class Game;

class Stage: public Timer
{
    protected:
        virtual void handleKeys()=0;
        virtual void stageDraw()=0;
        virtual void init()=0;
        virtual void onBlinkingDone()=0;
        Game *game;
        bool initialized;
        RenderTexture2D renderTexture;
        map<string, int> framesCounter;
        map<string, int> currentFrame;
    public:
        Stage(Game *gm);
        void draw();
        void run();
        void drawText(string text, int x, int y, bool blink);
        virtual void cleanUp()=0;
        void unloadTexture();
};

class TitleStage: public Stage 
{
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        void onBlinkingDone();
        void onTimeTick();
        bool blinkEnter = false;
        int maxBlink = 4;
        int blinkCount = 0;
    public:
        bool canEnter = true;
        void cleanUp();
};

class ViewStage: public Stage 
{
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        void onBlinkingDone();
        void onTimeTick();
    public:
        void cleanUp();
};

class GameStage: public Stage 
{
    using Stage::Stage;
    protected:
        void handleKeys();
        void stageDraw();
        void init();
        void onBlinkingDone();
        void onTimeTick();
    public:
        void cleanUp();
        void reset();
        void run();
        void handleEndState();
        int villainHealth;
        int villainX;
        int villainY;
        int villainCurrentMove = VILLAIN_MOVE_NONE;
        int villainMovementCounter;
        void setVillainSpritesCoordinates();
        void showVillain();
        void handleVillainMovement();
        void villainMovementTick();
        void villainFollowPlayer();
        void villainSimpleAttack();
        bool isVillainNearPlayer();
        void flipVillainSprites();
        void setEndStateWithPlayerMovement(int pMove, bool flip, bool playSound);
        bool isVillainFlipped = false;
        bool pauseMovement = false;
        int spinningChainX;
        int spinningChainY;
        int haltTime;
        int haltTimeHit;
        int maxHaltTime;
        int endState = END_STATE_START;
        int villainMoveState = MOVE_STATE_FOLLOW_PLAYER;
        int villainRandomAttack = -1;
        bool isCollidedWithPlayer();
        void checkCollisionWithPlayer(int *vX, int *vY, int *vBoxWidth, int *vBoxHeight, const CollisionInfo collisionInfo[]);
        bool showVillainHit;
        void resetVillainMove();
        void handleCollisionWithPlayer();
};

const string Villains[] = {
    "wang",
    "tao",
    "chen",
    "lang",
    "mu"
};

const string VillainSprites[] = {
    "kick",
    "other",
    "normal",
    "dead",
    "hit"
};

const CollisionInfo collisionsInfo[] = {
    {5, 6, 8, 16, 32, 11},
    {7, 4, 8, 5, 32, 12}, //{7, 4, 8, 16, 32, 12}, (TODO: NOT SURE IF THIS IS THE FIX FOR TAO)
    {9, 7, 8, 16, 32, 8},
    {7, 4, 9, 9, 31, 7},
    {12, 2, 8, 17, 32, 6}
};

const CollisionInfo collisionsKickInfo[] = {
    {0, 44, 20, 6, 4, 0},
    {0, 33, 20, 6, 4, 0},
    {0, 35, 13, 5, 2, 0},
    {0, 25, 8, 4, 6, 0},
    {0, 33, 26, 6, 4, 0}
};

const CollisionInfo collisionsOtherInfo[] = {
    {0, 47, 26, 3, 2, 0},
    {7, 29, 15, 3, 2, 0},
    {0, 37, 19, 3, 3, 0},
    {0, 23, 36, 6, 3, 0},
    {7, 29, 15, 3, 3, 0}
};

const int attackList[] = {
    VILLAIN_MOVE_KICK,
    VILLAIN_MOVE_OTHER
};

#endif