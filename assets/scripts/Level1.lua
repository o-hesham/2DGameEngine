--Load a different tilemap image depending on the current system time
local current_system_hour = os.date("*t").hour

local map_texture_asset_id

--Use a day-map or night-map texture (9am - 6pm)
if current_system_hour >= 7 and current_system_hour < 18 then
    map_texture_asset_id = "tilemap-texture-day"
else
    map_texture_asset_id = "tilemap-texture-night"
end

Level =
{
    ------------------------------------------------
    -- Table to define the list of assets
    ------------------------------------------------
    assets = {
        [0] =
        { type = "texture", id = "tilemap-texture-day", file = "../assets/tilemaps/jungle.png" },
        { type = "texture", id = "tilemap-texture-night",       file = "../assets/tilemaps/jungle-night.png" },
        { type = "texture", id = "chopper-texture",             file = "../assets/images/chopper-green-spritesheet.png" },
        { type = "texture", id = "su27-texture",                file = "../assets/images/su27-spritesheet.png" },
        { type = "texture", id = "f22-texture",                 file = "../assets/images/f22-spritesheet.png" },
        { type = "texture", id = "fw190-texture",               file = "../assets/images/fw190-spritesheet.png" },
        { type = "texture", id = "upf7-texture",                file = "../assets/images/upf7-spritesheet.png" },
        { type = "texture", id = "bf109-texture",               file = "../assets/images/bf109-spritesheet.png" },
        { type = "texture", id = "bomber-texture",              file = "../assets/images/bomber-spritesheet.png" },
        { type = "texture", id = "carrier-texture",             file = "../assets/images/carrier.png" },
        { type = "texture", id = "boat-texture",                file = "../assets/images/boat.png" },
        { type = "texture", id = "tank-tiger-up-texture",       file = "../assets/images/tank-tiger-up.png" },
        { type = "texture", id = "tank-tiger-right-texture",    file = "../assets/images/tank-tiger-right.png" },
        { type = "texture", id = "tank-tiger-down-texture",     file = "../assets/images/tank-tiger-down.png" },
        { type = "texture", id = "tank-tiger-left-texture",     file = "../assets/images/tank-tiger-left.png" },
        { type = "texture", id = "tank-tiger-killed-texture",   file = "../assets/images/tank-tiger-killed.png" },
        { type = "texture", id = "tank-panther-up-texture",     file = "../assets/images/tank-panther-up.png" },
        { type = "texture", id = "tank-panther-right-texture",  file = "../assets/images/tank-panther-right.png" },
        { type = "texture", id = "tank-panther-down-texture",   file = "../assets/images/tank-panther-down.png" },
        { type = "texture", id = "tank-panther-left-texture",   file = "../assets/images/tank-panther-left.png" },
        { type = "texture", id = "tank-panther-killed-texture", file = "../assets/images/tank-panther-killed.png" },
        { type = "texture", id = "truck-ford-up-texture",       file = "../assets/images/truck-ford-up.png" },
        { type = "texture", id = "truck-ford-right-texture",    file = "../assets/images/truck-ford-right.png" },
        { type = "texture", id = "truck-ford-down-texture",     file = "../assets/images/truck-ford-down.png" },
        { type = "texture", id = "truck-ford-left-texture",     file = "../assets/images/truck-ford-left.png" },
        { type = "texture", id = "truck-ford-killed-texture",   file = "../assets/images/truck-ford-killed.png" },
        { type = "texture", id = "army-walk-up-texture",        file = "../assets/images/army-walk-up.png" },
        { type = "texture", id = "army-walk-right-texture",     file = "../assets/images/army-walk-right.png" },
        { type = "texture", id = "army-walk-down-texture",      file = "../assets/images/army-walk-down.png" },
        { type = "texture", id = "army-walk-left-texture",      file = "../assets/images/army-walk-left.png" },
        { type = "texture", id = "army-walk-killed-texture",    file = "../assets/images/army-walk-killed.png" },
        { type = "texture", id = "army-gun-up-texture",         file = "../assets/images/army-gun-up.png" },
        { type = "texture", id = "army-gun-right-texture",      file = "../assets/images/army-gun-right.png" },
        { type = "texture", id = "army-gun-down-texture",       file = "../assets/images/army-gun-down.png" },
        { type = "texture", id = "army-gun-left-texture",       file = "../assets/images/army-gun-left.png" },
        { type = "texture", id = "army-gun-killed-texture",     file = "../assets/images/army-gun-killed.png" },
        { type = "texture", id = "sam-truck-right-texture",     file = "../assets/images/sam-truck-right.png" },
        { type = "texture", id = "sam-tank-left-texture",       file = "../assets/images/sam-tank-left-spritesheet.png" },
        { type = "texture", id = "sam-tank-right-texture",      file = "../assets/images/sam-tank-right-spritesheet.png" },
        { type = "texture", id = "takeoff-base-texture",        file = "../assets/images/takeoff-base.png" },
        { type = "texture", id = "landing-base-texture",        file = "../assets/images/landing-base.png" },
        { type = "texture", id = "runway-texture",              file = "../assets/images/runway.png" },
        { type = "texture", id = "obstacles1-texture",          file = "../assets/images/obstacles-1.png" },
        { type = "texture", id = "obstacles2-texture",          file = "../assets/images/obstacles-2.png" },
        { type = "texture", id = "obstacles3-texture",          file = "../assets/images/obstacles-3.png" },
        { type = "texture", id = "obstacles4-texture",          file = "../assets/images/obstacles-4.png" },
        { type = "texture", id = "obstacles5-texture",          file = "../assets/images/obstacles-5.png" },
        { type = "texture", id = "obstacles6-texture",          file = "../assets/images/obstacles-6.png" },
        { type = "texture", id = "obstacles7-texture",          file = "../assets/images/obstacles-7.png" },
        { type = "texture", id = "tree1-texture",               file = "../assets/images/tree-1.png" },
        { type = "texture", id = "tree2-texture",               file = "../assets/images/tree-2.png" },
        { type = "texture", id = "tree3-texture",               file = "../assets/images/tree-3.png" },
        { type = "texture", id = "tree4-texture",               file = "../assets/images/tree-4.png" },
        { type = "texture", id = "tree5-texture",               file = "../assets/images/tree-5.png" },
        { type = "texture", id = "tree6-texture",               file = "../assets/images/tree-6.png" },
        { type = "texture", id = "tree7-texture",               file = "../assets/images/tree-7.png" },
        { type = "texture", id = "tree8-texture",               file = "../assets/images/tree-8.png" },
        { type = "texture", id = "tree9-texture",               file = "../assets/images/tree-9.png" },
        { type = "texture", id = "tree10-texture",              file = "../assets/images/tree-10.png" },
        { type = "texture", id = "tree11-texture",              file = "../assets/images/tree-11.png" },
        { type = "texture", id = "tree12-texture",              file = "../assets/images/tree-12.png" },
        { type = "texture", id = "tree13-texture",              file = "../assets/images/tree-13.png" },
        { type = "texture", id = "tree14-texture",              file = "../assets/images/tree-14.png" },
        { type = "texture", id = "tree15-texture",              file = "../assets/images/tree-15.png" },
        { type = "texture", id = "tree16-texture",              file = "../assets/images/tree-16.png" },
        { type = "texture", id = "tree17-texture",              file = "../assets/images/tree-17.png" },
        { type = "texture", id = "tree18-texture",              file = "../assets/images/tree-18.png" },
        { type = "texture", id = "tree19-texture",              file = "../assets/images/tree-19.png" },
        { type = "texture", id = "tree20-texture",              file = "../assets/images/tree-20.png" },
        { type = "texture", id = "bullet-texture",              file = "../assets/images/bullet.png" },
        { type = "texture", id = "radar-texture",               file = "../assets/images/radar-spritesheet.png" },
        { type = "font",    id = "pico-font-5",                 file = "../assets/fonts/pico8.ttf",                      font_size = 5 },
        { type = "font",    id = "pico-font-10",                file = "../assets/fonts/pico8.ttf",                      font_size = 10 }
    },

    ------------------------------------------------
    -- Table to define the map config variables
    ------------------------------------------------
    tilemap =
    {
        map_file = "../assets/tilemaps/jungle.map",
        texture_asset_id = map_texture_asset_id,
        num_rows = 22,
        num_cols = 26,
        tile_size = 32,
        tile_set_cols = 10,
        map_Scale = 1.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    entities = {
        [0] =
        {
            -- Player
            tag = "player",
            components = {
                transform = {
                    position = { x = 130, y = 600 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "chopper-texture",
                    width = 32,
                    height = 32,
                    z_index = 4,
                    fixed = false,
                    src_rect_x = 0,
                    src_rect_y = 0
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 12 -- fps
                },
                box_collider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 200 },
                    repeat_frequency = 0,     -- seconds
                    projectile_duration = 10, -- seconds
                    hit_percentage_damage = 10,
                    friendly = true
                },
                keyboard_controller = {
                    speed = 100
                },
                camera_follow = {
                    follow = true
                }
            }
        },
        {
            -- Takeoff base
            components = {
                transform = {
                    position = { x = 126, y = 598 },
                    scale = { x = 1.2, y = 1.2 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "landing-base-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                }
            }
        },
        {
            -- Radar UI animation
            group = "ui",
            components = {
                transform = {
                    position = { x = 10, y = 10 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "radar-texture",
                    width = 64,
                    height = 64,
                    z_index = 10,
                    fixed = true
                },
                animation = {
                    num_frames = 8,
                    speed_rate = 7 -- fps
                }
            }
        },
        {
            -- Runway
            components = {
                transform = {
                    position = { x = 230, y = 115 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 90.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "runway-texture",
                    width = 21,
                    height = 191,
                    z_index = 1
                }
            }
        },
        {
            -- Runway
            components = {
                transform = {
                    position = { x = 490, y = 420 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "runway-texture",
                    width = 21,
                    height = 191,
                    z_index = 1
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 200, y = 497 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                box_collider = {
                    width = 32,
                    height = 18,
                    offset = { x = 3, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -100, y = 0 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false
                }

            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 335, y = 320 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 18,
                    offset = { x = 7, y = 10 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -150 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 1, -- seconds
                    hit_percentage_damage = 15,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 620, y = 200 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 20,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 20 },
                    repeat_frequency = 3,    -- seconds
                    projectile_duration = 5, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 545, y = 320 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -50, y = 0 },
                    repeat_frequency = 3,    -- seconds
                    projectile_duration = 3, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 577, y = 511 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 60, y = 0 },
                    repeat_frequency = 4,    -- seconds
                    projectile_duration = 4, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 570, y = 600 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -60, y = 0 },
                    repeat_frequency = 4,    -- seconds
                    projectile_duration = 4, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 600, y = 170 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -60, y = 0 },
                    repeat_frequency = 4,    -- seconds
                    projectile_duration = 4, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 670, y = 200 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 18,
                    offset = { x = 8, y = 6 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 40 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 80, y = 230 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 18,
                    offset = { x = 8, y = 6 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 40 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 225, y = 320 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 380, y = 435 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 20,
                    height = 17,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -40, y = 0 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 390, y = 320 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 18,
                    height = 20,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 100 },
                    repeat_frequency = 5,    -- seconds
                    projectile_duration = 5, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 255, y = 545 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -60, y = 0 },
                    repeat_frequency = 1,     -- seconds
                    projectile_duration = 10, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 50, y = 430 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 60, y = 0 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 130, y = 380 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 8, y = 4 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 100 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 260, y = 510 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 300, y = 510 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 250, y = 290 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 450, y = 610 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 530, y = 420 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 70, y = 120 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 22,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 0 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 610, y = 130 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 25,
                    height = 18,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = -200, y = 0 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 650, y = 300 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 19,
                    height = 20,
                    offset = { x = 6, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 300 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 700, y = 580 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-tiger-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 18,
                    height = 25,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -100 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 150, y = 70 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 17,
                    height = 20,
                    offset = { x = 8, y = 4 }
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 300 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 410, y = 540 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tank-panther-killed-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 90, y = 565 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 200, y = 360 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 300, y = 85 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "truck-ford-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 210, y = 465 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "truck-ford-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 620, y = 410 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "truck-ford-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 400, y = 490 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "truck-ford-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
                box_collider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 600, y = 380 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -50, y = -50 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 5, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 440, y = 410 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -50, y = -50 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 4, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 70, y = 200 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-tank-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 100, y = -75 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 535, y = 280 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-tank-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -100, y = -60 },
                    repeat_frequency = 2,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 170, y = 250 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -50 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 3, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 270, y = 590 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -50 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 80, y = 450 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "sam-truck-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
                box_collider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -70 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 125, y = 508 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree5-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 117, y = 525 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree5-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 318, y = 465 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree6-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 450, y = 500 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree14-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 130, y = 338 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree17-texture",
                    width = 17,
                    height = 20,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 434, y = 438 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree17-texture",
                    width = 17,
                    height = 20,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 300, y = 330 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree18-texture",
                    width = 17,
                    height = 20,
                    z_index = 2
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 700, y = 440 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree10-texture",
                    width = 31,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 350, y = 475 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree10-texture",
                    width = 31,
                    height = 32,
                    z_index = 2
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 300, y = 150 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "tree14-texture",
                    width = 32,
                    height = 32,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 669, y = 549 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 465, y = 549 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 315, y = 505 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 3
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 360, y = 507 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 138, y = 260 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 280, y = 130 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 631, y = 316 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 670, y = 150 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 675, y = 505 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 465, y = 470 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 190, y = 330 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 620, y = 340 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "army-walk-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 200, y = 280 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "army-walk-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 90, y = 70 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "army-gun-right-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 400, y = 600 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "army-walk-up-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 114, y = 480 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "army-walk-down-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 730, y = 470 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "army-walk-left-texture",
                    width = 32,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 60, y = 550 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 460, y = 220 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 345, y = 223 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = 270.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 540, y = 490 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Carrier
            components = {
                transform = {
                    position = { x = 0, y = 200 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "carrier-texture",
                    width = 59,
                    height = 191,
                    z_index = 1
                },
            }
        },
        {
            -- Carrier
            components = {
                transform = {
                    position = { x = 470, y = 50 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 180.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "carrier-texture",
                    width = 59,
                    height = 191,
                    z_index = 1
                },
            }
        },
        {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 480, y = 200 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 180.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 4
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                box_collider = {
                    width = 20,
                    height = 25,
                    offset = { x = 5, y = 5 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 18, y = 200 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 4
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                box_collider = {
                    width = 20,
                    height = 25,
                    offset = { x = 5, y = 5 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- SU-27 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 485, y = 60 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = -190.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 5.5, y = 35.0 }
                },
                sprite = {
                    texture_asset_id = "su27-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                box_collider = {
                    width = 25,
                    height = 30,
                    offset = { x = 5, y = 0 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Bomber plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 485, y = 570 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 0.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "bomber-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                box_collider = {
                    width = 32,
                    height = 32,
                    offset = { x = 0, y = 0 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- FW190 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 700, y = 500 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = -90.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = -50.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "fw190-texture",
                    width = 32,
                    height = 32,
                    z_index = 6
                },
                animation = {
                    num_frames = 3,
                    speed_rate = 15 -- fps
                },
                box_collider = {
                    width = 32,
                    height = 30,
                    offset = { x = 0, y = 0 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- UPF7 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 145, y = 195 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = 90.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "upf7-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                }
            }
        },
        {
            -- UPF7 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 550, y = 220 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = -90.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "upf7-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                }
            }
        },
        {
            -- UPF7 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 400, y = 179 },
                    scale = { x = 0.8, y = 0.8 },
                    rotation = 180.0, -- degrees
                },
                sprite = {
                    texture_asset_id = "upf7-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                }
            }
        },

        {
            -- SU-27 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 10, y = 650 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 90.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 300.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "su27-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                box_collider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                damage_animation = {
                    num_blinks = 3,
                    blink_on_duration = 80,
                    blink_off_duration = 40
                },
                projectile_emitter = {
                    projectile_velocity = { x = 400, y = 0 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 5, -- seconds
                    hit_percentage_damage = 40,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                        function(entity, delta_time, ellapsed_time)
                            -- this function makes the fighter jet move left and right across the map shooting projectiles
                            local current_position_x, current_position_y = get_position(entity)
                            local current_velocity_x, current_velocity_y = get_velocity(entity)
                            local new_velocity_x = current_velocity_x

                            -- if it reaches the left or the right of the map
                            if current_position_x < 10 or current_position_x > MAP_WIDTH - 32 then
                                new_velocity_x = current_velocity_x * -1 -- flip the entity x-velocity
                            end
                            set_velocity(entity, new_velocity_x, 0)

                            -- set the transform rotation and projectile direction to match going left or right
                            if (new_velocity_x > 0) then
                                set_rotation(entity, 90)                 -- point right
                                set_projectile_velocity(entity, 500, 0)  -- shoot projectiles right
                            else
                                set_rotation(entity, -90)                -- point left
                                set_projectile_velocity(entity, -500, 0) -- shoot projectiles left
                            end
                        end
                }
            }
        },
        {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 10, y = 10 },
                    scale = { x = 1.0, y = 1.0 },
                    rotation = 90.0, -- degrees
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    texture_asset_id = "f22-texture",
                    width = 32,
                    height = 32,
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                box_collider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 0 },
                    repeat_frequency = 1,    -- seconds
                    projectile_duration = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                        function(entity, delta_time, ellapsed_time)
                            -- print("Executing F-22 Lua script!")

                            -- change the position of the the airplane to follow a sine wave movement
                            local new_x = ellapsed_time * 0.09
                            local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
                            set_position(entity, new_x, new_y) -- set the new position
                        end
                }
            }
        }

    }
}

MAP_WIDTH = Level.tilemap.num_cols * Level.tilemap.tile_size * Level.tilemap.map_Scale
MAP_HEIGHT = Level.tilemap.num_rows * Level.tilemap.tile_size * Level.tilemap.map_Scale
