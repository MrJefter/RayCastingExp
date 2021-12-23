#include <iostream>
#include "TXLib.h"
#include "windows.h"
#include "arrays.h"

using namespace std;

double xPos = 3, yPos = 2;
bool isWalkingSoundPlaying = false, handShaking = 0, reload = 0;
double angle = 0;
int visibleEnemyCount = 0;
int handPos = 0, ammo = 6;

void drawAtScreen(double xView, double yView);
void rayCast();
void movementCheck(double xView, double yView);
void hudDrawing();

time_t timer = 6000;
time_t reloadTimer = 7000;

int windowXSize = GetSystemMetrics(SM_CXSCREEN), windowYSize = GetSystemMetrics(SM_CYSCREEN);
//int windowXSize = 800, windowYSize = 600;

HDC scrBuffer = txCreateCompatibleDC(windowXSize - 1, windowYSize - 1);

HDC hands = txLoadImage("resources/textures/hands.bmp");
HDC handsShoot = txLoadImage("resources/textures/hands_shoot.bmp");
HDC cross = txLoadImage("resources/textures/cross.bmp");
HDC bullet = txLoadImage("resources/textures/bullet.bmp");
HDC handsMem = txCreateCompatibleDC(windowYSize/1.8, windowYSize/1.8);
HDC handsShootMem = txCreateCompatibleDC(windowYSize/1.8, windowYSize/1.8);
HDC bulletMem = txCreateCompatibleDC(windowXSize/16, windowXSize/16);

int main() {
    StretchBlt(handsMem, 0, 0, windowYSize/1.8, windowYSize/1.8, hands, 0, 0, 180, 180, SRCCOPY);
    StretchBlt(handsShootMem, 0, 0, windowYSize/1.8, windowYSize/1.8, handsShoot, 0, 0, 180, 180, SRCCOPY);
    StretchBlt(bulletMem, 0, 0, windowXSize/16, windowXSize/16, bullet, 0, 0, 40, 40, SRCCOPY);
    double xView, yView;

    _txWindowStyle &= ~WS_CAPTION;
    txCreateWindow(windowXSize, windowYSize);
    txUpdateWindow(false);

    while (true) {
        xView = xPos + cos(angle) * 50;
        yView = yPos + sin(angle) * 50;
        drawAtScreen(xView, yView);
        movementCheck(xView, yView);
    }
}

void rayCast() {
    double winXCounter = 0, rayLength;
    COLORREF tempColor = RGB (252, 221, 118);
    txSetColor(tempColor, 1, scrBuffer);
    txSetFillColor(tempColor, scrBuffer);
    double bgGrad = windowYSize*0.9;
    txRectangle(0, windowYSize/2, windowXSize, windowYSize, scrBuffer);
    for (int count = 0; count < windowYSize*0.4; count++) {
        txLine(0, bgGrad, windowXSize, bgGrad, scrBuffer);
        tempColor = RGB (GetRValue(tempColor) - (255.0-127.0)/windowYSize*0.4,
                        GetGValue(tempColor) - (255.0-199.0)/windowYSize*0.4,
                        GetBValue(tempColor) - (255.0-118.0)/windowYSize*0.4);
        txSetColor(tempColor, 1, scrBuffer);
        bgGrad--;
    }
    int anotherCounter = 0;
    for (double xCount = angle - (M_PI/180.0)*30.0; xCount < angle + (M_PI/180.0)*30.0; xCount += 1.0/windowXSize) {
        double yCount;
        for (yCount = 1; yCount <= 16; yCount++) {
            if (mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] == '#') {
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                break;
            }
            rayLength = -1;
        }

        if (rayLength != -1) {
            if (255 - rayLength*(255/16) >= 0) {
                txSetColor(RGB (136 - rayLength*(136.0/16.0), 69 - rayLength*(69.0/16.0), 53 - rayLength*(53.0/16.0)), 1, scrBuffer);
                txSetFillColor(RGB (136 - rayLength*(136.0/16.0), 69 - rayLength*(69.0/16.0), 53 - rayLength*(53.0/16.0)), scrBuffer);
            }
            else {
                txSetColor(RGB (0, 0, 0), 1, scrBuffer);
                txSetFillColor(RGB (0, 0, 0), scrBuffer);
            }
            txLine(winXCounter, windowYSize/2*(1-1/rayLength), winXCounter, windowYSize/2*(1+1/rayLength), scrBuffer);
        }
        winXCounter++;
    }
    txBitBlt(0, 0, scrBuffer, 0, 0);
    hudDrawing();
    txRedrawWindow();
}

void movementCheck(double xView, double yView) {
    double shiftSpeed;
    if (GetKeyState(16) < -126) shiftSpeed = 1.5;
    else shiftSpeed = 1;
    if (GetKeyState(70) < -126 && GetTickCount() - timer > 1000 && ammo > 0) {
        timer = GetTickCount();
        handPos = 30;
        handShaking = true;
        txPlaySound("resources/sounds/shoot_sound.wav");
        isWalkingSoundPlaying = false;
        ammo--;
    }
    else if (!reload && (ammo <= 0 || (GetKeyState(82) < -126 && ammo != 6)) && GetTickCount() - timer > 1000 && GetTickCount() - reloadTimer > 6000) {
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
    if (GetKeyState(qKey) < -126) {
        angle -= (0.04 * shiftSpeed);
    }
    if (GetKeyState(eKey) < -126) {
        angle += (0.04 * shiftSpeed);
    }
    txSetFillColor(RGB (127, 199, 255), scrBuffer);
    txClear(scrBuffer);
}

void drawAtScreen(double xView, double yView) {
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
    txTransparentBlt(windowXSize/2-38, windowYSize/2-20,cross, TX_WHITE);
}