#include <iostream>
#include "TXLib.h"
#include "windows.h"
#include "thread"

using namespace std;

char mapArray[161][161] = {
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"###################################################################################...................................................................##########",
"###################################################################################...................................................................##########",
"###################################################################################...................................................................##########",
"###################################################################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................###########################################...................................................................##########",
"#################.......................##########################################################################################....................##########",
"#################.......................##########################################################################################....................##########",
"#################.......................##########################################################################################....................##########",
"#################.......................##########################################################################################....................##########",
"#################.......................##########################################################################################....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"#################......................................................................................................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################....................###########....................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################...................................................##########",
"############################################################...................####################....................#########################################",
"############################################################...................####################....................#########################################",
"############################################################...................####################....................#########################################",
"############################################################...................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................................................####################....................#########################################",
"###########################....................####################################################....................#########################################",
"###########################....................####################################################....................#########################################",
"###########################....................####################################################....................#########################################",
"###########################....................####################################################....................#########################################",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################....................####################################################...................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"###########################...........................................................................................................................##########",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
"################################################################################################################################################################",
};

int main() {
    float xView, yView, xPos = 20, yPos = 20, winXCounter, rayLenght;
    float angle = 0;
    txCreateWindow(320, 100);

    char keyTmp;
    char wKey = 87, aKey = 65, sKey = 83, dKey = 68;
    while (true) {
        txUpdateWindow(false);
        xView = xPos + cos(angle) * 50;
        yView = yPos + sin(angle) * 50;
        winXCounter = 0;
        COLORREF tempColor = RGB (50, 50, 50);
        txSetColor(tempColor);
        int bgGrad = 100;
        while (tempColor != RGB (0, 0, 0)) {
            txLine(0, bgGrad, 320, bgGrad);
            tempColor = RGB (GetRValue(tempColor) - 1, GetGValue(tempColor) - 1, GetBValue(tempColor) - 1);
            txSetColor(tempColor);
            bgGrad--;
        }
        for (float xCount = angle - 0.5; xCount < angle + 0.5; xCount += 1.0/320.0) {
            float yCount;
            for (yCount = 1; yCount <= 160; yCount++) {
                if (mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] == '#') {
                    rayLenght = (int)sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                    break;
                }
                rayLenght = -1;
            }

            if (rayLenght != -1) {
                if (255 - rayLenght*3 >= 0) {
                    txSetColor(RGB (/*255 - rayLenght*6*/ 0, 255 - rayLenght*3, /*255 - rayLenght*6*/ 0));
                    txSetFillColor(RGB (/*255 - rayLenght*6*/ 0, 255 - rayLenght*3, /*255 - rayLenght*6*/ 0));
                }
                else {
                    txSetColor(RGB (0, 0, 0));
                    txSetFillColor(RGB (0, 0, 0));
                }
                txLine(winXCounter, ((100 - 100 / (1 + 0.08 * rayLenght)) / 2), winXCounter, (100 - (100 - 100 / (1 + 0.04 * rayLenght)) / 2));
            }
            winXCounter++;
        }
        txRedrawWindow();
        keyTmp = wKey;
        if (GetKeyState(keyTmp) < -126) {
            if (mapArray[(int)(xPos + (xView-xPos)/20)][(int)(yPos + (yView-yPos)/20)] == '.') {
                xPos = xPos + (xView-xPos)/40;
                yPos = yPos + (yView-yPos)/40;
            }
        }
        keyTmp = sKey;
        if (GetKeyState(keyTmp) < -126) {
            if (mapArray[(int)(xPos - (xView-xPos)/20)][(int)(yPos - (yView-yPos)/20)] == '.') {
                xPos = xPos - (xView-xPos)/40;
                yPos = yPos - (yView-yPos)/40;
            }
        }
        keyTmp = aKey;
        if (GetKeyState(keyTmp) < -126) {
            angle -= 0.04;
        }
        keyTmp = dKey;
        if (GetKeyState(keyTmp) < -126) {
            angle += 0.04;
        }
        txSetFillColor(RGB (0, 0, 0));
        txClear();
    }
}
