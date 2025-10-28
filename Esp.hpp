#pragma once
#include <windows.h>
#include <cmath>

struct Vector3 { float x, y, z; };

// Corner box 
inline void DrawCornerBox(HDC hdc, int x, int y, int w, int h) {
    // Top-left
    MoveToEx(hdc, x, y, NULL); LineTo(hdc, x + w/4, y);
    MoveToEx(hdc, x, y, NULL); LineTo(hdc, x, y + h/4);
    // Top-right
    MoveToEx(hdc, x + w, y, NULL); LineTo(hdc, x + w - w/4, y);
    MoveToEx(hdc, x + w, y, NULL); LineTo(hdc, x + w, y + h/4);
    // Bottom-left
    MoveToEx(hdc, x, y + h, NULL); LineTo(hdc, x + w/4, y + h);
    MoveToEx(hdc, x, y + h, NULL); LineTo(hdc, x, y + h - h/4);
    // Bottom-right
    MoveToEx(hdc, x + w, y + h, NULL); LineTo(hdc, x + w - w/4, y + h);
    MoveToEx(hdc, x + w, y + h, NULL); LineTo(hdc, x + w, y + h - h/4);
}

// Full rectangle box
inline void DrawFullBox(HDC hdc, int x, int y, int w, int h) {
    Rectangle(hdc, x, y, x + w, y + h);
}

// Scaled boxes
inline void DrawScaledCornerBox(HDC hdc, int x, int y, float distance) {
    int size = static_cast<int>(1000.0f / (distance + 1.0f));
    DrawCornerBox(hdc, x - size/2, y - size/2, size, size);
}

inline void DrawScaledFullBox(HDC hdc, int x, int y, float distance) {
    int size = static_cast<int>(1000.0f / (distance + 1.0f));
    DrawFullBox(hdc, x - size/2, y - size/2, size, size);
}

// Skeleton (same as before)
inline void DrawScaledSkeleton(HDC hdc, int sx, int sy, float distance) {
    float scale = 50.0f / (distance + 1.0f);
    MoveToEx(hdc, sx, sy - static_cast<int>(scale), NULL); LineTo(hdc, sx, sy);
    LineTo(hdc, sx, sy + static_cast<int>(2*scale));
    MoveToEx(hdc, sx, sy + static_cast<int>(scale/2), NULL); LineTo(hdc, sx - static_cast<int>(scale), sy + static_cast<int>(scale));
    MoveToEx(hdc, sx, sy + static_cast<int>(scale/2), NULL); LineTo(hdc, sx + static_cast<int>(scale), sy + static_cast<int>(scale));
    MoveToEx(hdc, sx, sy + static_cast<int>(2*scale), NULL); LineTo(hdc, sx - static_cast<int>(scale/2), sy + static_cast<int>(3*scale));
    MoveToEx(hdc, sx, sy + static_cast<int>(2*scale), NULL); LineTo(hdc, sx + static_cast<int>(scale/2), sy + static_cast<int>(3*scale));
}

// Dummy WorldToScreen
inline bool WorldToScreen(const Vector3& pos, Vector3 vm[4], int screenW, int screenH, int& x, int& y) {
    x = static_cast<int>(pos.x);
    y = static_cast<int>(pos.y);
    return true;
}
