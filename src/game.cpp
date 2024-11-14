#include "game.hpp"
#include <map>

Game::Game()
{
    state = 0;
    stage = 1;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    InitAudioDevice();

    // initialize sprites
    for (auto& name : SpriteImages)
    {
        loadSprite(name);
    }

    sprites.at("letters").setTileCount(SpriteLetters.size());

    // initialize musics
    for (auto& name : MusicsList)
    {
        loadMusic(name);
    }

    // initialize stages
    titleStage = new TitleStage(this);
    viewStage = new ViewStage(this);
    gameStage = new GameStage(this);

    SetTargetFPS(TARGET_FPS);
}

void Game::cleanUp()
{
    // unload sprite textures
    for (auto& name : SpriteImages)
    {
        sprites.at(name).unload();
    }

    // unload musics
    for (auto& name : MusicsList)
    {
        UnloadMusicStream(musics.at(name));
    }

    // unload render textures on stages
    titleStage->unloadTexture();
    viewStage->unloadTexture();
    gameStage->unloadTexture();

    // other raylib cleanups
    CloseAudioDevice();
}

void Game::run()
{
    while(true)
    {
        if(IsKeyDown(KEY_ESCAPE) || WindowShouldClose())
            break;

        switch(state)
        {
            case STAGE_VIEW:
                viewStage->run();
                break;
            case STAGE_GAME:
                gameStage->run();
                break;
            default:
                titleStage->run();
                break;
        }
    }

    cleanUp();
    CloseWindow();
}

void Game::loadSprite(string name)
{
    sprites.insert({name, Sprite("assets/images/" + name + ".png")});
}

void Game::loadMusic(string name)
{
    musics.insert({name, LoadMusicStream(("assets/musics/" + name + ".mp3").c_str())});
}