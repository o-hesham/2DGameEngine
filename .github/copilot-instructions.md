# 2D Game Engine - AI Coding Agent Instructions

> ⚠️ **STABILITY NOTICE**: This is a **complete and stable** 2D game engine. The codebase is fully functional and should be treated as **FROZEN** for core engine functionality. Any AI-assisted modifications must follow the strict guidelines below.

---

## 🚨 CRITICAL: Change Policy

**This engine is COMPLETE. Do NOT:**

- Refactor, redesign, or restructure existing engine architecture
- Change ECS implementation, event system, or core game loop
- Modify existing system update order or component signatures
- Add new dependencies or upgrade existing ones without explicit user request
- "Improve" or "optimize" working code without specific bug reports

**AI suggestions MUST:**

- Prioritize **minimal, safe, non-breaking changes** only
- Preserve all existing behavior exactly as implemented
- Document any assumptions explicitly in code comments
- Prefer adding new code alongside existing code rather than modifying it
- Only fix bugs that are explicitly reported with reproduction steps

**When in doubt, DO NOT CHANGE existing code.**

---

## Engine Status

| Feature              | Status      |
| -------------------- | ----------- |
| ECS Framework        | ✅ Complete |
| Rendering System     | ✅ Complete |
| Animation System     | ✅ Complete |
| Collision Detection  | ✅ Complete |
| Keyboard Input       | ✅ Complete |
| Camera System        | ✅ Complete |
| Projectile System    | ✅ Complete |
| Health/Damage System | ✅ Complete |
| Lua Scripting        | ✅ Complete |
| ImGui Debug GUI      | ✅ Complete |
| TMX/Tilemap Loading  | ✅ Complete |
| Text Rendering       | ✅ Complete |
| Asset Management     | ✅ Complete |
| Event Bus            | ✅ Complete |

---

## Architecture Overview

This is a **custom Entity Component System (ECS)** game engine built with C++23, SDL2, and modern C++ features. The architecture follows a strict data-oriented design pattern where:

- **Entities** are simple IDs that aggregate components
- **Components** are plain data structs (no logic) - see [include/Components](include/Components)
- **Systems** contain all game logic and operate on entities with specific component signatures
- **EventBus** handles decoupled communication between systems using type-safe event dispatching

### Core Architecture Files (DO NOT MODIFY)

- [include/ECS/ECS.h](include/ECS/ECS.h): Complete ECS implementation with `Registry`, `System`, `Entity`, and component pooling
- [include/EventBus/EventBus.h](include/EventBus/EventBus.h): Type-safe event system using `std::invoke` and member function pointers
- [src/Game.cpp](src/Game.cpp): Main game loop, manages registry, asset store, event bus, and camera

## Critical Patterns (Reference Only)

### ECS Component Registration

Components are POD structs for unique ID generation:

```cpp
struct MyComponent {
    glm::vec2 position;
    float value;
};
```

### System Creation Pattern

Systems require components and process matching entities in their `Update()` method:

```cpp
class MySystem : public System {
public:
    MySystem() {
        RequireComponent<TransformComponent>();
        RequireComponent<MyComponent>();
    }

    void Update(/* system-specific params */) {
        for (auto& entity : GetSystemEntities()) {
            auto& transform = entity.GetComponent<TransformComponent>();
            // Process entity...
        }
    }
};
```

### Entity Creation & Component Addition

Entities are created through the registry and components are added fluently:

```cpp
Entity player = registry->CreateEntity();
player.Tag("player");  // Optional tag for retrieval
player.Group("enemies");  // Optional group membership
player.AddComponent<TransformComponent>(glm::vec2(x, y), glm::vec2(scale), rotation);
player.AddComponent<SpriteComponent>("texture-id", width, height, isFixed, zIndex);
```

### Event System Usage

Systems subscribe to events in their `SubscribeToEvents()` method:

```cpp
eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
```

Events are emitted with:

```cpp
eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
```

### Game Loop Structure (in Game.cpp)

1. **Setup**: Register all systems in `LoadLevel()`, add assets to `AssetStore`
2. **Update Loop**:
   - `eventBus->Reset()` - Clear previous frame's event handlers
   - Systems subscribe to events (call `SubscribeToEvents()`)
   - `registry->Update()` - Process entity creation/deletion queues
   - Update all systems in dependency order
3. **Render**: Call `RenderSystem::Update()` which sorts by z-index

## Build System

### Using Tasks.json (Recommended)

Execute the default build task: **"Build SDL2 Project"** via VS Code's build command (Ctrl+Shift+B).

### CMake Build (Cross-Platform)

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug   # or Release
cmake --build .
```

### Makefile Build

- **Debug build**: `make debug` (default, adds `-g -DDEBUG`)
- **Release build**: `make release` (adds `-O2 -DNDEBUG`)
- **Run**: `make run` (builds and executes)
- **Clean**: `make clean`

The Makefile supports Windows (MinGW), Linux, and macOS with platform-specific SDL2 linking.

## Dependencies & Vendor Libraries

All third-party libraries are vendored in `vendor/`:

- **SDL2**: Graphics, input, audio (linked from `lib/`)
- **GLM**: Math library for vectors/matrices (`#include <glm.hpp>`)
- **ImGui**: Immediate mode GUI with SDL2 backend
- **Lua + Sol2**: Scripting (Lua 5.3, Sol2 header-only bindings)
- **pugixml**: XML parsing for tilemap loading (TMX format)

**⚠️ NEVER modify vendor code** - treat as read-only dependencies.

## Asset Management

Assets are loaded via `AssetStore` in `LoadLevel()`:

```cpp
assetStore->AddTexture(renderer.get(), "asset-id", "../assets/images/sprite.png");
assetStore->AddFont("font-id", "../assets/fonts/font.ttf", fontSize);
```

Asset paths are **relative to executable location** (`bin/`), so use `../assets/` prefix.

### Asset Directory Structure

- `assets/images/`: Sprite sheets and textures
- `assets/tilemaps/`: `.map` files (CSV tile indices), `.tmx`, and `.png` tilesets
- `assets/fonts/`: TTF fonts
- `assets/scripts/`: Lua level scripts
- `assets/sounds/`: Audio files (SDL_mixer supported)

## Acceptable Modifications

The following are the ONLY types of changes that should be made:

### ✅ Allowed

- Adding new Lua level scripts in `assets/scripts/`
- Adding new assets (images, fonts, sounds) in `assets/`
- Adding new Components following existing patterns (new header in `include/Components/`)
- Adding new Systems following existing patterns (new header in `include/Systems/`)
- Adding new Events following existing patterns (new header in `include/Events/`)
- Bug fixes with explicit reproduction steps
- Documentation updates

### ❌ Not Allowed Without Explicit User Request

- Modifying core ECS implementation
- Changing Game.cpp game loop
- Altering system update order
- Refactoring existing systems
- Upgrading dependencies
- "Improving" or "optimizing" working code

## Code Style Conventions

- **C++23 features**: Use ranges, views, `std::invoke`, structured bindings
- **Smart pointers**: `std::unique_ptr` for ownership, raw pointers for non-owning refs
- **Custom deleters**: SDL types use custom deleters (see `Game.h`)
- **No exceptions**: SDL and game code use return codes and logging
- **Logging**: Use `Logger::Log()`, `Logger::Err()` (see [src/Logger.cpp](src/Logger.cpp))
- **Naming**: PascalCase for types/functions, camelCase for variables

## Debugging

- Build with `make debug` for debug symbols and assertions
- Enable debug rendering: Press `P` in-game to toggle debug mode (visualizes colliders)
- Check console output for Logger messages
- ImGui debug GUI appears in debug mode

## Known Patterns & Gotchas

1. **Component Pools**: Components are stored in contiguous memory pools per type (cache-friendly)
2. **Entity Lifecycle**: Entities marked for deletion via `Kill()` are removed in `registry->Update()`
3. **Camera System**: Camera is an `SDL_Rect` in `Game`, moved by `CameraMovementSystem` following `CameraFollowComponent` entities
4. **Z-Index Sorting**: `RenderSystem` stable-sorts entities by `SpriteComponent::zIndex` before rendering
5. **Fixed Sprites**: `SpriteComponent::isFixed` flag renders UI elements in screen space (ignore camera)
6. **Delta Time**: Movement systems multiply by `deltaTime` (seconds) for frame-rate independence
7. **SDL Resource Management**: Always use unique_ptr with custom deleters for SDL types
