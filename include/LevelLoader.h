#pragma once

#include "ECS/ECS.h"
#include "AssetStore/AssetStore.h"
#include <SDL.h>

class LevelLoader
{
public:
    LevelLoader();
    ~LevelLoader();

    void LoadLevel(sol::state &lua, SDL_Renderer *renderer, const std::unique_ptr<Registry> &registry, const std::unique_ptr<AssetStore> &assetStore, int levelNumber);
};
