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
    haltTimeJump = 0;
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
    case PLAYER_UP:
    case PLAYER_COMING_DOWN:
        (isFlyingKick)? game->sprites.at("player_flying_kick").draw() : game->sprites.at("player_down").draw();
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
    case PLAYER_HIGH_KICK:
        game->sprites.at("player_high_kick").draw();
        break;
    default:
        //PLAYER_IDLE
        game->sprites.at("player_normal").drawByIndex(0);
        break;
    }
}

void Player::onTimeTick()
{
    if (
        inputDisabled && currentMovement != PLAYER_UP && currentMovement != PLAYER_COMING_DOWN
    )
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

    if (inputDisabled && isFlyingKick)
    {
        haltTimeJump += 1;

        if (haltTimeJump == 2)
        {
            haltTimeJump = 0;
            isFlyingKick = false;
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
        }
        else if (IsKeyDown(KEY_RIGHT) && x < (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
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
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            setMovement(PLAYER_DOWN);
        }
        else if(IsKeyReleased(KEY_DOWN))
        {
            setMovement(PLAYER_IDLE);
        }
        
        if (IsKeyDown(KEY_UP))
        {
            jumpTowards = PLAYER_JUMP_TOWARDS_NONE;

            if (IsKeyDown(KEY_LEFT))
            {
                jumpTowards = PLAYER_JUMP_TOWARDS_LEFT;
            }

            if (IsKeyDown(KEY_RIGHT))
            {
                jumpTowards = PLAYER_JUMP_TOWARDS_RIGHT;
            }

            setMovement(PLAYER_UP);
            inputDisabled = true;
            accelerationSpeed = PLAYER_JUMP_ACCELERATION_FRAME_SPEED;
        }

        handleAttack(
            (IsKeyDown(KEY_A) && canAttack),
            ((IsKeyDown(KEY_DOWN))? PLAYER_SIT_PUNCH : PLAYER_STAND_PUNCH)
        );
        
        handleAttack(
            (IsKeyDown(KEY_S) && canAttack && (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))),
            PLAYER_HIGH_KICK
        );

        handleAttack(
            (IsKeyDown(KEY_S) && canAttack),
            ((IsKeyDown(KEY_DOWN))? PLAYER_SIT_KICK : PLAYER_STAND_KICK)
        );
    }

    if (game->state == STAGE_GAME && (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_S)))
    {
        canAttack = true;
    }

    if (
        game->state == STAGE_GAME 
        && IsKeyDown(KEY_S) 
        && !isFlyingKick
        && (currentMovement == PLAYER_UP || currentMovement == PLAYER_COMING_DOWN)
        && y <= (PLAYER_JUMP_HEIGHT + 16) // TODO: Not sure if it is the right Math
        && canFlyingKick
    )
    {
        isFlyingKick = true;
        PlaySound(game->sounds.at("attack"));
        haltTimeJump = 0;
        canFlyingKick = false;
    }
}

void Player::handleAttack(bool condition, int movement)
{
    if(condition)
    {
        PlaySound(game->sounds.at("attack"));
        inputDisabled = true;
        canAttack = false;
        setMovement(movement);
    }
}

void Player::handleJump()
{
    if (currentMovement == PLAYER_UP || currentMovement == PLAYER_COMING_DOWN)
    {
        jumpFramesCounter++;
        if (jumpFramesCounter >= (TARGET_FPS / accelerationSpeed))
        {
            jumpFramesCounter = 0;
            handleTowardsJump();

            if (currentMovement == PLAYER_UP)
            {
                if (y > PLAYER_JUMP_HEIGHT)
                {
                    accelerationSpeed -= 1;
                    y -= PLAYER_JUMP_SPEED;
                    return;
                }
                setMovement(PLAYER_COMING_DOWN);
                return;
            }

            if (y < PLAYER_DEFAULT_Y)
            {
                if (accelerationSpeed < PLAYER_JUMP_ACCELERATION_FRAME_SPEED)
                {
                    accelerationSpeed += 1;
                }
                y += PLAYER_JUMP_SPEED;
                return;
            }

            y = PLAYER_DEFAULT_Y;
            setMovement(PLAYER_IDLE);
            isFlyingKick = false;
            inputDisabled = false;
            canFlyingKick = true;
        }
    }
}

void Player::handleTowardsJump()
{
    if (jumpTowards == PLAYER_JUMP_TOWARDS_RIGHT)
    {

        if (x < (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            x += PLAYER_JUMP_SPEED;
            return;
        }

        x = (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2);
        jumpTowards = PLAYER_JUMP_TOWARDS_LEFT;
        return;
    }
    
    if(jumpTowards == PLAYER_JUMP_TOWARDS_LEFT)
    {

        if (x > STAGE_BOUNDARY)
        {
            x -= PLAYER_JUMP_SPEED;
            return;
        }

        x = STAGE_BOUNDARY;
        jumpTowards = PLAYER_JUMP_TOWARDS_RIGHT;
    }
}