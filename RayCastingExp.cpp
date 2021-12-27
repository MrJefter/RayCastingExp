#include <iostream>
#include "lib/TXLib.h"
#include "lib/arrays.h"

using namespace std;

double xPos = 20, yPos = 20;
bool isWalkingSoundPlaying = false, handShaking = 0, reload = 0;
double angle = 0;
int visibleEnemyCount = 0;
int handPos = 0, ammo = 6;

void drawAtScreen();
void rayCast();
void movementCheck(double xView, double yView);
void hudDrawing();

HCURSOR hCurs3;
HINSTANCE hinst;

time_t timer = 6000;
time_t reloadTimer = 7000;

int windowXSize = GetSystemMetrics(SM_CXSCREEN), windowYSize = GetSystemMetrics(SM_CYSCREEN);
//int windowXSize = 800, windowYSize = 600;

HDC scrBuffer = txCreateCompatibleDC(windowXSize, windowYSize);
HDC bgm = txCreateCompatibleDC(windowXSize, windowYSize);

HDC hands = txLoadImage("resources/textures/hands.bmp");
HDC handsShoot = txLoadImage("resources/textures/hands_shoot.bmp");
HDC cross = txLoadImage("resources/textures/cross.bmp");
HDC bullet = txLoadImage("resources/textures/bullet.bmp");
HDC handsMem = txCreateCompatibleDC(windowYSize/1.8, windowYSize/1.8);
HDC handsShootMem = txCreateCompatibleDC(windowYSize/1.8, windowYSize/1.8);
HDC bulletMem = txCreateCompatibleDC(windowXSize/16, windowXSize/16);

RECT rect = {0};

int main() {
    StretchBlt(handsMem, 0, 0, windowYSize/1.8, windowYSize/1.8, hands, 0, 0, 180, 180, SRCCOPY);
    StretchBlt(handsShootMem, 0, 0, windowYSize/1.8, windowYSize/1.8, handsShoot, 0, 0, 180, 180, SRCCOPY);
    StretchBlt(bulletMem, 0, 0, windowXSize/16, windowXSize/16, bullet, 0, 0, 40, 40, SRCCOPY);
    double xView, yView;

    txSetColor(RGB (252, 221, 118), 1, bgm);
    txSetFillColor(RGB (252, 221, 118), bgm);
    double bgGrad = windowYSize/2;
    while (bgGrad <= windowYSize) {
        txLine(0, bgGrad, windowXSize, bgGrad, bgm);
        txSetColor(RGB (252 - (windowYSize - bgGrad)*(252.0/windowYSize*1.2),
                        221 - (windowYSize - bgGrad)*(221.0/windowYSize*1.2),
                        118 - (windowYSize - bgGrad)*(118.0/windowYSize*1.2)),
                        1, bgm);
        bgGrad++;
    }
    txSetColor(RGB (127, 199, 255), 1, bgm);
    txSetFillColor(RGB (127, 199, 255), bgm);
    bgGrad = 0;
    while (bgGrad <= windowYSize/2 - 1) {
        txLine(0, bgGrad, windowXSize, bgGrad, bgm);
        txSetColor(RGB (127 - bgGrad*(127.0/windowYSize*1.2),
                        199 - bgGrad*(199.0/windowYSize*1.2),
                        255 - bgGrad*(255.0/windowYSize*1.2)),
                        1, bgm);
        bgGrad++;
    }

    _txWindowStyle &= ~WS_CAPTION;
    txCreateWindow(windowXSize, windowYSize);
    txUpdateWindow(false);

    GetWindowRect(txWindow(), &rect);

    SetForegroundWindow(txWindow());
    SetActiveWindow(txWindow());
    SetFocus(txWindow());
    Sleep(300);
    SetCursorPos(rect.right - windowXSize/2, rect.bottom - windowYSize/2);
    hCurs3 = CreateCursor(hinst, 19, 2, 32, 32, ANDmaskCursor, XORmaskCursor);
    SetClassLongA(txWindow(), GCL_HCURSOR, (long)hCurs3);

    while (true) {
        xView = xPos + cos(angle) * 50;
        yView = yPos + sin(angle) * 50;
        drawAtScreen();
        movementCheck(xView, yView);
        SetCursorPos(rect.right - windowXSize/2, rect.bottom - windowYSize/2);
        while (GetKeyState(escKey) < -126) {
            txSetFillColor(RGB (0, 0, 0));
            txClear();
            txTextOut(0, 0, "Press enter to stop the program.");
            txSetColor(RGB (255, 255, 255));
            txUpdateWindow();
            if (GetKeyState(enterKey) < -126) {
                txClear();
                txTextOut(0, 0, "Process finished. Press any key to close the window.");
                txUpdateWindow();
                return 0;
            }
        }
    }
}

void rayCast() {
    double winXCounter = 0, rayLength;
    txBitBlt(scrBuffer, 0, 0, 0, 0, bgm, 0, 0);
    for (double xCount = angle - M_PI/3.0; xCount < angle + M_PI/3.0; xCount += M_PI/1.5/windowXSize) {
        double yCount;
        for (yCount = 1; yCount <= 160; yCount++) {
            if (mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] == '#') {
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                break;
            }
            rayLength = -1;
        }

        if (rayLength != -1) {
            if (255 - rayLength*(255.0/160.0) >= 0) {
                txSetColor(RGB (136 - rayLength*(136.0/160.0), 69 - rayLength*(69.0/160.0), 53 - rayLength*(53.0/160.0)), 1, scrBuffer);
                txSetFillColor(RGB (136 - rayLength*(136.0/160.0), 69 - rayLength*(69.0/160.0), 53 - rayLength*(53.0/160.0)), scrBuffer);
            }
            else {
                txSetColor(RGB (0, 0, 0), 1, scrBuffer);
                txSetFillColor(RGB (0, 0, 0), scrBuffer);
            }
            txLine(winXCounter, windowYSize/2*(1-7/rayLength), winXCounter, windowYSize/2*(1+7/rayLength), scrBuffer);
        }
        winXCounter++;
    }
    txBitBlt(0, 0, scrBuffer, 0, 0);
    hudDrawing();
    txRedrawWindow();
}

void movementCheck(double xView, double yView) {
    double shiftSpeed;
    if (GetKeyState(shiftKey) < -126) shiftSpeed = 1.5;
    else shiftSpeed = 1;
    if (GetKeyState(leftMouse) < -126 && GetTickCount() - timer > 1000 && ammo > 0) {
        timer = GetTickCount();
        handPos = 30;
        handShaking = true;
        txPlaySound("resources/sounds/shoot_sound.wav");
        isWalkingSoundPlaying = false;
        ammo--;
    }
    else if (!reload && (ammo <= 0 || (GetKeyState(rKey) < -126 && ammo != 6)) && GetTickCount() - timer > 1000 && GetTickCount() - reloadTimer > 6000) {
        ammo = 0;
        txPlaySound("resources/sounds/reload_sound.wav");
        reload = true;
        isWalkingSoundPlaying = false;
        reloadTimer = GetTickCount();
    }
    if (GetTickCount() - timer > 1000 && GetTickCount() - reloadTimer > 6000) {
    if ((GetKeyState(wKey) < -126 || GetKeyState(aKey) < -126 || GetKeyState(sKey) < -126 || GetKeyState(dKey) < -126)) {
        if (!handShaking) {
        handPos++;
        if (handPos == 25) handShaking = true;
    }
    else {
        handPos--;
        if (handPos == 0) handShaking = false;
    }
        if (!isWalkingSoundPlaying) {
            txPlaySound("resources/sounds/running_sound.wav");
            isWalkingSoundPlaying = true;
        }
    }
    else {
        txPlaySound(0);
        isWalkingSoundPlaying = false;
    }
    }
    if (GetKeyState(wKey) < -126) {
        if (mapArray[(int)(xPos + (xView-xPos)/40)][(int)(yPos + (yView-yPos)/40)] == '.') {
            xPos = xPos + (xView-xPos)/(80 / shiftSpeed);
            yPos = yPos + (yView-yPos)/(80 / shiftSpeed);
        }
    }
    if (GetKeyState(sKey) < -126) {
        if (mapArray[(int)(xPos - (xView-xPos)/40)][(int)(yPos - (yView-yPos)/40)] == '.') {
            xPos = xPos - (xView-xPos)/(80 / shiftSpeed);
            yPos = yPos - (yView-yPos)/(80 / shiftSpeed);
        }
    }
    if (GetKeyState(aKey) < -126) {
        if (mapArray[(int)(xPos - (cos(angle+(3.14/180)*90)*50)/40)][(int)(yPos - (sin(angle+(3.14/180)*90)*50)/40)] == '.') {
            xPos = xPos + (cos(angle-(3.14/180)*90)*50)/(80 / shiftSpeed);
            yPos = yPos + (sin(angle-(3.14/180)*90)*50)/(80 / shiftSpeed);
        }
    }
    if (GetKeyState(dKey) < -126) {
        if (mapArray[(int)(xPos + (cos(angle+(3.14/180)*90)*50)/40)][(int)(yPos + (sin(angle+(3.14/180)*90)*50)/40)] == '.') {
            xPos = xPos + (cos(angle+(3.14/180)*90)*50)/(80 / shiftSpeed);
            yPos = yPos + (sin(angle+(3.14/180)*90)*50)/(80 / shiftSpeed);
        }
    }
    angle += (txMouseX() - windowXSize/2) * M_PI/180.0;
    txClear(scrBuffer);
}

void drawAtScreen() {
    rayCast();
}
void hudDrawing() {
    for (int i = 0; i < ammo; i++) {
        txTransparentBlt(-windowXSize/64+(i*windowXSize/43), 0, bulletMem, TX_WHITE);
    }
    if (reload) {
        if (GetTickCount() - reloadTimer > 6000) {
            reload = false;
            ammo = 6;
        }
    }
    else if (GetTickCount() - timer <= 200) {
        txTransparentBlt((windowXSize-windowYSize/1.8)+handPos*(windowYSize/243), (windowYSize-windowYSize/1.8)+handPos*(windowYSize/243)/3, handsShootMem, TX_WHITE);
    }
    else {
        txTransparentBlt((windowXSize-windowYSize/1.8)+handPos*(windowYSize/243), (windowYSize-windowYSize/1.8)+handPos*(windowYSize/243)/3, handsMem, TX_WHITE);
    }
    txTransparentBlt(windowXSize/2-19, windowYSize/2-10,cross, TX_WHITE);
}