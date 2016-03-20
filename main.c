/* 
 * File:   main.c
 * Co-author: Nick de Visser
 *
 * Created on 19 maart 2016, 16:38
 */

#include <xc.h>
#include <time.h>
#include <stdlib.h>
#include "config.h"

struct playerData
{
    unsigned char pY[2];
    unsigned char lives;
};

struct ballData
{
    unsigned char pos[2];
    unsigned int v;
    unsigned char dir;
    unsigned char angle;
};

//Led Matrix values
char rows[8] = {9, 14, 8, 12, 1, 7, 2, 5};
char cols[8] = {13, 3, 4, 10, 6, 11, 15, 16};
char pins[16] = {5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6};
char screen[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile char screenRow = 0;
volatile char screenCol = 0;

struct ballData _ball;
struct playerData _pOne;
struct playerData _pTwo;


void reset(void)
{   
    srand(time(NULL));
    //check if random is right
    _ball.pos = {(rand() % 2 + 8), (rand() % 9 + 1)};
    _ball.v = 3;
    _ball.angle = ((rand() % 4 + 1) * 45);
    _ball.dir = rand() % 2 + 1;
    
    _pOne.pY = _pTwo.pY = 4;
}

int getSpeed(int colisionAngle, int previousSpeed)
{
    
}

void calcPath(void)
{
    
}
void checkColision(char *ballPos[])
{
    
}

void buttonInput(char signal)
{
    
}
void playSound(char *file[])
{
    
}
void init(void)
{
    
}
void resetGame(void)
{
    
}

void main(void) 
{
    
}
