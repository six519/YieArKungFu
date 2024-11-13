#include "game.hpp"

Game::Game()
{
    state = 0;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    InitAudioDevice();

    titleStage = new TitleStage(this);

    SetTargetFPS(TARGET_FPS);
}

void Game::cleanUp()
{
    CloseAudioDevice();
}

void Game::run()
{
    while(true)
    {
        if(IsKeyDown(KEY_ESCAPE) || WindowShouldClose())
            break;

        titleStage->run();
    }

    cleanUp();
    CloseWindow();
}