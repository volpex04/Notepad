#include <windows.h>
#include <thread>
#include <vector>
#include <iostream>
#include <conio.h>
#include "esp.hpp"
#include "offsets.hpp"

// ------------------- Global Settings -------------------
bool triggerBotActive = false;
bool cornerBoxActive = false;
bool skeletonActive = false;
bool useFullBox = false;
bool programRunning = true;
COLORREF espColor = RGB(255,255,255);

struct Entity { Vector3 pos; int team; int health; float distance; };
std::vector<Entity> entities;

enum MenuState { MAIN_MENU, ESP_COLOR_MENU };
MenuState currentMenu = MAIN_MENU;

// ------------------- ESP Overlay -------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HPEN pen = CreatePen(PS_SOLID,2,espColor);
            HGDIOBJ oldPen = SelectObject(hdc, pen);

            int screenW=1920, screenH=1080;
            Vector3 vm[4];

            for(auto& e: entities) {
                int sx, sy;
                if(WorldToScreen(e.pos, vm, screenW, screenH, sx, sy)) {
                    if(cornerBoxActive) {
                        if(useFullBox)
                            DrawScaledFullBox(hdc, sx, sy, e.distance);
                        else
                            DrawScaledCornerBox(hdc, sx, sy, e.distance);
                    }
                    if(skeletonActive) DrawScaledSkeleton(hdc, sx, sy, e.distance);
                }
            }

            SelectObject(hdc, oldPen);
            DeleteObject(pen);
            EndPaint(hwnd, &ps);
        } break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd,msg,wParam,lParam);
    }
    return 0;
}

// ------------------- ESP Thread -------------------
void ESPLoop() {
    while(programRunning) {
        entities.clear();
        // Dummy entity for testing
        entities.push_back({{500,300,0},1,100,100.f});
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

// ------------------- TriggerBot Thread -------------------
void TriggerBotLoop() {
    while(programRunning) {
        if(triggerBotActive) {
            // Implement trigger memory read & fire logic using offsets.hpp
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

// ------------------- Menu -------------------
void ConsoleMenu() {
    while(programRunning) {
        system("cls");
        if(currentMenu == MAIN_MENU) {
            std::cout << "=== Notepad Menu ===\n\n";
            std::cout << "1: TriggerBot: " << (triggerBotActive ? "\033[32mON\033[0m" : "\033[31mOFF\033[0m") << "\n";
            std::cout << "2: ESP: " << (cornerBoxActive ? "\033[32mON\033[0m" : "\033[31mOFF\033[0m") << "\n";
            std::cout << "3: Skeleton ESP: " << (skeletonActive ? "\033[32mON\033[0m" : "\033[31mOFF\033[0m") << "\n";
            std::cout << "4: Change ESP Color\n";
            std::cout << "5: Box Type: " << (useFullBox ? "Full Box" : "Corner Box") << "\n";
            std::cout << "6: Exit\n";

            if(_kbhit()) {
                int key = _getch();
                switch(key) {
                    case '1': triggerBotActive = !triggerBotActive; break;
                    case '2': cornerBoxActive = !cornerBoxActive; break;
                    case '3': skeletonActive = !skeletonActive; break;
                    case '4': currentMenu = ESP_COLOR_MENU; break;
                    case '5': useFullBox = !useFullBox; break;
                    case '6': programRunning = false; break;
                }
            }
        }
        else if(currentMenu == ESP_COLOR_MENU) {
            system("cls");
            std::cout << "=== ESP Color Menu ===\n\n";
            std::cout << "1: White\n2: Black\n3: Red\n4: Sky Blue\n5: Light Green\n";

            if(_kbhit()) {
                int key = _getch();
                switch(key) {
                    case '1': espColor = RGB(255,255,255); break;
                    case '2': espColor = RGB(0,0,0); break;
                    case '3': espColor = RGB(255,0,0); break;
                    case '4': espColor = RGB(135,206,235); break;
                    case '5': espColor = RGB(144,238,144); break;
                }
                currentMenu = MAIN_MENU; // return to main menu after selection
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// ------------------- Main -------------------
int main() {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "ESPOverlay";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        "ESPOverlay",
        "ESP Overlay",
        WS_POPUP,
        0,0,1920,1080,
        NULL,NULL,wc.hInstance,NULL
    );
    SetLayeredWindowAttributes(hwnd,0,255,LWA_ALPHA);
    ShowWindow(hwnd, SW_SHOW);

    std::thread espThread(ESPLoop);
    std::thread triggerThread(TriggerBotLoop);
    std::thread menuThread(ConsoleMenu);

    espThread.detach();
    triggerThread.detach();
    menuThread.join();

    DestroyWindow(hwnd);
    return 0;
}
