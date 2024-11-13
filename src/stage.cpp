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

void Stage::drawText(string text, int x, int y)
{
    game->sprites.at("letters").x = x;
    game->sprites.at("letters").y = y;

    for (size_t i = 0; i < text.size(); i++)
    {
        auto it = find(SpriteLetters.begin(), SpriteLetters.end(), text[i]);

        if (it != SpriteLetters.end())
        {
            game->sprites.at("letters").drawByIndex(distance(SpriteLetters.begin(), it));
            game->sprites.at("letters").x += LETTER_WIDTH;
        }
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
    if(IsKeyDown(KEY_ENTER))
    {
        //initialized = false;
        //this->cleanUp();
    }
}

void TitleStage::stageDraw()
{
    game->sprites.at("konami_logo").draw();
    game->sprites.at("title").draw();

    //draw copyright text
    drawText(CopyrightText, (GAME_WIDTH / 2) - ((CopyrightText.size() * LETTER_WIDTH) / 2), 110);
    drawText(OtherText, (GAME_WIDTH / 2) - ((OtherText.size() * LETTER_WIDTH) / 2), 120);

    // press enter to start
    drawText(ToStartText, (GAME_WIDTH / 2) - ((ToStartText.size() * LETTER_WIDTH) / 2), 165);
}

void TitleStage::cleanUp()
{
    Stage::cleanUp();
}