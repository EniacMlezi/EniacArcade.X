/* 
 * File:   main.c
 * Author: Nick de Visser
 *
 * Created on 19 maart 2016, 16:38
 */

#ifndef __XTAL_FREQ
#define _XTAL_FREQ 32000000
#endif

#include <p18f4520.h>
#include <xc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Header/config.h"
#include "Header/LED-API.h"

//Possible game conditions
typedef enum {scored = '0', normal = '1', gameOver = '2', outOfBounds = "3"} gameConditions;

//Structure of a ball
struct ballData
{
    unsigned char bPos[2];
    //[1-6] 
    unsigned char bDirection;
};

//Structure of a player
struct playerData
{
    //0: lowest 1: highest
    unsigned char paddlePos[2];
    //amount of lives of an player
    unsigned char lives;
};

//Structure instances
struct ballData _ball;
struct playerData _p1;
struct playerData _p2;

//Ball hit (with the paddle) counter
char _hit = 0;
//Game state

//Handles collision based steps (check if it is possible to discard this method)
void handleCollision(void)
{
    //check 
}

char* getNextPosition(void)
{
    char *ballPos = malloc(sizeof(2 * char));
    
    switch(_ball.bDirection)
    {
        case 1:
            ballPos[0] = _ball.bPos[0]++;
            ballPos[1] = _ball.bPos[1]++;
            break;
        case 2:
            ballPos[0] = _ball.bPos[0]++;
            break;
        case 3:
            ballPos[0] = _ball.bPos[0]++;
            ballPos[1] = _ball.bPos[1]--;      
            break;
        case 4:
            ballPos[0] = _ball.bPos[0]--;
            ballPos[1] = _ball.bPos[1]--;
            break;
        case 5:
            ballPos[0] = _ball.bPos[0]--;
            break;
        case 6:
            ballPos[0] = _ball.bPos[0]--;
            ballPos[1] = _ball.bPos[1]++;
            break;
    }
    
    return ballPos;
}

void checkNextPosition(void)
{
    char* nextBallPos = getNextPosition();
    
    if(nextBallPos[1] == 0)
    {
        if(_p1.paddlePos[0] == nextBallPos[0] || _p1.paddlePos[1] == nextBallPos[0])
        {
            //paddle collision
        }
    }
    else if(nextBallPos[1] == 15)
    {
        if(_p2.paddlePos[0] == nextBallPos[0] || _p2.paddlePos[1] == nextBallPos[0])
        {
            //paddle collision
        }
    }
    else if(nextBallPos[0] < 0 || nextBallPos[0] > 7)
    {
        //ball collides with wall
    }
    
    
}

//Initializes structure values
void initializePONG(void)
{    
    //random ball generation
    _ball.bDirection = 2;//should be random
    _ball.bPos[0] = 4;//should be random
    _ball.bPos[1] = 6;//should be random
}

void initialize()
{ 
    // set osc speed to 8MHz and PLLx4 for 32MHz Clock Frequency
    OSCTUNEbits.PLLEN = 1; // Frequency Multiplier PLL for INTOSC Enabled
    OSCTUNEbits.TUN = 011111; // Run at maximum frequency
    
    OSCCONbits.SCS = 00; // set primary oscillator(internal) as System Clock
    OSCCONbits.IRCF = 111; // set Internal Oscillator Frequency Select bits to 8MHz 
    
    PLLEN = 1; // Enable PLLx4 for 32MHz Clock Frequency    
    
    //setup interrupts
    GIE = 1; // global interrupt enabled
    PEIE=1; // peripheral interrupt enabled
    
    //initialize A/D converter for inputs
    TRISBbits.TRISB0 = 1; // set AN12 input
    TRISBbits.TRISB1 = 1; // set AN10 input
    ADCON2bits.ADFM = 1; // set Result Format selection bit to Right Justified
    ADCON2bits.ADCS = 010; // Select Fosc/32 as Clock Time
    ADCON2bits.ACQT = 111; // Set Acquisition Time to 20uS just to be safe !!!
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.PCFG = 0000;
    ADCON0bits.CHS = 1100; // Listen to Channel 12 (Pin33)
    ADCON0bits.ADON = 1; // Enable the converter module
    ADIE = 1; // set A/D Interrupt Enable bit to enabled 
    
    //setup Timer0 for game refresh
    T0PS0 = 1; 
    T0PS1 = 0;
    T0PS2 = 1; // set prescaler to 1:64 for 512us timer when xtal is 1MHz
    PSA = 0; // Timer0 prescaler enabled
    T0CS = 0; // set Timer0 clock to internal instruction cycle clock
    T08BIT = 1; // Timer0 is configured as an 8-bit timer/counter 
    TMR0IE = 1; // set TIMER0 Interrupt Enable bit to enabled   
    TMR0IF = 0; // clear TIMER0 Interrupt Flag bit
    //start the timer
    TMR0ON = 1; // set Timer0 On/Off Control bit to on  
    
}

//Interrupt Service Routine
void interrupt ISR(void)
{
    if(TMR0IE && TMR0IF)
    {
        //TMR0 interrupt: start A/D converted inputs, write new ball coordinates to LED-API
        TMR0IF = 0;
        
        checkNextPosition();
        
        
        //start receiving input
        ADCON0bits.GODONE = 1; //start
        
    }
    
    if(ADIE && ADIF)
    {
        // refresh game, write new paddle coordinates to LED-API
        ADIF = 0;
        
        unsigned short ADvalue = ADRES;//grab the 10 bit value
        unsigned char value = (ADvalue / 85); // divide by 85 for a number between 0 and 6
        
        // select which paddle needs to refresh
        if(ADCON0bits.CHS == 12)
        {       
            ADCON0bits.CHS = 10; //next refresh will be on channel 10   
            rowOff(0); // turn off the paddle's row
            _p1.paddlePos[0] == value;
            _p1.paddlePos[1] == value + 1;
        }
        else
        {
            ADCON0bits.CHS = 12; // next refresh will be on channel 12
            rowOff(15); // turn off the paddle's row
            _p2.paddlePos[0] == value;
            _p2.paddlePos[1] == value + 1;
        }
    }
}

void main(void) 
{
    initializeLED();
    initialize();
    initializePONG();
    
    //LED symbols for displaying for the game start
    const char HELLO_SYMBOL; //{:AY drawn in LEDs
    const char THREE_SYMBOL; //THREE drawn in LEDs
    const char TWO_SYMBOL; //TWO drawn in LEDs
    const char ONE_SYMBOL; //ONE drawn in LEDs
    const char GO_SYMBOL; //GO drawn in LEDs   
    /*Wait for interrupt signal.*/
    while(!RBIF)
    {
        //Display play
        __delay_ms(2000);
        //LEDs off
        __delay_ms(1000);
    }
    //Display 3 -> 2 -> 1 -> GO  
    
    
    //this will work: if there's input, an interrupt is called which
    // will temporarily stop the refreshing.
    while(1){ refresh(); };
}