#include "Game.h"
#include "LevelLoader.h"
#include "Logger.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/BoxColliderComponent.h"
#include "Components/KeyboardControlledComponent.h"
#include "Components/CameraFollowComponent.h"
#include "Components/ProjectileEmitterComponent.h"
#include "Components/HealthComponent.h"
#include "Components/DamageAnimationComponent.h"
#include "Components/TextLabelComponent.h"
#include "Components/ScriptComponent.h"
#include <glm.hpp>
#include <fstream>
#include <ranges>

LevelLoader::LevelLoader()
{
    Logger::Log("LevelLoader constructor called!");
}

LevelLoader::~LevelLoader()
{
    Logger::Log("LevelLoader destructor called!");
}

void LevelLoader::LoadLevel(sol::state &lua, SDL_Renderer *renderer, const std::unique_ptr<Registry> &registry, const std::unique_ptr<AssetStore> &assetStore, int levelNumber)
{
    // Checks syntax of script
    sol::load_result script = lua.load_file("../assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
    if (!script.valid())
    {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }

    // Executes the script
    lua.script_file("../assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

    sol::table level = lua["Level"];

    //////////////////////////////////////////////////////////////////////////////
    // Read the level assets
    //////////////////////////////////////////////////////////////////////////////
    sol::table assets = level["assets"];

    int i = 0;
    while (true)
    {
        sol::optional<sol::table> hasAsset = assets[i];
        if (hasAsset == sol::nullopt)
        {
            break;
        }

        sol::table asset = assets[i];
        if (asset["type"].get<std::string>() == "texture")
        {
            assetStore->AddTexture(renderer, asset["id"], asset["file"]);
            Logger::Log(std::string("New texture asset loaded to the asset store, id: ") + asset["id"].get<std::string>());
        }
        else if (asset["type"].get<std::string>() == "font")
        {
            assetStore->AddFont(asset["id"], asset["file"], asset["font_size"]);
            Logger::Log(std::string("New font asset loaded to the asset store, id: ") + asset["id"].get<std::string>());
        }

        i++;
    }

    //////////////////////////////////////////////////////////////////////////////
    // Read the level tilemap information
    //////////////////////////////////////////////////////////////////////////////
    sol::table map = level["tilemap"];
    std::string mapFilePath = map["map_file"];
    std::string mapTextureId = map["texture_asset_id"];
    int mapNumRows = map["num_rows"];
    int mapNumCols = map["num_cols"];
    int tileSize = map["tile_size"];
    int tilesetCols = map["tile_set_cols"];
    float mapScale = map["map_Scale"];
    if (std::ifstream mapFile{mapFilePath}; mapFile)
    {
        if (mapFilePath == "../assets/tilemaps/desert.map")
        {
            for (int y = 0; y < mapNumRows; y++)
            {
                for (int x = 0; x < mapNumCols; x++)
                {
                    char ch;
                    mapFile.get(ch);
                    int srcRectY = std::atoi(&ch) * tileSize;
                    mapFile.get(ch);
                    int srcRectX = std::atoi(&ch) * tileSize;
                    mapFile.ignore();

                    Entity tile = registry->CreateEntity();
                    tile.AddComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)), glm::vec2(mapScale, mapScale), 0.0);
                    tile.AddComponent<SpriteComponent>(mapTextureId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
                }
            }
        }
        else
        {
            auto lines = std::views::istream<std::string>(mapFile);
            for (auto [rowIdx, line] : std::views::enumerate(lines))
            {
                auto tiles = std::views::split(line, ',');
                for (auto [colIdx, part] : std::views::enumerate(tiles))
                {
                    std::string tileString;
                    for (char c : part)
                        tileString.push_back(c);

                    if (tileString.empty())
                        continue;

                    int id = std::stoi(tileString);

                    int adjustedId = id - 1;
                    int tileY = adjustedId / tilesetCols;
                    int tileX = adjustedId % tilesetCols;

                    Entity tile = registry->CreateEntity();
                    tile.Tag("tiles");
                    tile.AddComponent<TransformComponent>(glm::vec2{colIdx * tileSize * mapScale, rowIdx * tileSize * mapScale},
                                                          glm::vec2{mapScale, mapScale}, 0.0);
                    tile.AddComponent<SpriteComponent>(mapTextureId, tileSize, tileSize, 0, false, tileX * tileSize, tileY * tileSize);
                }
            }
        }
        Game::mapWidth = mapNumCols * tileSize * mapScale;
        Game::mapHeight = mapNumRows * tileSize * mapScale;
    }
    else
    {
        Logger::Err("Failed to open map file!");
    }

    //////////////////////////////////////////////////////////////////////////////
    // Read the level entities and components
    //////////////////////////////////////////////////////////////////////////////
    sol::table entities = level["entities"];
    i = 0;
    while (true)
    {
        sol::optional<sol::table> hasEntity = entities[i];
        if (hasEntity == sol::nullopt)
        {
            break;
        }

        sol::table entity = entities[i];

        Entity newEntity = registry->CreateEntity();

        // Tag
        sol::optional<std::string> tag = entity["tag"];
        if (tag != sol::nullopt)
        {
            newEntity.Tag(entity["tag"]);
        }

        // Group
        sol::optional<std::string> group = entity["group"];
        if (group != sol::nullopt)
        {
            newEntity.Group(entity["group"]);
        }

        // Components
        sol::optional<sol::table> hasComponents = entity["components"];
        if (hasComponents != sol::nullopt)
        {
            // Transform
            sol::optional<sol::table> transform = entity["components"]["transform"];
            if (transform != sol::nullopt)
            {
                newEntity.AddComponent<TransformComponent>(
                    glm::vec2(entity["components"]["transform"]["position"]["x"].get<double>() * mapScale,
                              entity["components"]["transform"]["position"]["y"].get<double>() * mapScale),
                    glm::vec2(entity["components"]["transform"]["scale"]["x"].get_or(1.0),
                              entity["components"]["transform"]["scale"]["y"].get_or(1.0)),
                    entity["components"]["transform"]["rotation"].get_or(0.0));
            }

            // RigidBody
            sol::optional<sol::table> rigidbody = entity["components"]["rigidbody"];
            if (rigidbody != sol::nullopt)
            {
                newEntity.AddComponent<RigidBodyComponent>(
                    glm::vec2(entity["components"]["rigidbody"]["velocity"]["x"].get_or(0.0),
                              entity["components"]["rigidbody"]["velocity"]["y"].get_or(0.0)));
            }

            // Sprite
            sol::optional<sol::table> sprite = entity["components"]["sprite"];
            if (sprite != sol::nullopt)
            {
                newEntity.AddComponent<SpriteComponent>(
                    entity["components"]["sprite"]["texture_asset_id"],
                    entity["components"]["sprite"]["width"],
                    entity["components"]["sprite"]["height"],
                    entity["components"]["sprite"]["z_index"].get_or(1),
                    entity["components"]["sprite"]["fixed"].get_or(false),
                    entity["components"]["sprite"]["src_rect_x"].get_or(0),
                    entity["components"]["sprite"]["src_rect_y"].get_or(0));
            }

            // Animation
            sol::optional<sol::table> animation = entity["components"]["animation"];
            if (animation != sol::nullopt)
            {
                newEntity.AddComponent<AnimationComponent>(
                    entity["components"]["animation"]["num_frames"].get_or(1),
                    entity["components"]["animation"]["speed_rate"].get_or(1));
            }

            // Box Collider
            sol::optional<sol::table> collider = entity["components"]["box_collider"];
            if (collider != sol::nullopt)
            {
                newEntity.AddComponent<BoxColliderComponent>(
                    entity["components"]["box_collider"]["width"],
                    entity["components"]["box_collider"]["height"],
                    glm::vec2(entity["components"]["box_collider"]["offset"]["x"].get_or(0),
                              entity["components"]["box_collider"]["offset"]["y"].get_or(0)));
            }

            // Health
            sol::optional<sol::table> health = entity["components"]["health"];
            if (health != sol::nullopt)
            {
                newEntity.AddComponent<HealthComponent>(
                    static_cast<int>(entity["components"]["health"]["health_percentage"].get_or(100)));
            }

            // Projectile Emitter
            sol::optional<sol::table> projectileEmitter = entity["components"]["projectile_emitter"];
            if (projectileEmitter != sol::nullopt)
            {
                newEntity.AddComponent<ProjectileEmitterComponent>(
                    glm::vec2(entity["components"]["projectile_emitter"]["projectile_velocity"]["x"],
                              entity["components"]["projectile_emitter"]["projectile_velocity"]["y"]),
                    static_cast<int>(entity["components"]["projectile_emitter"]["repeat_frequency"].get_or(10) * 1000),
                    static_cast<int>(entity["components"]["projectile_emitter"]["projectile_duration"].get_or(1) * 1000),
                    static_cast<int>(entity["components"]["projectile_emitter"]["hit_percentage_damage"].get_or(10)),
                    entity["components"]["projectile_emitter"]["friendly"].get_or(false));
            }

            // Damage Animation
            sol::optional<sol::table> damageAnimation = entity["components"]["damage_animation"];
            if (damageAnimation != sol::nullopt)
            {
                newEntity.AddComponent<DamageAnimationComponent>(
                    entity["components"]["damage_animation"]["num_blinks"],
                    entity["components"]["damage_animation"]["blink_on_duration"],
                    entity["components"]["damage_animation"]["blink_off_duration"]);
            }

            // Keyboard Controller
            sol::optional<sol::table> keyboardController = entity["components"]["keyboard_controller"];
            if (keyboardController != sol::nullopt)
            {
                newEntity.AddComponent<KeyboardControlledComponent>(
                    entity["components"]["keyboard_controller"]["speed"].get_or(0.0f));
            }

            // Camera Follow
            sol::optional<sol::table> cameraFollow = entity["components"]["camera_follow"];
            if (cameraFollow != sol::nullopt)
            {
                newEntity.AddComponent<CameraFollowComponent>();
            }

            // Script Component
            sol::optional<sol::table> script = entity["components"]["on_update_script"];
            if (script != sol::nullopt)
            {
                sol::function func = entity["components"]["on_update_script"][0];
                newEntity.AddComponent<ScriptComponent>(func);
            }
        }
        i++;
    }
}