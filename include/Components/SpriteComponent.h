#pragma once

#include "glm.hpp"
#include "SDL.h"

struct SpriteComponent
{
    std::string assetId;

    int width;
    int height;
    bool isFixed;
    size_t zIndex;

    SDL_Rect srcRect;

    SpriteComponent()
        : assetId(""),
          width(0), height(0), isFixed(false), zIndex(0), srcRect{0, 0, 0, 0} {}

    SpriteComponent(std::string assetId, int width, int height, int zIndex, bool isFixed = false, int srcRectX = 0, int srcRectY = 0)
        : assetId(assetId), width(width), height(height), isFixed(isFixed), zIndex(zIndex), srcRect{srcRectX, srcRectY, width, height} {}
};