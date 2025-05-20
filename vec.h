#pragma once
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RAD2DEG (180.0f / 3.14159265f)
#define DEG2RAD (3.14159265358979323846f / 180.0f)


struct Vec2 {
    float x, y;
    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    float DistanceTo(const Vec2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return sqrtf(dx * dx + dy * dy);
    }
};

struct Vec3 {
    float x, y, z;

    // Constructor
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Addition
    Vec3 operator+(const Vec3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    // Subtraction
    Vec3 operator-(const Vec3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    // Scalar division
    Vec3 operator/(float scalar) const {
        return { x / scalar, y / scalar, z / scalar };
    }

    // Scalar multiplication
    Vec3 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    float Distance(const Vec3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};



inline Vec2 CalcAimAngles(const Vec3& from, const Vec3& to) {
    Vec3 delta = to - from;
    float yaw = std::atan2(delta.y, delta.x) * RAD2DEG + 90.0f;
    float pitch = std::asin(delta.z / from.Distance(to)) * RAD2DEG;

    return Vec2(pitch, yaw);
}

template<typename T>
T clamp(T val, T min, T max) {
    return (val < min) ? min : (val > max) ? max : val;
}