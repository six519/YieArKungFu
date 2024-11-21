#include "stage.hpp"

Stage::Stage(Game *gm)
{
    game = gm;
    Stage::cleanUp();
    renderTexture = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
};

void Stage::run()
{
    if (!initialized) {
        this->init();
        initialized = true;
    }
    this->draw();
    this->timeTick();
    this->handleKeys();
}

void Stage::cleanUp()
{
    initialized = false;
    timeSeconds = 0;
    timeCounter = 0;
}

void Stage::unloadTexture()
{
    UnloadRenderTexture(renderTexture);
}

void Stage::draw()
{
    BeginDrawing();
    BeginTextureMode(renderTexture);

    ClearBackground(BLACK);
    this->stageDraw();

    EndTextureMode();

    Rectangle srcRect = (Rectangle){ 
        0.0, 
        0.0, 
        static_cast<float>(GAME_WIDTH), 
        static_cast<float>(-GAME_HEIGHT) 
    };
    Rectangle dstRect = (Rectangle) { 
        static_cast<float>((SCREEN_WIDTH / 2.0) - ( (SCREEN_WIDTH * ((float)GAME_HEIGHT / (float)GAME_WIDTH)) / 2.0)), 
        0.0, 
        static_cast<float>(SCREEN_WIDTH * ((float)GAME_HEIGHT / (float)GAME_WIDTH)), 
        static_cast<float>(SCREEN_HEIGHT) 
    };
    DrawTexturePro(renderTexture.texture, srcRect , dstRect, (Vector2){ 0.0, 0.0 }, 0.0, WHITE);  
    EndDrawing();
}

void Stage::drawText(string text, int x, int y, bool blink)
{

    if (currentFrame.count(text) == 0)
    {
        // add to map
        currentFrame.insert({text, 0});
        framesCounter.insert({text, 0});
    }

    game->sprites.at("letters").x = x;
    game->sprites.at("letters").y = y;

    if (blink)
    {
        framesCounter.at(text)++;
        if (framesCounter.at(text) >= (TARGET_FPS / FRAME_SPEED))
        {
            framesCounter.at(text) = 0;
            currentFrame.at(text)++;

            if (currentFrame.at(text) > 1)
            {
                currentFrame.at(text) = 0;
                this->onBlinkingDone();
            }
        }
    }

    for (size_t i = 0; i < text.size(); i++)
    {
        auto it = find(SpriteLetters.begin(), SpriteLetters.end(), text[i]);
        if (it == SpriteLetters.end())
        {
            return;
        }

        if (!blink)
        {
            game->sprites.at("letters").drawByIndex(distance(SpriteLetters.begin(), it));
            game->sprites.at("letters").x += LETTER_WIDTH;
            continue;
        }

        //blinking
        if (currentFrame.at(text) == 1)
        {
            game->sprites.at("letters").drawByIndex(distance(SpriteLetters.begin(), it));
            game->sprites.at("letters").x += LETTER_WIDTH;
            continue;
        }

        game->sprites.at("letters").drawByIndex(SpriteLetters.size() - 2);
        game->sprites.at("letters").x += LETTER_WIDTH;
    }
}

void TitleStage::init()
{
    // set konami logo to center
    game->sprites.at("konami_logo").y = 35;
    game->sprites.at("konami_logo").x = (GAME_WIDTH / 2) - (game->sprites.at("konami_logo").getTexture().width / 2);
    // set title to center
    game->sprites.at("title").y = 85;
    game->sprites.at("title").x = (GAME_WIDTH / 2) - (game->sprites.at("title").getTexture().width / 2);
}

void TitleStage::handleKeys()
{
    if(IsKeyDown(KEY_ENTER) && !blinkEnter && canEnter)
    {
        blinkEnter = true;
        PlayMusicStream(game->musics.at("bg"));
    }
    else if (IsKeyReleased(KEY_ENTER))
    {
        canEnter = true;
    }
}

void TitleStage::stageDraw()
{
    game->sprites.at("konami_logo").draw();
    game->sprites.at("title").draw();

    //draw copyright text
    drawText(CopyrightText, (GAME_WIDTH / 2) - ((CopyrightText.size() * LETTER_WIDTH) / 2), 110, false);
    drawText(OtherText, (GAME_WIDTH / 2) - ((OtherText.size() * LETTER_WIDTH) / 2), 120, false);

    // press enter to start
    drawText(ToStartText, (GAME_WIDTH / 2) - ((ToStartText.size() * LETTER_WIDTH) / 2), 165, blinkEnter);

    UpdateMusicStream(game->musics.at("bg"));
}

void TitleStage::onBlinkingDone()
{
    if (blinkCount == maxBlink)
    {
        game->state = STAGE_VIEW;
        this->cleanUp();
        return;
    }

    blinkCount += 1;
}

void TitleStage::cleanUp()
{
    blinkEnter = false;
    blinkCount = 0;
    Stage::cleanUp();
}

void TitleStage::onTimeTick(){}

void ViewStage::init(){}

void ViewStage::handleKeys(){}

void ViewStage::stageDraw()
{
    UpdateMusicStream(game->musics.at("bg"));
    drawText(
        "stage 0" + to_string(game->stage), 
        (GAME_WIDTH / 2) - ((8 * LETTER_WIDTH) / 2), 
        (GAME_HEIGHT / 2) - (LETTER_WIDTH / 2), 
        false
    );
}

void ViewStage::onBlinkingDone(){}

void ViewStage::cleanUp()
{
    Stage::cleanUp();
}

void ViewStage::onTimeTick()
{
    if (timeSeconds == 10)
    {
        game->state = STAGE_GAME;
        this->cleanUp();
    }
}

void GameStage::init()
{
    game->sprites.at("life").y = 48;

    game->sprites.at("health_hud").y = 208;
    game->sprites.at("health_hud").x = (GAME_WIDTH / 2) - (game->sprites.at("health_hud").getTexture().width / 2);

    game->sprites.at("health_green").y = 210;
    game->sprites.at("health_red").y = 210;

    game->sprites.at("spinning_chain").overrideFrameSpeed(SPINNING_CHAIN_SPEED);

    reset();
}

void GameStage::run()
{
    Stage::run();
    if (!pauseMovement)
    {
        game->player->timeTick();
        game->player->handleJump();
    }
}

void GameStage::handleKeys()
{
    if (game->player->health > 0 && villainHealth > 0)
        game->player->handleKeys();

    if (game->gameStage->endState == END_STATE_GAME_OVER && IsKeyDown(KEY_ENTER))
    {
        cleanUp();
        game->state = 0;
        game->stage = 1;
        game->score = 0;
        game->titleStage->canEnter = false;
    }
}

void GameStage::stageDraw()
{
    UpdateMusicStream(game->musics.at("bg"));

    // background is the last to draw
    game->sprites.at("game_bg").draw();

    drawText(OtherText, (GAME_WIDTH / 2) - ((OtherText.size() * LETTER_WIDTH) / 2), 24, false);

    drawText(
        "stage-0" + to_string(game->stage), 
        168, 
        40, 
        false
    );

    drawText("score", 24, 40, false);
    drawText(to_string(game->score), 24, 48, false);

    game->sprites.at("life").x = 168;
    for (int x = 0; x < game->player->lives; x++)
    {
        game->sprites.at("life").draw();
        game->sprites.at("life").x += 8;
    }

    drawText("ferdie", 48, (GAME_HEIGHT - 24), false);
    drawText(Villains[game->stage - 1], (208 - (Villains[game->stage - 1].size() * 8)), (GAME_HEIGHT - 24), false);

    game->sprites.at("health_hud").draw();

    game->sprites.at("health_green").x = 104;
    game->sprites.at("health_red").x = 104;

    //draw player's health gauge
    for (int x = 0; x < game->player->health; x++)
    {
        game->sprites.at("health_green").draw();
        game->sprites.at("health_green").x -= 8;
    }

    //draw villain's health gauge

    game->sprites.at("health_green").x = 144;
    game->sprites.at("health_red").x = 144;

    for (int x = 0; x < villainHealth; x++)
    {
        string h_hud = (villainHealth > LOW_HEALTH)? "green" : "red";
        game->sprites.at("health_" + h_hud).draw();
        game->sprites.at("health_" + h_hud).x += 8;
    }

    // show villain
    showVillain();

    // show player
    game->player->play();


    if (game->gameStage->endState == END_STATE_GAME_OVER)
    {
        drawText(
            "game over", 
            (GAME_WIDTH / 2) - ((9 * LETTER_WIDTH) / 2), 
            (GAME_HEIGHT / 2) - (LETTER_WIDTH / 2), 
            false
        );

        drawText(
            "you win", 
            (GAME_WIDTH / 2) - ((7 * LETTER_WIDTH) / 2), 
            ((GAME_HEIGHT / 2) - (LETTER_WIDTH / 2)) + 8, 
            false
        );
    }

}

void GameStage::onBlinkingDone(){}

void GameStage::cleanUp()
{
    reset();
    game->player->clear();
    Stage::cleanUp();
}

void GameStage::onTimeTick()
{
    if (pauseMovement)
    {
        haltTime += 1;

        if (haltTime == 2)
        {
            pauseMovement = false;
            haltTime = 0;

            if (game->player->currentMovement == PLAYER_UP || game->player->currentMovement == PLAYER_COMING_DOWN)
                game->player->showHit = false;

            villainHealth -= 1;
            if (villainHealth == 0)
            {
                StopMusicStream(game->musics.at("bg"));
                haltTime = 0;
            }
        }
    }

    if (game->player->health == 0 || villainHealth == 0)
    {
        haltTime += 1;
        if (haltTime == maxHaltTime)
        {
            handleEndState();
            haltTime = 0;
        }
    }
}

void GameStage::handleEndState()
{
    //showtime: p, hk, lk, lk, hk, p
    switch(endState)
    {
        case END_STATE_PLAY_SOUND:
            PlaySound(game->sounds.at("win"));
            endState = END_STATE_SHOWTIME;
            break;
        case END_STATE_SHOWTIME:
            setEndStateWithPlayerMovement(PLAYER_STAND_PUNCH, false, true);
            break;
        case END_STATE_SHOWTIME_HK1:
            setEndStateWithPlayerMovement(PLAYER_HIGH_KICK, true, true);
            break;
        case END_STATE_SHOWTIME_LK1:
            setEndStateWithPlayerMovement(PLAYER_SIT_KICK, true, true);
            break;
        case END_STATE_SHOWTIME_LK2:
            setEndStateWithPlayerMovement(PLAYER_SIT_KICK, true, true);
            break;
        case END_STATE_SHOWTIME_HK2:
            setEndStateWithPlayerMovement(PLAYER_HIGH_KICK, true, true);
            break;
        case END_STATE_SHOWTIME_P:
            setEndStateWithPlayerMovement(PLAYER_STAND_PUNCH, true, true);
            break;
        case END_STATE_SMILE:
            setEndStateWithPlayerMovement(PLAYER_SMILE, true, false);
            break;
        case END_STATE_COUNT_LIFE:
            maxHaltTime = LOW_TIME;
            if (game->player->health > 0)
            {
                game->player->health -= 1;
                PlaySound(game->sounds.at("counting"));
                game->score += 100;
                return;
            }
            endState = END_STATE_END;
            break;
        case END_STATE_END:
            maxHaltTime = HIGH_TIME;
            if (game->stage == 5)
            {
                PlaySound(game->sounds.at("game_over"));
                endState = END_STATE_GAME_OVER;
                return;
            }
            cleanUp();
            game->stage += 1;
            game->state = STAGE_VIEW;
            PlayMusicStream(game->musics.at("bg"));
            break;
        case END_STATE_GAME_OVER:
            break;
        default:
            // END_STATE_START
            villainCurrentMove = VILLAIN_MOVE_DEAD;
            PlaySound(game->sounds.at("dead"));
            endState = END_STATE_PLAY_SOUND;
            break;
    }
}

void GameStage::setEndStateWithPlayerMovement(int pMove, bool flip, bool playSound)
{
    if (flip)
        game->player->flipSprites();
    game->player->setMovement(pMove);
    if (playSound)
        PlaySound(game->sounds.at("attack"));
    endState += 1;
}

void GameStage::setVillainSpritesCoordinates()
{
    for(int x = 0; x < sizeof(VillainSprites) / sizeof(VillainSprites[0]); x++)
    {
        game->sprites.at(Villains[game->stage - 1] + "_" + VillainSprites[x]).x = villainX;
        game->sprites.at(Villains[game->stage - 1] + "_" + VillainSprites[x]).y = villainY;
    }
}

void GameStage::showVillain()
{
    setVillainSpritesCoordinates();

    switch(villainCurrentMove)
    {
        case VILLAIN_MOVE_LEFT:
            break;
        case VILLAIN_MOVE_DEAD:
            game->sprites.at(Villains[game->stage - 1] + "_dead").draw();
            break;
        default:
            // VILLAIN_MOVE_IDLE

            if (game->stage == 3)
            {
                game->sprites.at("spinning_chain").x = spinningChainX;
                game->sprites.at("spinning_chain").y = spinningChainY;
                game->sprites.at("spinning_chain").play();
            }

            game->sprites.at(Villains[game->stage - 1] + "_normal").drawByIndex(0);
            break;
    }

    //flip checker
    if ((game->player->x) > villainX && !isVillainFlipped)
    {
        flipVillainSprites();
    }
    if ((villainX) > game->player->x  && isVillainFlipped)
    {
        flipVillainSprites();
    }
}

void GameStage::reset()
{
    villainCurrentMove = VILLAIN_MOVE_IDLE;
    villainHealth = DEFAULT_HEALTH;
    villainX = VILLAIN_DEFAULT_X;
    villainY = VILLAIN_DEFAULT_Y;
    pauseMovement = false;
    haltTime = 0;
    maxHaltTime = HIGH_TIME;

    spinningChainX = 140;
    spinningChainY = 155;

    endState = END_STATE_START;

    if (isVillainFlipped)
    {
        flipVillainSprites();
    }
}

void GameStage::flipVillainSprites()
{
    for(int x = 0; x < sizeof(VillainSprites) / sizeof(VillainSprites[0]); x++)
    {
        game->sprites.at(Villains[game->stage - 1] + "_" + VillainSprites[x]).flipHorizontal();   
    }
    game->sprites.at("spinning_chain").flipHorizontal();
    isVillainFlipped = !isVillainFlipped;

    if (isVillainFlipped)
    {
        spinningChainX -= 19;
        return;
    }

    spinningChainX += 19;
}