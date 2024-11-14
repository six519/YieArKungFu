#include "player.hpp"

Player::Player(Game *gm)
{
    game = gm;
    currentMovement = PLAYER_IDLE;
    x = PLAYER_DEFAULT_X;
    y = PLAYER_DEFAULT_Y;

    //override framespeed here
    game->sprites.at("player_normal").overrideFrameSpeed(PLAYER_FRAME_SPEED);
}

void Player::unload(){}

void Player::play()
{
    switch (currentMovement)
    {
    case PLAYER_LEFT:
        break;
    case PLAYER_RIGHT:
        break;
    default:
        //PLAYER_IDLE
        game->sprites.at("player_normal").x = x;
        game->sprites.at("player_normal").y = y;
        game->sprites.at("player_normal").drawByIndex(0);
        break;
    }
}