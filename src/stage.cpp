#include "stage.hpp"

Stage::Stage(Game *gm)
{
    game = gm;
    initialized = false;
    renderTexture = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
};

void Stage::run()
{
    if (!initialized) {
        this->init();
        initialized = true;
    }
    this->draw();
    this->handleKeys();
}

void Stage::cleanUp()
{
    UnloadRenderTexture(renderTexture);
}


void TitleStage::init()
{
}

void TitleStage::handleKeys()
{
    if(IsKeyDown(KEY_ENTER))
    {
        //initialized = false;
        //this->cleanUp();
    }
}

void Stage::draw()
{
    BeginDrawing();
    BeginTextureMode(renderTexture);

    this->stageDraw();

    EndTextureMode();
    ClearBackground(BLACK);

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

void TitleStage::stageDraw()
{
}

void TitleStage::cleanUp()
{
    Stage::cleanUp();
}