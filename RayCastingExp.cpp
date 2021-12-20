#include <iostream>
#include "TXLib.h"
#include "windows.h"
#include "arrays.h"

using namespace std;

double xPos = 20, yPos = 20;
bool isWalkingSoundPlaying = false, handShaking = 0, shoot = 0;
double angle = 0, handPos = 0;

void drawAtScreen(double xView, double yView);
void rayCast();
void movementCheck(double xView, double yView);
void handsDrawing();

time_t timer = 6000;

HDC hands = txLoadImage("hands.bmp");
HDC hands_shoot = txLoadImage("hands_shoot.bmp");
HDC cross = txLoadImage("cross.bmp");

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
    for (double xCount = angle - (3.14/180)*30; xCount < angle + (3.14/180)*30; xCount += 1.0/640.0) {
        double yCount;
        for (yCount = 1; yCount <= 160; yCount++) {
            if (mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] == '#') {
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                break;
            }
            rayLength = -1;
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
    handsDrawing();
    txTransparentBlt(301, 90, cross, TX_WHITE);
    txRedrawWindow();
}

void movementCheck(double xView, double yView) {
    double shiftSpeed;
    if (GetKeyState(16) < -126) shiftSpeed = 1.5;
    else shiftSpeed = 1;
    if (GetKeyState(70) < -126 && GetTickCount() - timer > 1000) {
        timer = GetTickCount();
        handPos = 30;
        handShaking = true;
        txPlaySound("shoot_sound");
        isWalkingSoundPlaying = false;
        shoot = true;
    }
    if (GetTickCount() - timer > 1000) {
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
void handsDrawing() {
    if (shoot) {
        txTransparentBlt(460+handPos, 90+handPos/3, hands_shoot, TX_WHITE);
        if (GetTickCount() - timer > 200) {
            shoot = false;
        }
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
}