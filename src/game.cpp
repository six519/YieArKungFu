#include "game.hpp"
#include <map>

Game::Game()
{
    state = 0;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    InitAudioDevice();

    // initialize sprites
    for (auto& name : SpriteImages)
    {
        loadSprite(name);
    }

    sprites.at("letters").setTileCount(SpriteLetters.size(), 1);

    // initialize stages
    titleStage = new TitleStage(this);

    SetTargetFPS(TARGET_FPS);
}

void Game::cleanUp()
{
    // unload sprite textures
    for (auto& name : SpriteImages)
    {
        sprites.at(name).unload();
    }

    // other raylib cleanups
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

void Game::loadSprite(string name)
{
    sprites.insert({name, Sprite("assets/images/" + name + ".png")});
}