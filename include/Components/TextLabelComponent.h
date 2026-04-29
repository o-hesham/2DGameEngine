#pragma once

#include <glm.hpp>
#include <string>
#include <SDL.h>

struct TextLabelComponent
{
    glm::vec2 position;
    std::string text;
    std::string assetId;
    SDL_Color color;
    bool isFixed;

    TextLabelComponent(glm::vec2 position = glm::vec2(0), std::string text = "", std::string assetId = "", SDL_Color color = {0, 0, 0}, bool isFixed = true)
        : position(position), text(text), assetId(assetId), color(color), isFixed(isFixed)
    {
    }
};
