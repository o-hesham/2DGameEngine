
/////////////////////////////Show console log in imgui instead of teminal/////////////////////////////
///////////////////////////////////////////Add audio//////////////////////////////////////////////////

#include <iostream>
#include <ranges>
#include <SDL_image.h>
#include <glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "Game.h"
#include "Logger.h"
#include "ECS/ECS.h"
#include "LevelLoader.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/CollisionSystem.h"
#include "Systems/DamageSystem.h"
#include "Systems/KeyBoardControlSystem.h"
#include "Systems/CameraMovementSystem.h"
#include "Systems/ProjectileEmitSystem.h"
#include "Systems/ProjectileLifecycleSystem.h"
#include "Systems/DamageAnimationSystem.h"
#include "Systems/RenderTextSystem.h"
#include "Systems/RenderHealthBarSystem.h"
#include "Systems/GUIRenderSystem.h"
#include "Systems/ScriptSystem.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game()
    : isRunning(false), isDebug(false), registry(std::make_unique<Registry>()), assetStore(std::make_unique<AssetStore>()), eventBus(std::make_unique<EventBus>())
{
    Logger::Log("Game constructor called!");
}

Game::~Game()
{
    Logger::Log("Game destructor called!");
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Err("Error initializing SDL.");
        return;
    }

    if (TTF_Init() != 0)
    {
        Logger::Err("Error initializing SDL TTF.");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    int ww = 800;
    int wh = 600;

    window.reset(SDL_CreateWindow("My 2D Game Engine (Demo)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ww, wh, SDL_WINDOW_SHOWN)); // SDL_WINDOW_BORDERLESS
    if (!window)
    {
        Logger::Err("Error creating SDL window.");
        return;
    }

    renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!renderer)
    {
        Logger::Err("Error creating SDL renderer.");
        return;
    }

    // Initialize the ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window.get(), renderer.get());
    ImGui_ImplSDLRenderer2_Init(renderer.get());

    // Initialize the camera view with the entire screen
    camera.x = 0;
    camera.y = 0;
    camera.w = ww; // windowWidth
    camera.h = wh; // windowHeight

    // SDL_SetWindowFullscreen(window.get(), SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::ProcessInput()
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent))
    {
        // ImGui SDL input
        ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
        ImGuiIO &io = ImGui::GetIO();
        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        // Handle core SDL events (close window, key pressed, etc...)
        switch (sdlEvent.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                isRunning = false;
            }
            if (sdlEvent.key.keysym.sym == SDLK_p)
            {
                isDebug = !isDebug;
            }
            eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
            break;
        case SDL_KEYUP:
            eventBus->EmitEvent<KeyReleasedEvent>(sdlEvent.key.keysym.sym);
        }
    }
}

void Game::Setup()
{
    // Add the systems that need to be processed in our game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyBoardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileLifecycleSystem>();
    registry->AddSystem<DamageAnimationSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderHealthBarSystem>();
    registry->AddSystem<GUIRenderSystem>();
    registry->AddSystem<ScriptSystem>();

    // Create bindings between C++ and Lua
    registry->GetSystem<ScriptSystem>().CreateLuaBindings(lua);

    // Load level
    LevelLoader loader;
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
    loader.LoadLevel(lua, renderer.get(), registry, assetStore, 1);
}

void Game::Update()
{
    // If we are too fast, wait some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
    {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();

    // Reset all event handlers for the current frame
    eventBus->Reset();

    // Preform the subscription of the events of all systems
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<KeyBoardControlSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->Update();

    // Ask the systems to update
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
    registry->GetSystem<KeyBoardControlSystem>().Update();
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update();
    registry->GetSystem<ProjectileLifecycleSystem>().Update();
    registry->GetSystem<DamageAnimationSystem>().Update();
    registry->GetSystem<ScriptSystem>().Update(deltaTime, SDL_GetTicks());
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer.get(), 21, 21, 21, 255);
    SDL_RenderClear(renderer.get());

    // Ask all systems to render
    registry->GetSystem<RenderSystem>().Update(renderer.get(), assetStore, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer.get(), assetStore, camera);
    registry->GetSystem<RenderHealthBarSystem>().Update(renderer.get(), assetStore, camera);
    if (isDebug)
    {
        registry->GetSystem<CollisionSystem>().Debug(renderer.get(), camera);
        registry->GetSystem<GUIRenderSystem>().Update(renderer.get(), registry, camera, assetStore);
    }

    SDL_RenderPresent(renderer.get());
}

void Game::Run()
{
    Setup();
    while (isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_Quit();
}
