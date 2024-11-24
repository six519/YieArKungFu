#include "game.hpp"
#include <map>

Game::Game()
{
    state = 0;
    stage = 1;
    score = 0;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
    InitAudioDevice();

    // initialize sprites
    for (auto& name : SpriteImages)
    {
        loadSprite(name);
    }

    sprites.at("letters").setTileCount(SpriteLetters.size());
    sprites.at("player_normal").setTileCount(2);
    sprites.at("wang_normal").setTileCount(2);
    sprites.at("tao_normal").setTileCount(2);
    sprites.at("chen_normal").setTileCount(4);
    sprites.at("lang_normal").setTileCount(2);
    sprites.at("mu_normal").setTileCount(2);
    sprites.at("spinning_chain").setTileCount(8);

    sprites.at("wang_kick").setTileCount(2);
    sprites.at("tao_kick").setTileCount(2);
    sprites.at("chen_kick").setTileCount(2);
    sprites.at("lang_kick").setTileCount(2);
    sprites.at("mu_kick").setTileCount(2);

    sprites.at("wang_other").setTileCount(2);
    sprites.at("tao_other").setTileCount(2);
    sprites.at("chen_other").setTileCount(2);
    sprites.at("lang_other").setTileCount(2);
    sprites.at("mu_other").setTileCount(2);

    //override speeds of villain sprites    
    for(int x = 0; x < sizeof(Villains) / sizeof(Villains[0]); x++)
    {
        sprites.at(Villains[x] + "_normal").overrideFrameSpeed(VILLAIN_SPRITE_FRAME_SPEED);
        sprites.at(Villains[x] + "_kick").overrideFrameSpeed(VILLAIN_SPRITE_FRAME_SPEED);
        sprites.at(Villains[x] + "_other").overrideFrameSpeed(VILLAIN_SPRITE_FRAME_SPEED);
    }

    // initialize musics
    for (auto& name : MusicsList)
    {
        loadMusic(name);
    }

    // initialize sounds
    for (auto& name : SoundsList)
    {
        loadSound(name);
    }

    // initialize stages
    titleStage = new TitleStage(this);
    viewStage = new ViewStage(this);
    gameStage = new GameStage(this);

    // initialize player
    player = new Player(this);

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

    // unload sounds
    for (auto& name : SoundsList)
    {
        UnloadSound(sounds.at(name));
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

void Game::loadSound(string name)
{
    sounds.insert({name, LoadSound(("assets/sounds/" + name + ".wav").c_str())});
}