#pragma once
#include "vec.h"
#include <basetsd.h>


struct WeaponEntry {
    DWORD32 vftable;     // 0x00
    int id;            // 0x04
    struct PlayerStruct* owner; // 0x08
};

struct PlayerStruct {
    DWORD32 vftable;           // 0x000
    Vec3 positionHead;         // 0x004
    char pad1[0x028 - 0x010];   // 0x010 → 0x028
    Vec3 positionFeet;         // 0x028
    float yaw;                 // 0x034
    float pitch;               // 0x038
    char pad2[0x0D4 - 0x03C];  // 0x03C → 0x0D4
    int frameD4;               // 0x0D4
    int frameD8;               // 0x0D8
    char padXX[0x0E4 - 0x0DC]; // 0x0DC → 0x0E4
    int lastVisibleFrame;      // 0x0E4
    char padX[0x0EC - 0x0E8];  // 0x0E8 → 0x0EC
    int health;                // 0x0EC
    int armor;                 // 0x0F0
    char pad3[0x11C - 0x0F4];  // 0x0F4 → 0x11C
    int mag;                   // 0x11C
    char pad4[0x140 - 0x120];  // 0x120 → 0x140
    int ammo;                  // 0x140
    char pad5[0x1DC - 0x144];  // 0x144 → 0x1DC
    int frags;                 // 0x1DC
    char pad6[0x1E4 - 0x1E0];  // 0x1E0 → 0x1E4
    int deaths;                // 0x1E4 
    char pad7[0x205 - 0x1E8];  // 0x1E8 → 0x205
    char name[16];             // 0x205
    char pad8[0x30C - (0x205 + 16)];  // 0x215 → 0x30C
    int state;                 // 0x30C
    char pad11[0x33C - 0x310];

    // Weapon pointers
    WeaponEntry* knife;           // 0x33C
    WeaponEntry* pistol;          // 0x340
    WeaponEntry* carbine;         // 0x344
    WeaponEntry* shotgun;         // 0x348
    WeaponEntry* subgun;          // 0x34C
    WeaponEntry* sniper;          // 0x350
    WeaponEntry* assault;         // 0x354
    WeaponEntry* grenade;         // 0x358
    WeaponEntry* akimbo;          // 0x35C
    WeaponEntry* currentWeapon;   // 0x360
    WeaponEntry* lastWeapon;      // 0x364
    WeaponEntry* nextWeapon;      // 0x368
    WeaponEntry* altWeapon;       // 0x36C
    WeaponEntry* swapWeapon;      // 0x370
    WeaponEntry* maybeUnused;     // 0x374
};