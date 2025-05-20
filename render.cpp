#include "render.h"
#include "globals.h"
#include "memory.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CreateOverlayWindow() {
    wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        L"OverlayClass",
        nullptr
    };

    RegisterClassEx(&wc);
    HWND parentWindow = CreateWindowEx(0, L"STATIC", nullptr, WS_DISABLED,
        0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);

    hwndOverlay = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED,
        L"OverlayClass", L"Overlay",
        WS_POPUP, 100, 100, 800, 600,
        parentWindow, nullptr, wc.hInstance, nullptr
    );

    SetLayeredWindowAttributes(hwndOverlay, RGB(0, 0, 0), 255, LWA_COLORKEY);
    ShowWindow(hwndOverlay, SW_SHOW);
    return true;
}

bool CreateDevice() {
    pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (!pD3D) return false;

    d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwndOverlay;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = 800;
    d3dpp.BackBufferHeight = 600;

    return SUCCEEDED(pD3D->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwndOverlay,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice));
}

ImGuiIO& InitOverlay() {
    CreateOverlayWindow();
    CreateDevice();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwndOverlay);
    ImGui_ImplDX9_Init(pDevice);

    return io;
}

void BeginImGuiFrame(ImGuiIO& io) {
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    ImGui::NewFrame();
}

void EndImGuiFrame() {
    ImGui::Render();
    pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    if (pDevice->BeginScene() >= 0) {
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        pDevice->EndScene();
    }
    pDevice->Present(NULL, NULL, NULL, NULL);
}

void Cleanup() {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (pDevice) pDevice->Release();
    if (pD3D) pD3D->Release();

    UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void DrawMenu(const Player& localPlayer) {
    if (!ImGui::GetCurrentContext() || !settings.showMenu)
        return;

    ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Cheat Menu by BobBuilder/Raphael", &settings.showMenu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
        int gamemode = RPM<int>(OFFSET_GAMEMODE);
        char mapName[32] = {};
        RPM<char>(OFFSET_MAPNAME, mapName, 32);

        ImGui::Text("Gamemode: %d", gamemode);
        ImGui::Text("Map: %s", mapName);
        ImGui::Text("Yaw: %.1f", localPlayer->yaw);
        ImGui::Text("Pitch: %.1f", localPlayer->pitch);
        ImGui::Separator();

        ImGui::Checkbox("Show Snaplines", &settings.showSnaplines);
        ImGui::Checkbox("Show Enemies", &settings.showEnemies);
        ImGui::Checkbox("Show Friendlies", &settings.showFriendlies);
        ImGui::Checkbox("Godmode", &settings.godmode);
        ImGui::Checkbox("Unlimited Ammo", &settings.infiniteAmmo);
        ImGui::Checkbox("Aimbot Enabled", &settings.enableAimbot);
        ImGui::Checkbox("No Recoil", &settings.noRecoil);
        ImGui::Checkbox("Show FOV Circle", &settings.showFovCircle);

        if (settings.showFovCircle) {
            ImGui::SliderFloat("FOV Radius", &settings.fovSize, 50.0f, 1000.0f, "%.0f px");
        }
    }
    ImGui::End();
}

void UpdateRenderDimensions() {
    RECT clientRect;
    GetClientRect(hwndGame, &clientRect);
    POINT topLeft = { 0, 0 };
    ClientToScreen(hwndGame, &topLeft);

    screenWidth = clientRect.right;
    screenHeight = clientRect.bottom;
    MoveWindow(hwndOverlay, topLeft.x, topLeft.y, screenWidth, screenHeight, TRUE);
}

bool IsGameMinimized() {
    WINDOWPLACEMENT placement = { sizeof(WINDOWPLACEMENT) };
    GetWindowPlacement(hwndGame, &placement);
    return placement.showCmd == SW_SHOWMINIMIZED;
}

void HandleWindowMessages(MSG& msg) {
    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool HandleDeviceResetIfNeeded(int& lastWidth, int& lastHeight) {
    if (screenWidth == lastWidth && screenHeight == lastHeight) return true;
    if (settings.showMenu && ImGui::IsAnyMouseDown()) return true;

    lastWidth = screenWidth;
    lastHeight = screenHeight;

    ImGui_ImplDX9_InvalidateDeviceObjects();

    d3dpp.BackBufferWidth = screenWidth;
    d3dpp.BackBufferHeight = screenHeight;

    HRESULT hr = pDevice->Reset(&d3dpp);
    if (FAILED(hr)) {
        std::cerr << "[ERROR] Device reset failed: " << std::hex << hr << std::endl;
        return false;
    }

    ImGui_ImplDX9_CreateDeviceObjects();
    return true;
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED) return 0;
        screenWidth = (UINT)LOWORD(lParam);
        screenHeight = (UINT)HIWORD(lParam);
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
