#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>

DWORD GetProcessID(const wchar_t* exeName);
HANDLE OpenGameProcess(DWORD pid, const wchar_t* exeName);
HWND FindGameWindow(DWORD pid);
bool InitGame();
