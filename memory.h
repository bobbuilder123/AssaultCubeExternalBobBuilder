#ifndef MEMORY_H
#define MEMORY_H

#include <Windows.h>
#include <optional>

extern HANDLE hGame;

template<typename T>
T RPM(uintptr_t address) {
    T buffer{};
    ReadProcessMemory(hGame, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(T), nullptr);
    return buffer;
}

template<typename T>
bool RPM(uintptr_t address, T& outBuffer) {
    SIZE_T bytesRead = 0;
    return ReadProcessMemory(hGame, reinterpret_cast<LPCVOID>(address), &outBuffer, sizeof(T), &bytesRead)
        && bytesRead == sizeof(T);
}

template<>
inline float* RPM<float*>(uintptr_t address) {
    static float buffer[16]{};
    ReadProcessMemory(hGame, reinterpret_cast<LPCVOID>(address), &buffer, sizeof(buffer), nullptr);
    return buffer;
}

template<typename T>
void RPM(uintptr_t address, T* outBuffer, size_t count) {
    ReadProcessMemory(hGame, reinterpret_cast<LPCVOID>(address), outBuffer, sizeof(T) * count, nullptr);
}

template<typename T>
void WPM(uintptr_t address, const T& value) {
    WriteProcessMemory(hGame, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
}

inline void WPM(uintptr_t address, const BYTE* buffer, size_t size) {
    WriteProcessMemory(hGame, reinterpret_cast<LPVOID>(address), buffer, size, nullptr);
}

#endif // MEMORY_H
