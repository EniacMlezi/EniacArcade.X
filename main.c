/* 
 * File:   main.c
 * Author: Nick de Visser
 *
 * Created on 19 maart 2016, 16:38
 */

#include <xc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "config.h"


//Possible ball values
enum ballHorizontalDirection {up = '1', down = '0'};
enum ballVerticalDirection {left = '1', right = '0'};
enum ballVelocity {bSlow = '3', bMedium = '4', bHard = '5', bHardest = '6'};

//Possible player values
enum padleSpeed {pStill = '0', pSlow = '1', pMedium = '2', pHard = '3'};
enum padleDirection {pUp = 1, pDown = 0};

//Possible game conditions
typedef enum {scored = '0', normal = '1', gameOver = '2'} gameConditions;

//(16 led dots - 1)
#define MAXX = 15
#define MINX = 0

//(8 led dots -1)
#define MAXY = 7
#define MINY = 0

//Player lives
char _pOneLives = 3;
char _pTwoLives = 3;

//Set pins to receive input
#define TRISBbits.Rb3 = 0xFF //pin 33
#define TRISBbits.Rb2 = 0xFF //pin 34
#define TRISBbits.Rb1 = 0xFF //pin 35
#define TRISBbits.Rb0 = 0xFF //pin 36

//Will be done by the LED.API.c in the final version but is necessary for testing purposes
#define _XTAL_FREQ 1000
#define OSCON = 0x76

/*Drawings the are shown on the led matrix 
 * before the game start*/
#define const PLAY_SYMBOL //PLAY drawn in LEDs
#define const THREE_SYMBOL //THREE drawn in LEDs
#define const TWO_SYMBOL //TWO drawn in LEDs
#define const ONE_SYMBOL //ONE drawn in LEDs
#define const GO_SYMBOL //GO draw in LEDs

//Structure of a player
struct playerData
{
    //0: lowest 1: highest
    unsigned char paddlePos[2];
    //min 0 max 3
    unsigned char pVelocity;
    //up ^ down
    unsigned char pHorizontalDirection;
    //amount of lives of an player
    unsigned char lives;
};

//Structure of a ball
struct ballData
{
    unsigned char bPos[2];
    //min 3 max 6
    unsigned char bVelocity;
    //left ^ right
    unsigned char bHorizontalDirection;
    //up ^ down
    unsigned char bVerticalDirection;
};

//Structure instances
struct ballData _ball;
struct playerData _pOne;
struct playerData _pTwo;

//Ball hit (with the paddle) counter
char _hit = 0;

//Initializes structure values
void init(void)
{    
    
    /*
    _ball.bHorizontalDirection = ;
    _ball.bVerticalDirection = ;
    _ball.bPos = {a, b};
    _ball.bVelocity = 3;
    
    _pOne.pHorizontalDirection = ;
    _pOne.pVelocity = 0;
    _pOne.paddlePos = {,};
    _pOne.lives = _pOneLives;
    
    _pTwo.pHorizontalDirection = ;
    _pTwo.pVelocity = 0;
    _pTwo.paddlePos = {,};
    _pTwo.lives = _pTwoLives;
     */
}

//check if the next step is going to be a collision
gameConditions checkNextPosition()
{
    
}

void goalScored()
{
    
}

//Handles collision based steps (check if it is possible to discard this method)
void handleCollision()
{
    //check 
}

//Preforms a step that changes the position of the ball
void preformStep()
{
    switch(checkNextPosition())
    {
        case 0:
            goalScored();
            break;
        case 1:
            handleCollision();
            break;
        default:
            break;
            //Change _ball Vertical- and Horizontal position
    }
    //change position of _ball
}



//Changes the of a given paddle
void padleChange()
{
    
}

void main(void) 
{
    init();
    /*Wait for interrupt signal.*/
    while(!RBIF)
    {
        //Display play
        __delay_ms(2000);
        //LEDs off
        __delay_ms(1000);
    }
    //Clear flag (I think)
    RBIF = 0;
    //Display 3 -> 2 -> 1 -> GO
    
    while(1)
    {
        //if a button is pressed
        if(RBIF)
            padleChange();
        preformStep();
    }
    
    exit(EXIT_SUCCESS);
}
