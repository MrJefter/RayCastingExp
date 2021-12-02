#include <iostream>
#include "TXLib.h"
#include "windows.h"
#include "arrays.h"

using namespace std;

double xPos = 20, yPos = 20;
bool isSoundPlaying = false;
double angle = 0;

void drawAtScreen(double xView, double yView);
void rayCast();
void movementCheck(double xView, double yView);


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
    COLORREF tempColor = RGB (50, 50, 50);
    txSetColor(tempColor);
    double bgGrad = 200;
    while (tempColor != RGB (0, 0, 0)) {
        txLine(0, bgGrad, 640, bgGrad);
        tempColor = RGB (GetRValue(tempColor) - 1, GetGValue(tempColor) - 1, GetBValue(tempColor) - 1);
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
                txSetColor(RGB (/*255 - rayLength*6*/ 0, 255 - rayLength*3, /*255 - rayLength*6*/ 0));
                txSetFillColor(RGB (/*255 - rayLength*6*/ 0, 255 - rayLength*3, /*255 - rayLength*6*/ 0));
            }
            else {
                txSetColor(RGB (0, 0, 0));
                txSetFillColor(RGB (0, 0, 0));
            }
            txLine(winXCounter, ((200 - 200 / (1 + 0.08 * rayLength)) / 2), winXCounter, (200 - (200 - 200 / (1 + 0.04 * rayLength)) / 2));
        }
        winXCounter++;
    }
    txRedrawWindow();
}

void movementCheck(double xView, double yView) {
    double shiftSpeed;
    if (GetKeyState(16) < -126) shiftSpeed = 1.5;
    else shiftSpeed = 1;
    if ((GetKeyState(wKey) < -126 || GetKeyState(aKey) < -126 || GetKeyState(sKey) < -126 || GetKeyState(dKey) < -126)) {
        if (!isSoundPlaying) {
            txPlaySound("running_sound");
            isSoundPlaying = true;
        }
    }
    else {
        txPlaySound(0);
        isSoundPlaying = false;
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
    txSetFillColor(RGB (0, 0, 0));
    txClear();
}

void drawAtScreen(double xView, double yView) {
    rayCast();
}