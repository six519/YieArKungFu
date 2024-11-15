#include "stage.hpp"

Stage::Stage(Game *gm)
{
    game = gm;
    initialized = false;
    timeSeconds = 0;
    timeCounter = 0;
    renderTexture = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
};

void Stage::run()
{
    if (!initialized) {
        this->init();
        initialized = true;
    }
    this->timeTick();
    this->draw();
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

void Stage::timeTick()
{
    timeCounter++;
    if (timeCounter >= (TARGET_FPS / FRAME_SPEED))
    {
        timeCounter = 0;
        if (timeSeconds == 59)
        {
            timeSeconds = 0;
            this->onTimeTick();
            return;
        }
        timeSeconds += 1;
        this->onTimeTick();
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
    if(IsKeyDown(KEY_ENTER) && !blinkEnter)
    {
        blinkEnter = true;
        PlayMusicStream(game->musics.at("bg"));
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

    if (blinkEnter)
    {
        UpdateMusicStream(game->musics.at("bg"));
    }
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

void GameStage::init(){}

void GameStage::handleKeys(){
    // making sure that it will be executed only on a specific state
    if (game->state == STAGE_GAME)
    {
        if (IsKeyDown(KEY_LEFT) && game->player->x > STAGE_BOUNDARY)
        {
            game->player->currentMovement = PLAYER_LEFT;
            game->player->x -= PLAYER_SPEED;
        }
        else if (IsKeyDown(KEY_LEFT) && game->player->x <= STAGE_BOUNDARY)
        {
            game->player->currentMovement = PLAYER_IDLE;
        }
        else if(IsKeyReleased(KEY_LEFT))
        {
            game->player->currentMovement = PLAYER_IDLE;
        } else if (IsKeyDown(KEY_RIGHT) && game->player->x < (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            game->player->currentMovement = PLAYER_RIGHT;
            game->player->x += PLAYER_SPEED;
        }
        else if(IsKeyDown(KEY_RIGHT) && game->player->x >= (GAME_WIDTH - STAGE_BOUNDARY - (game->sprites.at("player_normal").getTexture().width) / 2))
        {
            game->player->currentMovement = PLAYER_IDLE_2;
        }
        else if(IsKeyReleased(KEY_RIGHT))
        {
            game->player->currentMovement = PLAYER_IDLE;
        }
    }
}

void GameStage::stageDraw()
{
    UpdateMusicStream(game->musics.at("bg"));

    // background is the last to draw
    game->sprites.at("game_bg").draw();

    game->player->play();
}

void GameStage::onBlinkingDone(){}

void GameStage::cleanUp()
{
    game->player->clear();
    Stage::cleanUp();
}

void GameStage::onTimeTick(){}