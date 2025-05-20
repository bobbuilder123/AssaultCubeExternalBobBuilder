#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <optional>
#include "vec.h"

// === CONSTANTS ===
constexpr uintptr_t OFFSET_LOCALPLAYER = 0x0058AC00;
constexpr uintptr_t OFFSET_NUMPLAYERS = 0x0058AC0C;
constexpr uintptr_t OFFSET_PLAYERLIST = 0x0058AC04;
constexpr uintptr_t OFFSET_CURRENT_FRAME = 0x0057F10C;
constexpr uintptr_t OFFSET_VIEWMATRIX_BASE = 0x0057DFD0;
constexpr uintptr_t OFFSET_RECOIL = 0x004C2EC3;
constexpr uintptr_t OFFSET_GAMEMODE = 0x0058ABF8;
constexpr uintptr_t OFFSET_MAPNAME = 0x0058AE0C;

constexpr int MAX_PLAYERS = 32;
inline const wchar_t* assault_cube_procname = L"ac_client.exe";

// === GLOBAL STATE (externs) ===
extern HWND hwndOverlay;
extern HWND hwndGame;
extern LPDIRECT3D9 pD3D;
extern LPDIRECT3DDEVICE9 pDevice;
extern HANDLE hGame;
extern MSG msg;
extern WNDCLASSEX wc;
extern D3DPRESENT_PARAMETERS d3dpp;

extern int screenWidth;
extern int screenHeight;
extern int lastWidth;
extern int lastHeight;

extern bool noRecoilCurrentlyPatched;
extern bool lastKeyState;

// === SETTINGS ===
struct CheatSettings {
    bool showMenu = false;
    bool showSnaplines = true;
    bool showEnemies = true;
    bool showFriendlies = true;
    bool godmode = true;
    bool infiniteAmmo = true;
    bool enableAimbot = true;
    bool noRecoil = false;
    bool showFovCircle = true;
    float fovSize = 400.0f;
};

extern CheatSettings settings;
