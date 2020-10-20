#include <iostream>
#include <string>
#include <windows.h>
#include "mingw.thread.h"
using namespace std;
//--------------------------------------------------
//Global Declares
wstring player;
unsigned char *field = nullptr;
int fieldwidth = 60;
int fieldheight = 18;
int screenwidth = 80;
int screenheight = 30;
bool gameover = false;
//--------------------------------------------------
bool collision(int nXpos, int nYpos) {
    int pos = (nYpos - 2) * fieldwidth + (nXpos - 2);
    if (field[pos] != 0) {
        return false;
    }
    return true;
}
//Make asset drawing function
void draw_asset(int xlen, int ylen, int nXpos, int nYpos) {
    for (int px = 0; px < xlen; px++) {
        for (int py = (3 - ylen); py < 3; py++) {
            field[(nYpos  + py)*fieldwidth + (nXpos + px)] = 2;
        }
    }
    return;
}

void remove_asset(int xlen, int ylen, int nXpos, int nYpos) {
    for (int px = 0; px < xlen - 1; px++) {
        for (int py = (3 - ylen); py < 3; py++) {
            field[(nYpos  + py)*fieldwidth + (nXpos + px)] = 0;
        }
    }
}

bool collisonXwall(int nXpos, int nYpos) {
    
}
//--------------------------------------------------
//TODO:

//Make function that deletes 'X' values if it collides with '|' value

//Make function that generates random block assets for player to jump

//Somehow allow player to move to the right and have screen 'scroll' ...

//Create scoring mechanism that awards points as time passes OR as blocks are jumped (how?)

//Implement game over case for player collision with block asset
//--------------------------------------------------
//Game Logic Stuff
int Xpos = 3;
int Ypos = 18;
int asset1X = fieldwidth - 3;
int asset2X = fieldwidth - 3;
int gconst = 10;
int gconstcounter = 0;
int movementconst = 7;
int rightconstcounter = 0;
int leftconstcounter = 0;
int blockcounter = 0;
int blockspeed = 15;
bool keypress[3];
bool uphold = false;
bool ongroundagain = true;
bool bgravdown = false;
bool gorightagain = false;
bool goleftagain = false;
bool blockmove = true;
//--------------------------------------------------
int main() {
    //--------------------------------------------------
    //FIELD
    field = new unsigned char [fieldwidth * fieldheight];
    for (int x = 0; x < fieldwidth; x++) {
        for (int y = 0; y < fieldheight; y++) {
            if (x == 0 || x == fieldwidth - 1) {
                field[y*fieldwidth + x] = y == 17 ? 2 : 3;
            }
            else {
                field[y*fieldwidth + x] = y == 17 ? 2 : 0; 
            }
             
        }
    }
    //--------------------------------------------------
    //BUFFER
    wchar_t *screen = new wchar_t[screenwidth*screenheight];
    for (int i = 0; i < screenwidth*screenheight; i++) {
        screen[i] = L' ';
    }
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
    //--------------------------------------------------
    while(!gameover) {
        //TIMING
        this_thread::sleep_for(15ms);
        gconstcounter++;
        rightconstcounter++;
        leftconstcounter++;
        blockcounter++;
        bgravdown = (gconstcounter == gconst);
        gorightagain = (rightconstcounter == movementconst);
        goleftagain = (leftconstcounter == movementconst);
        blockmove = (blockcounter == blockspeed);
        //--------------------------------------------------
        //INPUT
        for (int k = 0; k < 3; k++) {
            keypress[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x26"[k]))) != 0;
        } 
        //-------------------------------------------------- 
        //LOGIC
        if (gorightagain) {
            if (keypress[0] && collision(Xpos + 1, Ypos)) { 
                Xpos += 1; //right
            }
            rightconstcounter = 0;
        }
        if (goleftagain) {
            if (keypress[1] && collision(Xpos - 1, Ypos)) {
                Xpos -= 1; //left
            }
            leftconstcounter = 0;
        }
        if (keypress[2] && collision(Xpos, Ypos - 1)) {
            if (uphold == false && ongroundagain == true) {
                Ypos -= 1; //jump
                this_thread::sleep_for(10ms);
                Ypos -= 1;
                this_thread::sleep_for(10ms);
                Ypos -= 1;
            }
            ongroundagain = false;
            uphold = true;
        }
        else {
            uphold = false;
        }
        if (bgravdown) {
            if (collision(Xpos, Ypos + 1)) {
                Ypos++;
            }
            else {
                ongroundagain = true;
            }
            gconstcounter = 0;
        }
        
        //--------------------------------------------------
        //DRAW PLAYER
        //--------------------------------------------------
        //OUTPUT RENDERING
        //*if (blockmove) {
        //    draw_asset(3, 3, asset1X, 14);
        //    remove_asset(3,3,asset1X + 2, 14);     
        //    asset1X--;
        //    blockcounter = 0;
        //*}


        draw_asset(2, 2, 14, 14);
        draw_asset(2, 4, 16, 14);
        draw_asset(2, 6, 18, 14);
        draw_asset(2, 8, 20, 14);
        draw_asset(2, 8, 23, 14);
        draw_asset(2, 8, 28, 14);
        for (int x = 0; x < fieldwidth; x++) {
            for (int y = 0; y < fieldheight; y++) {
                screen[(y + 2)*screenwidth + (x + 2)] = L" YX|"[field[y*fieldwidth + x]];
            }   
        }

        //--------------------------------------------------    
        //DRAW PLAYER
        screen[(Ypos)*screenwidth + (Xpos)] = 'Y';
        WriteConsoleOutputCharacterW(hConsole, screen, screenwidth * screenheight, { 0,0 }, &dwBytesWritten); //FUCK THIS WIN32 BIT SHIT 
    }

    return 0;
}  