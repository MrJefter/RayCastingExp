#include <iostream>
#include "TXLib.h"
#include "windows.h"
#include "arrays.h"

using namespace std;

double xPos = 20, yPos = 20;
bool isWalkingSoundPlaying = false, handShaking = 0, reload = 0;
double angle = 0, rayLengthTmp[640], enemyRayLength[5], enemyPos[5], visibleEnemyCount = 0;
int handPos = 0, ammo = 6;

void drawAtScreen(double xView, double yView);
void rayCast();
void movementCheck(double xView, double yView);
void hudDrawing();
void enemyDrawing();

time_t timer = 6000;
time_t reloadTimer = 7000;

HDC hands = txLoadImage("hands.bmp");
HDC hands_shoot = txLoadImage("hands_shoot.bmp");
HDC cross = txLoadImage("cross.bmp");
HDC bullet = txLoadImage("bullet.bmp");
HDC enemy = txLoadImage("enemy.bmp");

int main() {
    double xView, yView;

    txCreateWindow(640, 200);
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
    txSetColor(tempColor);
    txSetFillColor(tempColor);
    double bgGrad = 175;
    txRectangle(0, 176, 640, 200);
    for (int count = 0; count < 70; count++) {
        txLine(0, bgGrad, 640, bgGrad);
        tempColor = RGB (GetRValue(tempColor) - (255.0-127.0)/70.0,
                        GetGValue(tempColor) - (255.0-199.0)/70.0,
                        GetBValue(tempColor) - (255.0-118.0)/70.0);
        txSetColor(tempColor);
        bgGrad--;
    }
    int anotherCounter = 0;
    for (double xCount = angle - (3.14/180)*30; xCount < angle + (3.14/180)*30; xCount += 1.0/640.0) {
        double yCount;
        for (yCount = 1; yCount <= 160; yCount++) {
            if (mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] == '#') {
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                break;
            }
            if (mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] == 'E') {
                
                enemyRayLength[visibleEnemyCount] = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                enemyPos[visibleEnemyCount] = xCount;
                visibleEnemyCount++;
            }
            rayLength = -1;
            rayLengthTmp[anotherCounter] = rayLength;
            anotherCounter++;
        }

        if (rayLength != -1) {
            if (255 - rayLength*3 >= 0) {
                txSetColor(RGB (136 - rayLength*(136.0/85.0), 69 - rayLength*(69.0/85.0), 53 - rayLength*(53.0/85.0)));
                txSetFillColor(RGB (136 - rayLength*(136.0/85.0), 69 - rayLength*(69.0/85.0), 53 - rayLength*(53.0/85.0)));
            }
            else {
                txSetColor(RGB (0, 0, 0));
                txSetFillColor(RGB (0, 0, 0));
            }
            txLine(winXCounter, ((200 - 200 / (1 + 0.08 * rayLength)) / 2), winXCounter, (200 - (200 - 200 / (1 + 0.04 * rayLength)) / 2));
        }
        winXCounter++;
    }
    enemyDrawing();
    hudDrawing();
    txRedrawWindow();
}

void enemyDrawing() {
    if (visibleEnemyCount > 0) {
        for (visibleEnemyCount; visibleEnemyCount > 0; visibleEnemyCount++) {
            txLine(enemyPos[visibleEnemyCount] - 73, ((200 - 200 / (1 + 0.08 * rayLength)) / 2),
            enemyPos[visibleEnemyCount] + 74, (200 - (200 - 200 / (1 + 0.04 * rayLength)) / 2));
        }
    }
}

void movementCheck(double xView, double yView) {
    double shiftSpeed;
    if (GetKeyState(16) < -126) shiftSpeed = 1.5;
    else shiftSpeed = 1;
    if (GetKeyState(70) < -126 && GetTickCount() - timer > 1000 && ammo > 0) {
        timer = GetTickCount();
        handPos = 30;
        handShaking = true;
        txPlaySound("shoot_sound");
        isWalkingSoundPlaying = false;
        ammo--;
    }
    else if (!reload && (ammo <= 0 || (GetKeyState(82) < -126 && ammo != 6)) && GetTickCount() - timer > 1000 && GetTickCount() - reloadTimer > 6000) {
        ammo = 0;
        txPlaySound("reload_sound");
        reload = true;
        isWalkingSoundPlaying = false;
        reloadTimer = GetTickCount();
    }
    if (GetTickCount() - timer > 1000 && GetTickCount() - reloadTimer > 6000) {
    if ((GetKeyState(wKey) < -126 || GetKeyState(aKey) < -126 || GetKeyState(sKey) < -126 || GetKeyState(dKey) < -126)) {
        if (!isWalkingSoundPlaying) {
            txPlaySound("running_sound");
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
    txSetFillColor(RGB (127, 199, 255));
    txClear();
}

void drawAtScreen(double xView, double yView) {
    rayCast();
}
void hudDrawing() {
    for (int i = 0; i < ammo; i++) {
        txTransparentBlt(-10+(i*15), 0, bullet, TX_WHITE);
    }
    if (reload) {
        if (GetTickCount() - reloadTimer > 6000) {
            reload = false;
            ammo = 6;
        }
    }
    else if (GetTickCount() - timer <= 200) {
        txTransparentBlt(460+handPos, 90+handPos/3, hands_shoot, TX_WHITE);
    }
    else txTransparentBlt(460+handPos, 90+handPos/3, hands, TX_WHITE);
    if (!handShaking) {
        handPos++;
        if (handPos == 25) handShaking = true;
    }
    else {
        handPos--;
        if (handPos == 0) handShaking = false;
    }
    txTransparentBlt(301, 90, cross, TX_WHITE);
}
