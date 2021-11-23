#include <iostream>
#include "TXLib.h"
#include "windows.h"
#include "thread"

using namespace std;

int mapX = 330, mapY = 0;

POINT area[8] = {
    {mapX + 40, mapY + 0}, {mapX + 120, mapY + 0}, {mapX + 160, mapY + 40}, {mapX + 160, mapY + 120},
    {mapX + 120, mapY + 160}, {mapX + 40, mapY + 160}, {mapX + 0, mapY + 120}, {mapX + 0, mapY + 40}
};

void drawMap();

int main() {
    int xView, yView, xPos = mapX + 80, yPos = mapY + 80, winXCounter, rayLenght;
    float angle = 0;
    txCreateWindow(500, 161);
    
    char keyTmp;
    char wKey = 87, aKey = 65, sKey = 83, dKey = 68;
    while (true) {
        txUpdateWindow(false);
        txSetColor(RGB (0, 0, 0));
        drawMap();
        txSetColor(RGB (0, 0, 0));
        txSetFillColor(TX_TRANSPARENT);
        txSetFillColor(RGB (125, 125, 125));
        xView = xPos + cos(angle) * 50;
        yView = yPos + sin(angle) * 50;
        txSetPixel(xPos, yPos, RGB (125, 125, 125));
        winXCounter = 0;
        COLORREF tempColor = RGB (25, 25, 25); 
        txSetColor(tempColor);
        int bgGrad = 100;
        while (tempColor != RGB (0, 0, 0)) {
            txLine(0, bgGrad, 320, bgGrad);
            tempColor = RGB (GetRValue(tempColor) - 1, GetGValue(tempColor) - 1, GetBValue(tempColor) - 1);
            txSetColor(tempColor);
            bgGrad--;
        }
        txSetColor(RGB (0, 0, 0));
        for (float xCount = angle - 0.5; xCount < angle + 0.5; xCount += 1.0/80.0) {
            txSetColor(RGB (125, 125, 125));
            float yCount;
            for (yCount = 1; yCount <= 50; yCount++) {
                if (txGetPixel((float)xPos + cos(xCount) * yCount, (float)yPos + sin(xCount) * yCount) == RGB (0, 0, 0) || txGetPixel((float)xPos + cos(xCount) * yCount, (float)yPos + sin(xCount) * yCount) == RGB (203, 179, 83)) {
                    rayLenght = (int)sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                    txLine(xPos, yPos, xPos + cos(xCount) * yCount, yPos + sin(xCount) * yCount);
                    break;
                }
                rayLenght = -1;
            }

            if (rayLenght != -1) {
                if (255 - rayLenght*6 >= 0) {
                    txSetColor(RGB (/*255 - rayLenght*6*/ 0, 255 - rayLenght*6, /*255 - rayLenght*6*/ 0));
                    txSetFillColor(RGB (/*255 - rayLenght*6*/ 0, 255 - rayLenght*6, /*255 - rayLenght*6*/ 0));
                }
                else {
                    txSetColor(RGB (0, 0, 0));
                    txSetFillColor(RGB (0, 0, 0));
                }
                txRectangle(winXCounter, ((100 - 100 / (1 + 0.08 * rayLenght)) / 2), winXCounter + 4, (100 - (100 - 100 / (1 + 0.04 * rayLenght)) / 2));
            }
            winXCounter += 4;
        }
        txRedrawWindow();
        txSetColor(RGB (0, 0, 0));
        keyTmp = wKey;
        if (GetKeyState(keyTmp) < -126) {
            if (txGetPixel(xPos + (xView-xPos)/10, yPos + (yView-yPos)/10) != RGB (0, 0, 0)) {
                xPos = xPos + (xView-xPos)/20;
                yPos = yPos + (yView-yPos)/20;
            }
        }
        keyTmp = sKey;
        if (GetKeyState(keyTmp) < -126) {
            if (txGetPixel(xPos - (xView-xPos)/10, yPos - (yView-yPos)/10) != RGB (0, 0, 0)) {
                xPos = xPos - (xView-xPos)/20;
                yPos = yPos - (yView-yPos)/20;
            }
        }
        keyTmp = aKey;
        if (GetKeyState(keyTmp) < -126) {
            angle -= 0.05;
        }
        keyTmp = dKey;
        if (GetKeyState(keyTmp) < -126) {
            angle += 0.05;
        }
        txSetFillColor(RGB (0, 0, 0));
        txClear();
    }
}

void drawMap() {
    txSetFillColor(RGB (255, 255, 255));
    txPolygon(area, 8);
    txSetFillColor(RGB (0, 0, 0));
    txRectangle(mapX + 30, mapY + 30, mapX + 40, mapY + 40);
    txRectangle(mapX + 120, mapY + 30, mapX + 130, mapY + 40);
    txRectangle(mapX + 30, mapY + 120, mapX + 40, mapY + 130);
    txRectangle(mapX + 120, mapY + 120, mapX + 130, mapY + 130);
}