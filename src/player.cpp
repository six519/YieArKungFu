#include "player.hpp"

Player::Player(Game *gm)
{
    game = gm;
    lives = PLAYER_DEFAULT_LIVES;
    clear();

    //override framespeed here
    game->sprites.at("player_normal").overrideFrameSpeed(PLAYER_FRAME_SPEED);
}

void Player::clear()
{
    setMovement(PLAYER_IDLE);
    x = PLAYER_DEFAULT_X;
    y = PLAYER_DEFAULT_Y;
    inputDisabled = false;
    haltTime = 0;
    haltTimeJump = 0;
    canAttack = true;
    activateAttack = false;
    lastMovement = PLAYER_NONE;
    health = DEFAULT_HEALTH;

    if (isFlipped)
    {
        flipSprites();
    }
}

void Player::setSpritesCoordinates()
{
    for (auto& name : PlayerSprites)
    {
        game->sprites.at(name).x = x;
        game->sprites.at(name).y = y;
    }
}

void Player::flipSprites()
{
    for (auto& name : PlayerSprites)
    {
        game->sprites.at(name).flipHorizontal();
    }

    game->sprites.at("hit").flipHorizontal();

    isFlipped = !isFlipped;
}

void Player::play()
{
    if (shake)
    {
        x = (addX)? x + PLAYER_SHAKE_FORCE : x - PLAYER_SHAKE_FORCE;
        addX = !addX;
    }

    setSpritesCoordinates();

    switch (currentMovement)
    {
    case PLAYER_LEFT:
    case PLAYER_RIGHT:
        game->sprites.at("player_normal").paused = game->gameStage->showVillainHit;
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
    case PLAYER_SMILE:
        game->sprites.at("player_smile").draw();
        break;
    case PLAYER_VERY_DEAD:
        game->sprites.at("player_dead").drawByIndex(0);
        break;
    case PLAYER_DEAD:
        if (game->sprites.at("player_dead").play())
        {
            PlaySound(game->sounds.at("feet_sound"));
            kuyakoy += 1;

            if (kuyakoy == 3)
            {
                setMovement(PLAYER_VERY_DEAD);
                game->gameStage->villainEndState = END_STATE_VILLAIN_END;
            }
        }
        break;
    default:
        //PLAYER_IDLE
        game->sprites.at("player_normal").drawByIndex(0);
        break;
    }

    if (showHit)
    {
        game->sprites.at("hit").draw();
    }

    //flip checker
    if (game->gameStage->endState <= END_STATE_START)
    {
        if ((game->gameStage->villainX) < x && !isFlipped && !isFlyingKick)
        {
            flipSprites();
        }
        if ((x) < game->gameStage->villainX  && isFlipped && !isFlyingKick)
        {
            flipSprites();
        }
    }
}

void Player::onTimeTick()
{
    if (
        inputDisabled && currentMovement != PLAYER_UP && currentMovement != PLAYER_COMING_DOWN && !game->gameStage->showVillainHit
    )
    {
        haltTime += 1;

        if (haltTime == 3)
        {
            inputDisabled = false;
            haltTime = 0;
            showHit = false;
            activateAttack = true;
            activateTime = 0;

            if (lastMovement == PLAYER_DOWN && (IsKeyDown(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))))
            {
                setMovement(PLAYER_DOWN);
                return;
            }

            setMovement(PLAYER_IDLE);
        }
    }

    if (inputDisabled && isFlyingKick && !game->gameStage->showVillainHit)
    {
        haltTimeJump += 1;

        if (haltTimeJump == 2)
        {
            haltTimeJump = 0;
            isFlyingKick = false;
        }
    }


    if (activateAttack)
    {
        activateTime += 1;
        if (activateTime == PLAYER_CAN_ATTACK_TIME)
        {
            activateTime = 0;
            canAttack = true;
            activateAttack = false;
        }
    }

}

void Player::setMovement(int move)
{
    if (currentMovement == PLAYER_DEAD && move == PLAYER_IDLE)
        return;
    lastMovement = currentMovement;
    currentMovement = move;
}

void Player::handleKeys()
{
    // making sure that it will be executed only on a specific state
    if (game->state == STAGE_GAME && !inputDisabled && !game->gameStage->pauseMovement && !game->gameStage->showVillainHit)
    {
        if ((IsKeyDown(KEY_LEFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))) && x > STAGE_BOUNDARY)
        {
            setMovement(PLAYER_LEFT);
            x -= PLAYER_SPEED;
        }
        else if ((IsKeyDown(KEY_LEFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))) && x <= STAGE_BOUNDARY)
        {
            setMovement(PLAYER_IDLE);
        }
        else if((IsKeyReleased(KEY_LEFT) || (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) && IsGamepadAvailable(0))))
        {
            setMovement(PLAYER_IDLE);
        }
        else if ((IsKeyDown(KEY_RIGHT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))) && x < (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            setMovement(PLAYER_RIGHT);
            x += PLAYER_SPEED;
        }
        else if((IsKeyDown(KEY_RIGHT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))) && x >= (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            setMovement(PLAYER_IDLE_2);
        }
        else if((IsKeyReleased(KEY_RIGHT) || (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) && IsGamepadAvailable(0))))
        {
            setMovement(PLAYER_IDLE);
        }
        else if ((IsKeyDown(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))))
        {
            setMovement(PLAYER_DOWN);
        }
        else if((IsKeyReleased(KEY_DOWN) || (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) && IsGamepadAvailable(0))))
        {
            setMovement(PLAYER_IDLE);
        }
        
        if ((IsKeyDown(KEY_UP) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_UP))))
        {
            jumpTowards = PLAYER_JUMP_TOWARDS_NONE;

            if ((IsKeyDown(KEY_LEFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))))
            {
                jumpTowards = PLAYER_JUMP_TOWARDS_LEFT;
            }

            if ((IsKeyDown(KEY_RIGHT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))))
            {
                jumpTowards = PLAYER_JUMP_TOWARDS_RIGHT;
            }

            setMovement(PLAYER_UP);
            inputDisabled = true;
            accelerationSpeed = PLAYER_JUMP_ACCELERATION_FRAME_SPEED;
        }

        handleAttack(
            ((IsKeyDown(KEY_A) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))) && canAttack),
            (((IsKeyDown(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))))? PLAYER_SIT_PUNCH : PLAYER_STAND_PUNCH)
        );
        
        handleAttack(
            ((IsKeyDown(KEY_S) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) && canAttack && ((IsKeyDown(KEY_LEFT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT))) || (IsKeyDown(KEY_RIGHT) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT))))),
            PLAYER_HIGH_KICK
        );

        handleAttack(
            ((IsKeyDown(KEY_S) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) && canAttack),
            (((IsKeyDown(KEY_DOWN) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN))))? PLAYER_SIT_KICK : PLAYER_STAND_KICK)
        );
    }

    if (game->state == STAGE_GAME && ((IsKeyReleased(KEY_A) || (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) && IsGamepadAvailable(0))) || (IsKeyReleased(KEY_S) || (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) && IsGamepadAvailable(0)))) && !activateAttack && !showHit)
    {
        activateAttack = true;
        activateTime = 0;
    }

    if (
        game->state == STAGE_GAME 
        && (IsKeyDown(KEY_S) || (IsGamepadAvailable(0) && IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) 
        && !isFlyingKick
        && (currentMovement == PLAYER_UP || currentMovement == PLAYER_COMING_DOWN)
        && y <= (PLAYER_JUMP_HEIGHT + 24) // TODO: Not sure if it is the right Math (16 ORIGINALY)
        && canFlyingKick
        && !game->gameStage->pauseMovement
        && !game->gameStage->showVillainHit
    )
    {
        isFlyingKick = true;
        haltTimeJump = 0;
        canFlyingKick = false;
        isCollidedWithVillain();
    }
}

void Player::handleAttack(bool condition, int movement)
{
    if(condition)
    {
        inputDisabled = true;
        canAttack = false;
        setMovement(movement);
        isCollidedWithVillain();
    }
}

void Player::handleJump()
{
    if ((currentMovement == PLAYER_UP || currentMovement == PLAYER_COMING_DOWN) && !game->gameStage->showVillainHit && game->player->health > 0)
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


void Player::checkCollisionWithVillain(int *playerX, int *playerY, int *playerBoxWidth, int *playerBoxHeight, CollisionInfo collisionInfo)
{
    *playerX = (game->player->isFlipped)? game->player->x : (game->player->x + collisionInfo.x1) ;
    *playerY = (game->player->y + collisionInfo.y);
    *playerBoxWidth = collisionInfo.width;
    *playerBoxHeight = collisionInfo.height;
}

void Player::isCollidedWithVillain()
{
    int villainX = (game->gameStage->isVillainFlipped)? (game->gameStage->villainX + collisionsInfo[game->stage - 1].x2) : (game->gameStage->villainX + collisionsInfo[game->stage - 1].x1);
    int villainY = (game->gameStage->villainY + collisionsInfo[game->stage - 1].y);
    int lowerX1 = villainX + collisionsInfo[game->stage - 1].width - 1;
    int lowerY1 = villainY + collisionsInfo[game->stage - 1].height - 1;

    int lowerX2 = 0;
    int lowerY2 = 0;
    int playerX = 0;
    int playerY = 0;
    int playerBoxWidth = 0;
    int playerBoxHeight = 0;
    int scoreToAdd = 100;

    switch(currentMovement)
    {
        case PLAYER_SIT_PUNCH:
            checkCollisionWithVillain(&playerX, &playerY, &playerBoxWidth, &playerBoxHeight, collisionInfoSitPunch);
            break;
        case PLAYER_STAND_KICK:
            checkCollisionWithVillain(&playerX, &playerY, &playerBoxWidth, &playerBoxHeight, collisionInfoStandKick);
            break;
        case PLAYER_SIT_KICK:
            checkCollisionWithVillain(&playerX, &playerY, &playerBoxWidth, &playerBoxHeight, collisionInfoSitKick);
            break;
        case PLAYER_HIGH_KICK:
            checkCollisionWithVillain(&playerX, &playerY, &playerBoxWidth, &playerBoxHeight, collisionInfoHighKick);
            scoreToAdd = 200;
            break;
        case PLAYER_UP:
        case PLAYER_COMING_DOWN:
            checkCollisionWithVillain(&playerX, &playerY, &playerBoxWidth, &playerBoxHeight, collisionInfoAir);
            scoreToAdd = 300;
            break;
        default:
            //PLAYER_STAND_PUNCH
            checkCollisionWithVillain(&playerX, &playerY, &playerBoxWidth, &playerBoxHeight, collisionInfoStandPunch);
            break;
    }

    lowerX2 = playerX + playerBoxWidth - 1;
    lowerY2 = playerY + playerBoxHeight - 1;

    if (lowerX1 < playerX || villainX > lowerX2 || lowerY1 < playerY || villainY > lowerY2)
    {
        PlaySound(game->sounds.at("attack"));
        return;
    }

    // collided
    PlaySound(game->sounds.at("collided"));
    game->sprites.at("hit").x = playerX;
    game->sprites.at("hit").y = playerY;
    game->gameStage->haltTime = 0;
    game->gameStage->pauseMovement = true;
    game->score += scoreToAdd;
    showHit = true;
}