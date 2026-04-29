#include "AssetStore/AssetStore.h"
#include "Logger.h"
#include "SDL_image.h"

AssetStore::AssetStore()
{
    Logger::Log("AssetStore constructor called!");
}

AssetStore::~AssetStore()
{
    ClearAssets();
    Logger::Log("AssetStore destructor called!");
}

void AssetStore::ClearAssets()
{
    for (auto &[key, value] : textures)
    {
        SDL_DestroyTexture(value);
    }
    textures.clear();

    for (auto &[key, value] : fonts)
    {
        TTF_CloseFont(value);
    }
    fonts.clear();
}

void AssetStore::AddTexture(SDL_Renderer *renderer, const std::string &assetId, const std::string &filePath)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Add the texture to the map
    textures.emplace(assetId, texture);

    Logger::Log("New  texture added to the Asset Store with id = " + assetId);
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId)
{
    auto it = textures.find(assetId);
    if (it == textures.end())
    {
        Logger::Err("Textures not found: " + assetId);
        return nullptr;
    }
    return it->second;
}

void AssetStore::AddFont(const std::string &assetId, const std::string &filePath, size_t fontSize)
{
    fonts.emplace(assetId, TTF_OpenFont(filePath.c_str(), fontSize));
}

TTF_Font *AssetStore::GetFont(const std::string &assetId)
{
    auto it = fonts.find(assetId);
    if (it == fonts.end())
    {
        Logger::Err("Font not found: " + assetId);
        return nullptr;
    }
    return it->second;
}
