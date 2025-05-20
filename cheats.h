#pragma once

#include <vector>
#include "player.h"

// Core features
void DrawESP(const std::vector<Player>& players);
void DrawFOV();
void Aimbot(const std::vector<Player>& players, const Player& localPlayer);
void UpdateNoRecoilPatch();

// Game logic
Player GetLocalPlayer();
std::vector<Player> BuildPlayerList(const Player& localPlayer);
