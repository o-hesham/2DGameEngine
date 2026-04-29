#pragma once

#include <memory>
#include <SDL.h>
#include <sol/sol.hpp>
#include "ECS/ECS.h"
#include "AssetStore/AssetStore.h"
#include "EventBus/EventBus.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
    struct SDLWindowDeleter
    {
        void operator()(SDL_Window *w) const noexcept { SDL_DestroyWindow(w); };
    };

    struct SDLRendererDeleter
    {
        void operator()(SDL_Renderer *r) const noexcept { SDL_DestroyRenderer(r); }
    };

private:
    bool isRunning;
    bool isDebug;
    size_t millisecsPreviousFrame = 0;

    std::unique_ptr<SDL_Window, SDLWindowDeleter> window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> renderer;

    SDL_Rect camera;

    sol::state lua;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> assetStore;
    std::unique_ptr<EventBus> eventBus;

public:
    Game();
    ~Game();

    void Initialize();
    void Run();
    void Setup();
    void LoadTilemap(const std::string &filepath);
    void ProcessInput();
    void Update();
    void Render();

    void Destroy();

    static int windowWidth;
    static int windowHeight;
    static int mapWidth;
    static int mapHeight;
};