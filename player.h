#pragma once

#include <memory>
#include <string>
#include <vector>
#include "structs.h"
#include "vec.h"
#include "globals.h"

// Forward declaration
struct PlayerStruct;

class Player {
public:
    std::unique_ptr<PlayerStruct> internalStruct;
    PlayerStruct* ptr = nullptr;
    uintptr_t address = 0;

    Vec2 screenHead = {};
    Vec2 screenFeet = {};

    bool isEnemy = false;
    bool isVisible = false;
    float distance = 0.f;
    float boxHeight = 0.f;
    float boxWidth = 0.f;
    float distanceToAim = 0.f;

    Player() = default;
    Player(uintptr_t playerPtr);
    Player(uintptr_t offset, bool isOffset);

    Player(Player&& other) noexcept;
    Player& operator=(Player&& other) noexcept;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;

    PlayerStruct* operator->() const;
    PlayerStruct& operator*() const;

    bool IsValid() const;
    bool IsAlive() const;
    int GetTeam() const;
    bool IsEnemy(const Player& localPlayer) const;
    bool IsVisible(int currentFrame) const;

    int GetHealth() const;
    int GetArmor() const;
    const char* GetName() const;
    float GetYaw() const;
    float GetPitch() const;
    Vec3 HeadPos() const;
    Vec3 FeetPos() const;
    float DistanceTo(const Player& other) const;
    WeaponEntry* GetWeapon() const;

    void SetMag(int value) const;
    void SetAmmo(int value) const;
    void SetHealth(int value) const;
    void SetShield(int value) const;

    void ComputeBoxDimensions();
    ImVec2 GetBoxTopLeft() const;
    ImVec2 GetBoxBottomRight() const;
    void DrawBox(ImDrawList* drawList, float rounding = 0.0f, float thickness = 1.0f) const;
    ImU32 GetBoxColor() const;
    void DrawHealthBar(ImDrawList* drawList) const;
    void DrawTextAboveBox(ImDrawList* drawList, const std::string& text, float yOffset, ImU32 color = IM_COL32(255, 255, 255, 255)) const;
    void DrawName(ImDrawList* drawList) const;
    void DrawDistance(ImDrawList* drawList, ImU32 color = IM_COL32(255, 255, 255, 255)) const;
    void DrawNameAndDistance(ImDrawList* drawList) const;
    void DrawSnapline(ImDrawList* drawList) const;
};
