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

//Possible game conditions
typedef enum {scored = '0', normal = '1', gameOver = '2'} gameConditions;

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



//Structure of a player
struct playerData
{
    //[0]: lowest [1]: highest
    unsigned char paddlePos[2];
    //min 0 max 3
    unsigned char pVelocity;
    //1 = up; 0 = down
    unsigned bit pHorizontalDirection;
    //amount of lives of an player
    unsigned char lives;
};

//Structure of a ball
struct ballData
{
    unsigned char bPos[2];
    //min 3 max 6
    unsigned char bVelocity;
    //1 = up; 0 = down
    unsigned bit bHorizontalDirection;
    //1 = up; 0 = down
    unsigned bit bVerticalDirection;
};

//Structure instances
struct ballData _ball;
struct playerData _pOne;
struct playerData _pTwo;

//Ball hit (with the paddle) counter
char _hit = 0;

short getRandom(short max)
{
    unsigned short temp = ((max >> 0) ^ (max >> 2) ^ (max >> 3) ^ (max >> 4) & 1;
    max = (max >> 1) | (temp << 7);
    return max;
}

//Initializes structure values
void init(void)
{    
    
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
     
}

//check if the next step is going to be a collision
gameConditions checkNextPosition(void)
{
    
}

void goalScored(void)
{
    
}

//Handles collision based steps (check if it is possible to discard this method)
void handleCollision(void)
{
    //check 
}

//Preforms a step that changes the position of the ball
void preformStep(void)
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
void padleChange(void)
{
    
}


void main(void) 
{
    //LED symbols for displaying for the game start
    const char HELLO_SYMBOL; //{:AY drawn in LEDs
    const char THREE_SYMBOL; //THREE drawn in LEDs
    const char TWO_SYMBOL; //TWO drawn in LEDs
    const char ONE_SYMBOL; //ONE drawn in LEDs
    const char GO_SYMBOL; //GO drawn in LEDs
    
    
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
