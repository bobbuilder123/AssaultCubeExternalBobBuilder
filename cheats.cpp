#include "cheats.h"
#include "globals.h"
#include "memory.h"
#include "vec.h"


bool WorldToScreen(const Vec3& world, Vec2& screen, float* m, int width, int height) {
    // Matrix is column-major: index as [row + col * 4]
    float clipX = world.x * m[0] + world.y * m[4] + world.z * m[8] + m[12];
    float clipY = world.x * m[1] + world.y * m[5] + world.z * m[9] + m[13];
    float clipW = world.x * m[3] + world.y * m[7] + world.z * m[11] + m[15];

    if (clipW < 0.001f) return false;

    float ndcX = clipX / clipW;
    float ndcY = clipY / clipW;

    screen.x = (width / 2.0f) + (ndcX * width / 2.0f);
    screen.y = (height / 2.0f) - (ndcY * height / 2.0f);

    return true;
}

Player GetLocalPlayer() {
    Player localPlayer(OFFSET_LOCALPLAYER, true);
    if (!localPlayer.IsValid()) return Player();  // Return invalid player

    if (settings.infiniteAmmo) {
        localPlayer.SetAmmo(999);
        localPlayer.SetMag(999);
    }
    if (settings.godmode) {
        localPlayer.SetHealth(100);
        localPlayer.SetShield(100);
    }
    return localPlayer;
}

std::vector<Player> BuildPlayerList(const Player& localPlayer) {
    float* viewMatrix = RPM<float*>(OFFSET_VIEWMATRIX_BASE);
    std::vector<Player> players;

    int outTotalPlayers = RPM<int>(OFFSET_NUMPLAYERS);
    uintptr_t playerListPtr = static_cast<uintptr_t>(RPM<uint32_t>(OFFSET_PLAYERLIST));
    if (playerListPtr == 0 || outTotalPlayers <= 0 || outTotalPlayers > MAX_PLAYERS)
        return players;

    int currentFrame = RPM<int>(OFFSET_CURRENT_FRAME);

    for (int i = 0; i < MAX_PLAYERS; ++i) {
        uintptr_t entryAddress = playerListPtr + i * sizeof(uint32_t);
        uintptr_t playerPtr = static_cast<uintptr_t>(RPM<uint32_t>(entryAddress));
        if (!playerPtr) continue;

        Player p(playerPtr);
        if (!p.IsValid() || !p.IsAlive()) continue;

        if (!WorldToScreen(p.HeadPos(), p.screenHead, viewMatrix, screenWidth, screenHeight)) { continue; }
        if (!WorldToScreen(p.FeetPos(), p.screenFeet, viewMatrix, screenWidth, screenHeight)) { continue; }

        p.isEnemy = p.IsEnemy(localPlayer);
        p.isVisible = p.IsVisible(currentFrame);
        p.distance = p.DistanceTo(localPlayer);
        p.ComputeBoxDimensions();
        p.distanceToAim = Vec2{ (float)screenWidth / 2, (float)screenHeight / 2 }.DistanceTo(p.screenHead);

        players.push_back(std::move(p));
        if ((int)players.size() >= outTotalPlayers - 1)
            break;
    }

    return players;
}

void DrawFOV() {
    if (!ImGui::GetCurrentContext()) return;
    auto* drawList = ImGui::GetBackgroundDrawList();
    if (!drawList) return;
    if (settings.showFovCircle) {
        ImVec2 center = ImVec2(screenWidth / 2.f, screenHeight / 2.f);
        drawList->AddCircle(center, settings.fovSize, IM_COL32(255, 255, 0, 255), 64, 2.0f);
    }
}

void DrawESP(const std::vector<Player>& players) {
    if (!ImGui::GetCurrentContext()) return;

    ImDrawList* drawList = ImGui::GetBackgroundDrawList(); // use BackgroundDrawList to avoid z-order issues
    if (!drawList) return;

    for (const auto& player : players) {
        if (settings.showEnemies && player.isEnemy) {
            player.DrawBox(drawList, 3.0f);
            player.DrawHealthBar(drawList);
            player.DrawNameAndDistance(drawList);
            if (settings.showSnaplines)
                player.DrawSnapline(drawList);
        }

        if (settings.showFriendlies && !player.isEnemy) {
            player.DrawBox(drawList, 3.0f);
            player.DrawHealthBar(drawList);
            player.DrawNameAndDistance(drawList);
            if (settings.showSnaplines)
                player.DrawSnapline(drawList);
        }
    }
}

void Aimbot(const std::vector<Player>& players, const Player& localPlayer) {
    if (!settings.enableAimbot) return;
    if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000)) return;

    const Player* closestPlayer = nullptr;

    for (const auto& player : players) {
        if (!player.isEnemy || !player.isVisible) continue;
        if (player.distanceToAim > settings.fovSize) continue;
        if (!closestPlayer || player.distanceToAim < closestPlayer->distanceToAim)
            closestPlayer = &player;
    }

    if (closestPlayer) {
        Vec2 aimAngles = CalcAimAngles(localPlayer.HeadPos(), closestPlayer->HeadPos());
        WPM<float>(localPlayer.address + 0x34, aimAngles.y);      // yaw
        WPM<float>(localPlayer.address + 0x38, aimAngles.x);      // pitch
    }

}

void UpdateNoRecoilPatch() {
    if (settings.noRecoil && !noRecoilCurrentlyPatched) {
        BYTE nopPatch[5] = { 0x90, 0x90, 0x90, 0x90, 0x90 };
        WPM(OFFSET_RECOIL, nopPatch, sizeof(nopPatch));
        noRecoilCurrentlyPatched = true;
    }
    else if (!settings.noRecoil && noRecoilCurrentlyPatched) {
        BYTE originalRecoilBytes[5] = { 0xF3, 0x0F, 0x11, 0x56, 0x38 };
        WPM(OFFSET_RECOIL, originalRecoilBytes, sizeof(originalRecoilBytes));
        noRecoilCurrentlyPatched = false;
    }
}
