#include "globals.h"
#include "vec.h"
#include "memory.h"
#include "player.h"
#include "cheats.h"
#include "render.h"
#include "gameutils.h"
#pragma comment(lib, "d3d9.lib")


int main() {
    if (!InitGame()) {
        std::cerr << "Could not init game." << std::endl;
        return -1;
    }
    ImGuiIO& io = InitOverlay();
    // === MAIN LOOP ===
    while (msg.message != WM_QUIT) {
        HandleWindowMessages(msg);
        UpdateRenderDimensions();
        // === Reset device if resized ===
        if (!HandleDeviceResetIfNeeded(lastWidth, lastHeight))
            break;
        io.DisplaySize = ImVec2((float)screenWidth, (float)screenHeight);
        if (IsGameMinimized()) {
            ShowWindow(hwndOverlay, SW_HIDE);
            continue;
        }
        else {
            ShowWindow(hwndOverlay, SW_SHOW);
        }
        bool keyPressed = (GetAsyncKeyState(VK_INSERT) & 1);
        if (keyPressed && !lastKeyState) {
            settings.showMenu = !settings.showMenu;
        }
        lastKeyState = keyPressed;
        Player localPlayer = GetLocalPlayer();  // includes SetHealth/Ammo
        auto players = BuildPlayerList(localPlayer);
        // === Begin ImGui ===
        BeginImGuiFrame(io);
        DrawMenu(localPlayer);
        UpdateNoRecoilPatch();
        DrawFOV();
        DrawESP(players);
        Aimbot(players, localPlayer);
        EndImGuiFrame();
    }
    Cleanup();
    return 0;
}
