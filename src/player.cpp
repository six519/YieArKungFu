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
    setMovement(PLAYER_IDLE);
    x = PLAYER_DEFAULT_X;
    y = PLAYER_DEFAULT_Y;
    lives = PLAYER_DEFAULT_LIVES;
    inputDisabled = false;
    haltTime = 0;
    canAttack = true;
    lastMovement = PLAYER_NONE;
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
    case PLAYER_SIT_PUNCH:
        game->sprites.at("player_sit_punch").draw();
        break;
    case PLAYER_STAND_KICK:
        game->sprites.at("player_stand_kick").draw();
        break;
    case PLAYER_SIT_KICK:
        game->sprites.at("player_sit_kick").draw();
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
            haltTime = 0;

            if (lastMovement == PLAYER_DOWN && IsKeyDown(KEY_DOWN))
            {
                setMovement(PLAYER_DOWN);
                return;
            }

            setMovement(PLAYER_IDLE);
        }
    }
}

void Player::setMovement(int move)
{
    lastMovement = currentMovement;
    currentMovement = move;
}

void Player::handleKeys()
{
    // making sure that it will be executed only on a specific state
    if (game->state == STAGE_GAME && !inputDisabled)
    {
        if (IsKeyDown(KEY_LEFT) && x > STAGE_BOUNDARY)
        {
            setMovement(PLAYER_LEFT);
            x -= PLAYER_SPEED;
        }
        else if (IsKeyDown(KEY_LEFT) && x <= STAGE_BOUNDARY)
        {
            setMovement(PLAYER_IDLE);
        }
        else if(IsKeyReleased(KEY_LEFT))
        {
            setMovement(PLAYER_IDLE);
        } else if (IsKeyDown(KEY_RIGHT) && x < (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            setMovement(PLAYER_RIGHT);
            x += PLAYER_SPEED;
        }
        else if(IsKeyDown(KEY_RIGHT) && x >= (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            setMovement(PLAYER_IDLE_2);
        }
        else if(IsKeyReleased(KEY_RIGHT))
        {
            setMovement(PLAYER_IDLE);
        }else if (IsKeyDown(KEY_DOWN))
        {
            setMovement(PLAYER_DOWN);
        }
        else if(IsKeyReleased(KEY_DOWN))
        {
            setMovement(PLAYER_IDLE);
        }
        
        if(IsKeyDown(KEY_A) && canAttack)
        {
            PlaySound(game->sounds.at("attack"));
            inputDisabled = true;
            canAttack = false;
            setMovement((IsKeyDown(KEY_DOWN))? PLAYER_SIT_PUNCH : PLAYER_STAND_PUNCH);
        }

        if(IsKeyDown(KEY_S) && canAttack)
        {
            PlaySound(game->sounds.at("attack"));
            inputDisabled = true;
            canAttack = false;
            setMovement((IsKeyDown(KEY_DOWN))? PLAYER_SIT_KICK : PLAYER_STAND_KICK);
        }
    }

    if (game->state == STAGE_GAME && (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_S)))
    {
        canAttack = true;
    }
}