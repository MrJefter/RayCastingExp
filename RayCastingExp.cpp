#include <iostream>
#include "lib/TXLib.h"
#include "lib/arrays.h"

using namespace std;

double xPos = 20, yPos = 20;
bool isWalkingSoundPlaying = false, handShaking = false,
reload = false, toggleKill = false, menuButtonUp = false,
menuButtonDown = false, startup = true, difficulty = false,
keyPressed = false;
double angle = 0;
int handPos = 0, ammo = 6, score = 0, lives = 10,
enemyAtScreen = 0, enemySize = 0, bottomSide = 0;
int saveX = 0, saveY = 0;

void drawAtScreen();
void rayCast();
void movementCheck(double xView, double yView);
void hudDrawing();

HCURSOR hCurs3;
HINSTANCE hinst;

time_t timer = 6000;
time_t reloadTimer = 7000;
time_t enemyShootTimer = 2000;

int windowXSize = GetSystemMetrics(SM_CXSCREEN), windowYSize = GetSystemMetrics(SM_CYSCREEN);
//int windowXSize = 800, windowYSize = 600;

HDC scrBuffer = txCreateCompatibleDC(windowXSize, windowYSize);
HDC bgm = txCreateCompatibleDC(windowXSize, windowYSize);

HDC hands = txLoadImage("resources/textures/hands.bmp");
HDC handsShoot = txLoadImage("resources/textures/hands_shoot.bmp");
HDC cross = txLoadImage("resources/textures/cross.bmp");
HDC bullet = txLoadImage("resources/textures/bullet.bmp");
HDC enemy = txLoadImage("resources/textures/sakuya_fumo.bmp");
HDC mainMenu = txLoadImage("resources/textures/FSSim_menu.bmp");
HDC deathScreen = txLoadImage("resources/textures/death_screen.bmp");
HDC winScreen = txLoadImage("resources/textures/win_screen.bmp");
HDC handsMem = txCreateCompatibleDC(windowYSize/1.8, windowYSize/1.8);
HDC handsShootMem = txCreateCompatibleDC(windowYSize/1.8, windowYSize/1.8);
HDC bulletMem = txCreateCompatibleDC(windowXSize/16, windowXSize/16);
HDC enemyMem = txCreateCompatibleDC(windowXSize, windowYSize);
HDC mainMenuMem = txCreateCompatibleDC(windowXSize, windowYSize);
HDC deathScreenMem = txCreateCompatibleDC(windowXSize, windowYSize);
HDC winScreenMem = txCreateCompatibleDC(windowXSize, windowYSize);

RECT rect = {0};

int main() {
    StretchBlt(mainMenuMem, 0, 0, windowXSize - 1, windowYSize - 1, mainMenu, 0, 0, 1280, 720, SRCCOPY);
    StretchBlt(handsMem, 0, 0, (int)(windowYSize/1.8), (int)(windowYSize/1.8), hands, 0, 0, 180, 180, SRCCOPY);
    StretchBlt(handsShootMem, 0, 0, (int)(windowYSize/1.8), (int)(windowYSize/1.8), handsShoot, 0, 0, 180, 180, SRCCOPY);
    StretchBlt(bulletMem, 0, 0, windowXSize/16, windowXSize/16, bullet, 0, 0, 40, 40, SRCCOPY);
    StretchBlt(deathScreenMem, 0, 0, windowXSize - 1, windowYSize - 1, deathScreen, 0, 0, 1280, 720, SRCCOPY);
    StretchBlt(winScreenMem, 0, 0, windowXSize - 1, windowYSize - 1, winScreen, 0, 0, 1280, 720, SRCCOPY);
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
    //txUpdateWindow(false);

    txSelectFont("Arial Black", 60);

    GetWindowRect(txWindow(), &rect);

    SetForegroundWindow(txWindow());
    SetActiveWindow(txWindow());
    SetFocus(txWindow());
    Sleep(300);
    SetCursorPos(rect.right - windowXSize/2, rect.bottom - windowYSize/2);
    hCurs3 = CreateCursor(hinst, 19, 2, 32, 32, ANDmaskCursor, XORmaskCursor);
    SetClassLongA(txWindow(), GCL_HCURSOR, (long)hCurs3);

    int menuPos = 0;
    txPlaySound("resources/sounds/background.wav");
    while (startup) {
        txBegin();
        txClear();
        txBitBlt(0, 0, mainMenuMem);
        if (GetKeyState(38) < 0) {
            if (!menuButtonUp) {
                if (menuPos <= 0) menuPos = 2;
                else menuPos--;
                menuButtonUp = true;
            }
        }
        else menuButtonUp = false;
        if (GetKeyState(40) < 0) {
            if (!menuButtonDown) {
                if (menuPos >= 2) menuPos = 0;
                else menuPos++;
                menuButtonDown = true;
            }
        }
        else menuButtonDown = false;
        txSetFillColor(RGB (0, 0, 0));
        txSetColor(RGB (0, 0, 0));
        switch (menuPos) {
            case 0:
                txCircle(windowXSize/9*6, windowYSize/10*4, 10);
                if (GetKeyState(13) < 0) startup = false;
                break;
            case 1:
                txCircle(windowXSize/9*6, windowYSize/10*6, 10);
                if (GetKeyState(13) < 0) {
                    if (!keyPressed) {
                    difficulty = !difficulty;
                    keyPressed = true;
                    }
                }
                else keyPressed = false;
                if (difficulty) txTextOut(windowXSize/9*6 - 180, windowYSize/10*6 - 30, "normal");
                else txTextOut(windowXSize/9*6 - 180, windowYSize/10*6 - 30, "peaceful");
                break;
            case 2:
                txCircle(windowXSize/9*6, windowYSize/10*8, 10);
                if (GetKeyState(13) < 0) return(0);
                break;
            default:
                break;
        }
        txEnd();
        Sleep(1);
    }
    txPlaySound(0);

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
                txTextOut(windowXSize/2 - 300, windowYSize/2, "Process finished. Press any key to close the window.");
                txUpdateWindow();
                return 0;
            }
        }
        if (lives <= 0) {
            txClear();
            txSetColor(RGB (255, 75, 75));
            txBitBlt(0, 0, deathScreenMem);
            txTextOut(0, 0, "You died. Press any key to close the window.");
            txPlaySound("resources/sounds/death_sound.wav");
            txUpdateWindow();
            return 0;
        }
        if (score >= 7) {
            txClear();
            txSetColor(RGB (75, 255, 75));
            txBitBlt(0, 0, winScreenMem);
            txTextOut(0, 0, "You win! Press any key to close the window.");
            txPlaySound("resources/sounds/win_sound.wav");
            txUpdateWindow();
            return 0;
        }
    }
}

void rayCast() {
    double winXCounter = 0, rayLength;
    txBitBlt(scrBuffer, 0, 0, 0, 0, bgm, 0, 0);
    for (double xCount = angle - M_PI/3.0; xCount < angle + M_PI/3.0; xCount += M_PI/1.5/windowXSize) {
        double yCount;
        char checkChar;
        for (yCount = 1; yCount <= 160; yCount++) {
            checkChar = mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)];
            if (checkChar == '#') {
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                break;
            }
            if (checkChar == 'A') {
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                if (rayLength < 5) {
                    mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] = '.';
                    lives++;
                }
                break;
            }
            if (checkChar == 'E') {
                enemyAtScreen = (int)winXCounter;
                rayLength = sqrt(pow(cos(xCount) * yCount, 2) + pow(sin(xCount) * yCount, 2));
                txSetColor(RGB (255, 0, 0), 1, scrBuffer);
                txSetFillColor(RGB (255, 0, 0), scrBuffer);
                enemySize = (int)(windowYSize/2*(1+7/rayLength) - windowYSize/2*(1-7/rayLength));
                saveX = (int)winXCounter;
                saveY = (int)(windowYSize/2*(1-7/rayLength));
                if (winXCounter > windowXSize/2 - enemySize/2 && winXCounter < windowXSize/2 + enemySize/2 && GetTickCount() - timer < 200) {
                    mapArray[(int)(xPos + cos(xCount) * yCount)][(int)(yPos + sin(xCount) * yCount)] = '.';
                    if (!toggleKill) {
                        toggleKill = true;
                        score++;
                    }
                }
                else toggleKill = false;
                break;
            }
            rayLength = -1;
        }

        if (rayLength != -1 && checkChar == '#') {
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
        else if (rayLength != -1 && checkChar == 'A') {
            txSetFillColor(RGB (0, 0, 255), scrBuffer);
            txSetColor(RGB (0, 0, 255), 1, scrBuffer);
            txLine(winXCounter, windowYSize/2*(1-7/rayLength), winXCounter, windowYSize/2*(1+7/rayLength), scrBuffer);
        }
        winXCounter++;
    }
    if (enemyAtScreen != 0) {
        StretchBlt(enemyMem, 0, 0, enemySize, enemySize, enemy, 0, 0, 240, 240, SRCCOPY);
        for (int i = 1; i <= enemySize; i++) {
            if (GetGValue(txGetPixel((saveX + enemySize/2) - i, saveY - 1, scrBuffer)) > 69) txTransparentBlt(scrBuffer, (saveX + enemySize/2) - i, saveY, 1, enemySize, enemyMem, enemySize - i, 0, TX_WHITE);
        }
    }
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
    txBitBlt(0, 0, scrBuffer, 0, 0);
    hudDrawing();
    string tempStrScore = "Score: " + to_string(score);
    const char* scoreStr = tempStrScore.c_str();
    string tempStrHearts = "Health: " + to_string(lives);
    const char* scoreHearts = tempStrHearts.c_str();
    txTextOut(10, windowYSize/10, scoreStr);
    txTextOut(10, windowYSize/10*2, scoreHearts);
    if (enemyAtScreen != 0) {
        if (difficulty && GetTickCount() - enemyShootTimer > 2000) {
            enemyShootTimer = GetTickCount();
            bottomSide = rand()%windowXSize;
            if (bottomSide > windowXSize/4 && bottomSide < windowXSize/4*3) lives--;
            txPlaySound("resources/sounds/shoot_sound.wav");
        }
        if (GetTickCount() - enemyShootTimer < 500) {
            txSetColor(RGB (0, 0, 0), 3);
            txLine(enemyAtScreen - enemySize/2 + enemySize/10, windowYSize/2 + enemySize/6, bottomSide, windowYSize);
            txSetColor(RGB (0, 0, 0), 1);
        }
    }
    enemyAtScreen = 0;
    txRedrawWindow();
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