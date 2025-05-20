#pragma once

#include <Windows.h>
#include <d3d9.h>
#include "imgui.h"
#include "player.h"

// === Overlay Lifecycle ===
bool CreateOverlayWindow();
bool CreateDevice();
ImGuiIO& InitOverlay();
void Cleanup();

// === Rendering ===
void BeginImGuiFrame(ImGuiIO& io);
void EndImGuiFrame();
void DrawMenu(const Player& localPlayer);

// === Window + Device Logic ===
void UpdateRenderDimensions();
bool IsGameMinimized();
void HandleWindowMessages(MSG& msg);
bool HandleDeviceResetIfNeeded(int& lastWidth, int& lastHeight);

// === WndProc ===
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
