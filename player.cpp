#include "player.h"
#include "memory.h"

Player::Player(uintptr_t playerPtr) {
    if (playerPtr == 0) return;
    auto temp = std::make_unique<PlayerStruct>();
    if (!RPM(playerPtr, *temp)) return;
    ptr = temp.get();
    internalStruct = std::move(temp);
    address = playerPtr;
}

Player::Player(uintptr_t offset, bool isOffset) {
    if (!isOffset) return;
    uint32_t ptr32 = RPM<uint32_t>(offset);
    uintptr_t playerPtr = static_cast<uintptr_t>(ptr32);
    if (playerPtr == 0) return;
    auto temp = std::make_unique<PlayerStruct>();
    if (!RPM(playerPtr, *temp)) return;
    ptr = temp.get();
    internalStruct = std::move(temp);
    address = playerPtr;
}

Player::Player(Player&& other) noexcept {
    internalStruct = std::move(other.internalStruct);
    ptr = internalStruct.get();
    address = other.address;
    screenHead = other.screenHead;
    screenFeet = other.screenFeet;
    isEnemy = other.isEnemy;
    isVisible = other.isVisible;
    distance = other.distance;
    boxHeight = other.boxHeight;
    boxWidth = other.boxWidth;
    distanceToAim = other.distanceToAim;
}

Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        internalStruct = std::move(other.internalStruct);
        ptr = internalStruct.get();
        address = other.address;
        screenHead = other.screenHead;
        screenFeet = other.screenFeet;
        isEnemy = other.isEnemy;
        isVisible = other.isVisible;
        distance = other.distance;
        boxHeight = other.boxHeight;
        boxWidth = other.boxWidth;
        distanceToAim = other.distanceToAim;
    }
    return *this;
}

PlayerStruct* Player::operator->() const { return ptr; }
PlayerStruct& Player::operator*() const { return *ptr; }

bool Player::IsValid() const { return ptr != nullptr; }
bool Player::IsAlive() const { return ptr && ptr->health > 0; }
int Player::GetTeam() const { return ptr ? (ptr->state & 1) : -1; }
bool Player::IsEnemy(const Player& localPlayer) const { return IsValid() && ((ptr->state & 1) != (localPlayer->state & 1)); }
bool Player::IsVisible(int currentFrame) const { return IsValid() && ptr->lastVisibleFrame >= currentFrame; }

int Player::GetHealth() const { return ptr ? ptr->health : 0; }
int Player::GetArmor() const { return ptr ? ptr->armor : 0; }
const char* Player::GetName() const { return ptr ? ptr->name : ""; }
float Player::GetYaw() const { return ptr ? ptr->yaw : 0.f; }
float Player::GetPitch() const { return ptr ? ptr->pitch : 0.f; }
Vec3 Player::HeadPos() const { return ptr->positionHead + Vec3(0.f, 0.f, 0.75f); }
Vec3 Player::FeetPos() const { return ptr->positionFeet; }
float Player::DistanceTo(const Player& other) const { return this->HeadPos().Distance(other.HeadPos()); }
WeaponEntry* Player::GetWeapon() const { return ptr ? ptr->currentWeapon : nullptr; }

void Player::SetMag(int value) const { if (address) WPM<int>(address + 0x11C, value); }
void Player::SetAmmo(int value) const { if (address) WPM<int>(address + 0x140, value); }
void Player::SetHealth(int value) const { if (address) WPM<int>(address + 0xEC, value); }
void Player::SetShield(int value) const { if (address) WPM<int>(address + 0xF0, value); }

void Player::ComputeBoxDimensions() {
    boxHeight = screenFeet.y - screenHead.y;
    boxWidth = boxHeight * 0.5f;
}

ImVec2 Player::GetBoxTopLeft() const {
    return ImVec2(screenHead.x - boxWidth / 2, screenHead.y);
}

ImVec2 Player::GetBoxBottomRight() const {
    return ImVec2(screenHead.x + boxWidth / 2, screenFeet.y);
}

void Player::DrawBox(ImDrawList* drawList, float rounding, float thickness) const {
    drawList->AddRect(GetBoxTopLeft(), GetBoxBottomRight(), GetBoxColor(), rounding, 0, thickness);
}

ImU32 Player::GetBoxColor() const {
    if (isEnemy) {
        return isVisible ? IM_COL32(255, 0, 0, 255) : IM_COL32(255, 255, 0, 255);
    }
    else {
        return isVisible ? IM_COL32(0, 0, 255, 255) : IM_COL32(0, 255, 255, 255);
    }
}

void Player::DrawHealthBar(ImDrawList* drawList) const {
    float boxHeight = screenFeet.y - screenHead.y;
    float healthPct = clamp(GetHealth() / 100.0f, 0.0f, 1.0f);
    float healthHeight = boxHeight * healthPct;
    ImVec2 topLeft = GetBoxTopLeft();
    ImVec2 bottomRight = GetBoxBottomRight();
    ImVec2 barStart = { topLeft.x - 6, bottomRight.y - healthHeight };
    ImVec2 barEnd = { topLeft.x - 2, bottomRight.y };
    drawList->AddRectFilled(barStart, barEnd, IM_COL32(0, 255, 0, 255));
    drawList->AddRect(ImVec2(topLeft.x - 6, topLeft.y), ImVec2(topLeft.x - 2, bottomRight.y), IM_COL32(255, 255, 255, 100));
}

void Player::DrawTextAboveBox(ImDrawList* drawList, const std::string& text, float yOffset, ImU32 color) const {
    ImVec2 topLeft = GetBoxTopLeft();
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
    ImVec2 pos = ImVec2(topLeft.x - textSize.x / 2.0f, topLeft.y + yOffset);
    drawList->AddText(pos, color, text.c_str());
}

void Player::DrawName(ImDrawList* drawList) const {
    DrawTextAboveBox(drawList, GetName(), -18.0f);
}

void Player::DrawDistance(ImDrawList* drawList, ImU32 color) const {
    char buf[32];
    snprintf(buf, sizeof(buf), "[%.1fm]", distance);
    DrawTextAboveBox(drawList, buf, -6.0f, color);
}

void Player::DrawNameAndDistance(ImDrawList* drawList) const {
    char buf[64];
    snprintf(buf, sizeof(buf), "%s [%.1fm]", GetName(), distance);
    DrawTextAboveBox(drawList, buf, -18.0f);
}

void Player::DrawSnapline(ImDrawList* drawList) const {
    if (isEnemy) {
        ImVec2 bottom = GetBoxBottomRight();
        drawList->AddLine(
            ImVec2((bottom.x + GetBoxTopLeft().x) / 2, bottom.y),
            ImVec2(screenWidth / 2.0f, screenHeight),
            IM_COL32(0, 255, 255, 100),
            1.5f
        );
    }
}
