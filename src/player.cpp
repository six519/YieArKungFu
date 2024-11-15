#include "player.hpp"

Player::Player(Game *gm)
{
    game = gm;
    clear();

    //override framespeed here
    game->sprites.at("player_normal").overrideFrameSpeed(PLAYER_FRAME_SPEED);
}

void Player::clear()
{
    currentMovement = PLAYER_IDLE;
    x = PLAYER_DEFAULT_X;
    y = PLAYER_DEFAULT_Y;
    lives = PLAYER_DEFAULT_LIVES;
    inputDisabled = false;
    haltTime = 0;
    canStandPunch = true;
}

void Player::setSpritesCoordinates()
{
    for (auto& name : PlayerSprites)
    {
        game->sprites.at(name).x = x;
        game->sprites.at(name).y = y;
    }
}

void Player::play()
{
    setSpritesCoordinates();

    switch (currentMovement)
    {
    case PLAYER_LEFT:
        game->sprites.at("player_normal").play();
        break;
    case PLAYER_RIGHT:
        game->sprites.at("player_normal").play();
        break;
    case PLAYER_DOWN:
        game->sprites.at("player_down").draw();
        break;
    case PLAYER_IDLE_2:
        game->sprites.at("player_normal").drawByIndex(1);
        break;
    case PLAYER_STAND_PUNCH:
        game->sprites.at("player_stand_punch").draw();
        break;
    default:
        //PLAYER_IDLE
        game->sprites.at("player_normal").drawByIndex(0);
        break;
    }
}

void Player::onTimeTick()
{
    if (inputDisabled)
    {
        haltTime += 1;

        if (haltTime == 3)
        {
            inputDisabled = false;
            currentMovement = PLAYER_IDLE;
            haltTime = 0;
        }
    }
}