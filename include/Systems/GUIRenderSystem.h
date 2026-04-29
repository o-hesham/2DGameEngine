#pragma once

#include "ECS/ECS.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

class GUIRenderSystem : public System
{
public:
    GUIRenderSystem() = default;

    void Update(SDL_Renderer *renderer, std::unique_ptr<Registry> &registry, const SDL_Rect &camera, const std::unique_ptr<AssetStore> &assetStore)
    {
        // -----------------------------
        // ImGui frame start
        // -----------------------------
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // =============================
        // Enemy Spawner Window
        // =============================
        if (ImGui::Begin("Spawn Enemy"))
        {
            // -----------------------------
            // Persistent UI state
            // -----------------------------
            static int posX = 0;
            static int posY = 0;

            static float scaleX = 1;
            static float scaleY = 1;
            static float rotation = 0.0f;

            static int velX = 0;
            static int velY = 0;

            // Box collider
            static int colliderW = 32;
            static int colliderH = 32;
            static int colliderOffsetX = 0;
            static int colliderOffsetY = 0;

            // Projectile emitter
            static bool enableProjectile = true;
            static float projAngle = 0.0f;
            static float projSpeed = 100.0f;
            static int projRepeat = 3;
            static int projDuration = 3;
            static int projDamage = 10;

            static int health = 100;

            const char *sprites[] = {
                "tank-panther-right-texture",
                "tank-panther-left-texture",
                "tank-panther-up-texture",
                "tank-panther-down-texture",
                "tank-tiger-right-texture",
                "tank-tiger-left-texture",
                "tank-tiger-up-texture",
                "tank-tiger-down-texture",
                "truck-ford-right-texture",
                "truck-ford-left-texture",
                "truck-ford-up-texture",
                "truck-ford-down-texture"};
            static int selectedSprite = 0;

            // ------------------------------------
            // Create preview render target
            // ------------------------------------
            static SDL_Texture *previewTexture = nullptr;
            static SDL_Renderer *cachedRenderer = nullptr;
            const int PREVIEW_SIZE = 128;

            if (!previewTexture || cachedRenderer != renderer)
            {
                if (previewTexture)
                {
                    SDL_DestroyTexture(previewTexture);
                }
                previewTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, PREVIEW_SIZE, PREVIEW_SIZE);

                SDL_SetTextureBlendMode(previewTexture, SDL_BLENDMODE_BLEND);
                cachedRenderer = renderer;
            }

            // ------------------------------------
            // Render preview enemy
            // ------------------------------------

            // Set the render target to the preview texture instead of the screen
            SDL_SetRenderTarget(renderer, previewTexture);

            // Set background color of the preview (dark gray)
            SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);

            // Clear the preview texture with the background color
            SDL_RenderClear(renderer);

            // Get the sprite texture for the currently selected sprite
            SDL_Texture *spriteTex = assetStore->GetTexture(sprites[selectedSprite]);

            // Destination rectangle where the sprite will be drawn
            SDL_Rect dst{};

            if (spriteTex)
            {
                // Query the original width and height of the sprite texture.
                int texW, texH;
                SDL_QueryTexture(spriteTex, nullptr, nullptr, &texW, &texH);

                // Apply scaling to the sprite size
                dst.w = static_cast<int>(texW * scaleX);
                dst.h = static_cast<int>(texH * scaleY);

                // Center the sprite inside the preview texture
                dst.x = (PREVIEW_SIZE - dst.w) / 2;
                dst.y = (PREVIEW_SIZE - dst.h) / 2;

                // Render the sprite
                SDL_RenderCopyEx(renderer, spriteTex, nullptr, &dst, glm::degrees(rotation), nullptr, SDL_FLIP_NONE);

                // ------------------------------------
                // Collider preview rendering
                // ------------------------------------

                // Scale collider dimensions based on sprite scale
                int scaledColliderW = static_cast<int>(colliderW * scaleX);
                int scaledColliderH = static_cast<int>(colliderH * scaleY);

                // Scale collider offset so it stays aligned with the sprite
                int scaledOffsetX = static_cast<int>(colliderOffsetX * scaleX);
                int scaledOffsetY = static_cast<int>(colliderOffsetY * scaleY);

                // Collider rectangle relative to the sprite position
                SDL_Rect colliderPreview{
                    dst.x + scaledOffsetX,
                    dst.y + scaledOffsetY,
                    scaledColliderW,
                    scaledColliderH};

                // Set collider outline color
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 180);

                // Draw collider as an outline rectangle
                SDL_RenderDrawRect(renderer, &colliderPreview);
            }

            // Restore rendering back to the main screen
            SDL_SetRenderTarget(renderer, nullptr);

            // ------------------------------------
            // ImGui preview UI
            // ------------------------------------
            ImGui::SeparatorText("Preview");
            ImGui::BeginChild("PreviewBox", ImVec2(150, 150), true);
            ImGui::Image((ImTextureID)previewTexture, ImVec2(128, 128), ImVec2(0, 0), ImVec2(1, 1));
            ImGui::EndChild();

            // -----------------------------
            // Sprite
            // -----------------------------
            ImGui::SeparatorText("Sprite");
            ImGui::Combo("Texture", &selectedSprite, sprites, IM_ARRAYSIZE(sprites));

            // -----------------------------
            // Transform
            // -----------------------------
            ImGui::SeparatorText("Transform");
            ImGui::InputInt("Position X", &posX);
            ImGui::InputInt("Position Y", &posY);
            ImGui::SliderFloat("Scale X", &scaleX, 0.1f, 10.0f, "%.1f");
            ImGui::SliderFloat("Scale Y", &scaleY, 0.1f, 10.0f, "%.1f");
            ImGui::SliderAngle("Rotation", &rotation, 0.0f, 360.0f);

            // -----------------------------
            // Rigid Body
            // -----------------------------
            ImGui::SeparatorText("Movement");
            ImGui::InputInt("Velocity X", &velX);
            ImGui::InputInt("Velocity Y", &velY);

            // -----------------------------
            // Box Collider
            // -----------------------------
            ImGui::SeparatorText("Box Collider");
            ImGui::InputInt("Width", &colliderW);
            ImGui::InputInt("Height", &colliderH);
            ImGui::InputInt("Offset X", &colliderOffsetX);
            ImGui::InputInt("Offset Y", &colliderOffsetY);

            // -----------------------------
            // Projectile Emitter
            // -----------------------------
            ImGui::SeparatorText("Projectile Emitter");
            ImGui::Checkbox("Enable Projectile", &enableProjectile);

            if (enableProjectile)
            {
                ImGui::SliderAngle("Angle", &projAngle, 0.0f, 360.0f);
                ImGui::SliderFloat("Speed", &projSpeed, 10.0f, 600.0f);
                ImGui::InputInt("Repeat (sec)", &projRepeat);
                ImGui::InputInt("Duration (sec)", &projDuration);
                ImGui::InputInt("Damage", &projDamage);
            }

            // -----------------------------
            // Health
            // -----------------------------
            ImGui::SeparatorText("Health");
            ImGui::SliderInt("%", &health, 1, 100);

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            // -----------------------------
            // Spawn Button
            // -----------------------------
            if (ImGui::Button("Spawn Enemy", ImVec2(ImGui::GetContentRegionAvail().x, 30)))
            {
                Entity enemy = registry->CreateEntity();
                enemy.Group("enemies");
                enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(velX, velY));
                enemy.AddComponent<SpriteComponent>(sprites[selectedSprite], 32, 32, 1);
                enemy.AddComponent<BoxColliderComponent>(colliderW, colliderH, glm::vec2(colliderOffsetX, colliderOffsetY));

                if (enableProjectile)
                {
                    double vx = cos(projAngle) * projSpeed;
                    double vy = sin(projAngle) * projSpeed;

                    enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(vx, vy), projRepeat * 1000, projDuration * 1000, projDamage, false);
                }

                enemy.AddComponent<HealthComponent>(health);
                enemy.AddComponent<DamageAnimationComponent>();

                posX = posY = rotation = projAngle = 0;
                scaleX = scaleY = 1;
                projRepeat = projDuration = 10;
                projSpeed = 100;
                health = 100;
            }
        }
        ImGui::End();

        // =============================
        // Mouse World Coordinates Overlay
        // =============================
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoNav;

        ImGui::SetNextWindowPos(ImVec2(10, 10));
        ImGui::SetNextWindowBgAlpha(0.85f);

        if (ImGui::Begin("Map Coordinates", nullptr, flags))
        {
            ImGui::Text(
                "Map Coordinates (x=%.1f, y=%.1f)",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y);
        }
        ImGui::End();

        // -----------------------------
        // Render ImGui
        // -----------------------------
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
};
