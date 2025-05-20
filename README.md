# AssaultCube External ESP + Aimbot

An external cheat for **AssaultCube** featuring ESP, aimbot, no recoil, godmode, and more — built with **ImGui** and **DirectX 9** in C++. Designed for learning and practical reverse engineering.

> Author: [BobBuilder / Raphael](https://adminions.ca/books/the-team/page/bobbuilder)

---

## Project Structure

This repository is cleanly organized by functionality for maintainability and learning purposes:

```

/src
├── main.cpp              # Main entry point and render loop
├── globals.h/.cpp        # Constants, offsets, global settings & handles
├── memory.h              # RPM/WPM memory access wrappers
├── vec.h                 # Vec2/Vec3 structs and math utilities
├── structs.h             # Game structure definitions (PlayerStruct, WeaponEntry)
├── player.h/.cpp         # Player class abstraction with logic & drawing
├── cheats.h/.cpp         # Aimbot, ESP, NoRecoil, player list building
├── render.h/.cpp         # Overlay window, ImGui rendering, device management
├── gameutils.h/.cpp      # Process attach, InitGame, FindWindow helpers

```

---

## Features

- ESP Boxes (Enemies + Friendlies)
- Snaplines
- Health bars
- Name + Distance display
- FOV aimbot (Right Mouse Button)
- Toggleable No Recoil patch
- Godmode / Infinite Ammo
- Overlay Menu via `Insert`
- Full resolution resize & window tracking

---

## Technical Details

- This is an **external cheat**, meaning it:
  - Does not inject into the game process
  - Reads/writes game memory remotely via `ReadProcessMemory` and `WriteProcessMemory`
- Because it’s external, it can be compiled in **x64 or x86** regardless of the target game (which is 32-bit).
  - You can safely compile this as a **x64 .exe**
- Overlay is implemented using **DirectX 9** + **Dear ImGui**
- Game data structures are reverse-engineered and read into local `Player` objects every frame

---

## Requirements
- Windows OS
- Visual Studio (or another compiler with C++17 support)
- `imgui`, `imgui_impl_win32`, `imgui_impl_dx9` (already integrated)

---

## How to Build

1. Open the project in Visual Studio
2. Set the build target to **x64** (or x86 if you prefer)
3. Compile → This will produce a standalone `.exe`

---

## Usage
1. Launch **AssaultCube** (must be the `ac_client.exe`)
2. Run the compiled cheat `.exe` as Administrator
3. Use the `Insert` key to open the menu
4. Hold **Right Mouse Button** to activate aimbot (if enabled)
5. Use checkboxes in the menu to toggle ESP, recoil, etc.

---

## Screenshot & Demo

![ScreenshotDemo](screenshot.png)
![Demo](https://youtu.be/cWaQgsfLquk)

---

## Author

**BobBuilder / Raphael**  
[→ Profile Page](https://adminions.ca/books/the-team/page/bobbuilder)

---

## License - GNU General Public License v3.0

This project is licensed under the **GNU General Public License v3.0**.  
You are free to use, modify, and distribute this software under the same license, provided that the source code and this copyright notice remain.

See the [LICENSE](./LICENSE) file for full terms.

This project also uses components under the **MIT License**, including [Dear ImGui](https://github.com/ocornut/imgui), which are compatible with the GPLv3.


This project is intended for **educational purposes**. Use responsibly.

