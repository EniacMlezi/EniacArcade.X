/* 
 * File:   main.c
 * Author: Nick de Visser
 *
 * Created on 19 maart 2016, 16:38
 */

#ifndef __XTAL_FREQ
#define _XTAL_FREQ 32000000
#endif

#define DEFAULTSPEED 150
#define MAXSPEED 30 // smaller is faster

#include <p18f4520.h>
#include <xc.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Header/config.h"
#include "Header/LED-API.h"

void handlePaddleLeftCollision(char pixel);
void handlePaddleRightCollision(char pixel);

//Structure of a ball
typedef struct ball {
    //position, bPos[0] = ROW Y bPos[1] = COL X
    unsigned char bPos[2];
    //[1-6] 
    unsigned char bDirection;
} ball;

//Structure of a player

typedef struct player {
    //0: lowest 1: highest
    unsigned char paddlePosOld[2];
    //0: lowest 1: highest
    unsigned char paddlePos[2];
    //amount of lives of a player
    unsigned char lives;
    // collision handler
    void (*handleCollision)(char pixel);
} player;

//Structure instances
ball _ball;
player *_p1;
player *_p2;
unsigned char _nextPos[2];

unsigned int _speed = DEFAULTSPEED;
unsigned int _timerCounter = 0;

void delaySeconds(int numberOfSeconds) {
    for (int i = 0; i < numberOfSeconds * 100; i++)
        __delay_ms(10);
}

//shows player lives and resets ball structure values

void startPong(void) {
    //Display player lives
    turnAllOff();
    drawSymbol(_p1->lives, 0);
    drawSymbol(_p2->lives, 8);
    for (unsigned int counter = 0; counter < 800; counter++) {
        refresh();
    }
    turnAllOff();

    //Seed random algorithm
    //rndInit()
    //random ball generation
    _ball.bDirection = 3; //should be random
    //_ball.bDirection = getRandom(0, 7);
    _ball.bPos[0] = 2; //should be random
    //_ball.bPos[0] = getRandom(7, 10);
    _ball.bPos[1] = 4; //should be random
    //_ball.bPos[1] = getRandom(1, 8);

    _nextPos[0] = _ball.bPos[0];
    _nextPos[1] = _ball.bPos[1];
}

void initializePong(void) {
    //main menu?
    
    //set player lives
    _p1->lives = 3;
    _p2->lives = 3;

    //set collision handlers
    _p1->handleCollision = handlePaddleLeftCollision;
    _p2->handleCollision = handlePaddleRightCollision;
    
    startPong();

    //start the timer
    TMR0ON = 1; // set Timer0 On/Off Control bit to on  
}

void initialize() {
    // set osc speed to 8MHz and PLLx4 for 32MHz Clock Frequency
    OSCTUNEbits.PLLEN = 1; // Frequency Multiplier PLL for INTOSC Enabled
    OSCTUNEbits.TUN = 011111; // Run at maximum frequency

    OSCCONbits.SCS = 00; // set primary oscillator(internal) as System Clock
    OSCCONbits.IRCF = 111; // set Internal Oscillator Frequency Select bits to 8MHz 
    //Get oscillator clock value as seed for the random gen
    PLLEN = 1; // Enable PLLx4 for 32MHz Clock Frequency    

    //setup interrupts
    GIE = 1; // global interrupt enabled
    PEIE = 1; // peripheral interrupt enabled

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
    //stop the timer
    TMR0ON = 0; // set Timer0 On/Off Control bit to off  
}

//Display the new paddle- and led positions

void writePong(void) {
    //write all positions

    //write ball positions
    off(_ball.bPos[1], _ball.bPos[0]);
    _ball.bPos[0] = _nextPos[0];
    _ball.bPos[1] = _nextPos[1];
    on(_ball.bPos[1], _ball.bPos[0]);

    //write paddle positions
    off(_p1->paddlePosOld[0], 0);
    off(_p1->paddlePosOld[1], 0);

    off(_p2->paddlePosOld[0], 15);
    off(_p2->paddlePosOld[1], 15);

    on(_p1->paddlePos[0], 0);
    on(_p1->paddlePos[1], 0);
    on(_p2->paddlePos[0], 15);
    on(_p2->paddlePos[1], 15);
}

//Works (Don't edit!!)

getNextPosition(void) { 
    switch (_ball.bDirection) {
        case 1:
            _nextPos[0] = _ball.bPos[0] + 1;
            _nextPos[1] = _ball.bPos[1] + 1;
            break;
        case 2:
            _nextPos[0] = _ball.bPos[0] + 1;
            break;
        case 3:
            _nextPos[0] = _ball.bPos[0] + 1;
            _nextPos[1] = _ball.bPos[1] - 1;
            break;
        case 4:
            _nextPos[0] = _ball.bPos[0] - 1;
            _nextPos[1] = _ball.bPos[1] - 1;
            break;
        case 5:
            _nextPos[0] = _ball.bPos[0] - 1;
            break;
        case 6:
            _nextPos[0] = _ball.bPos[0] - 1;
            _nextPos[1] = _ball.bPos[1] + 1;
            break;
    }
}

void handleWallCollision(void) {
    switch (_ball.bDirection) {
        case 1:
            _ball.bDirection = 3;
            break;
        case 3:
            _ball.bDirection = 1;
            break;
        case 4:
            _ball.bDirection = 6;
            break;
        case 6:
            _ball.bDirection = 4;
            break;
    }
    getNextPosition();
}

void checkWallCollision(void) {
    if (_nextPos[1] < 0 || _nextPos[1] > 7) {
        handleWallCollision();
    }
}

void handlePaddleLeftCollision(char pixel) {
    //increase game speed
    if (_speed > MAXSPEED)
        _speed = _speed - 10;

    if (pixel) { // top pixel hit
        switch (_ball.bDirection) {
                //process 6, 5, 4. redirect ball
            case 4:
                _ball.bDirection = 1;
                break;
            case 5:
                _ball.bDirection = 3;
                break;
            case 6:
                _ball.bDirection = 3;
                break;
        }
    } else { // bottom pixel hit
        switch (_ball.bDirection) {
                //process 6, 5, 4. redirect ball
            case 4:
                _ball.bDirection = 2;
                break;
            case 5:
                _ball.bDirection = 1;
                break;
            case 6:
                _ball.bDirection = 2;
        }
    }
    getNextPosition();
}

void handlePaddleRightCollision(char pixel) {
    //increase game speed
    if (_speed > MAXSPEED)
        _speed = _speed - 10;

    if (pixel) { // top pixel hit
        switch (_ball.bDirection) {
            case 1:
                _ball.bDirection = 4;
                break;
            case 2:
                _ball.bDirection = 4;
                break;
            case 3:
                _ball.bDirection = 6;
                break;
        }
    } else { // bottom pixel hit
        switch (_ball.bDirection) {
            case 1:
                _ball.bDirection = 4;
                break;
            case 2:
                _ball.bDirection = 6;
                break;
            case 3:
                _ball.bDirection = 5;
                break;
        }
    }
    getNextPosition();
}

bit checkPaddleCollision(player *paddle) {
    //if next ball position is one row in front of paddle row
    if (_nextPos[0] == 1 || _nextPos[0] == 14) {
        int i = 0;
        //if next ball position column is equal to on of the paddle position columns
        if (paddle->paddlePos[0] == _nextPos[1]) {
            //handle paddle collision         
            paddle->handleCollision(0);          
            return 1; // paddle collision occured
        } else if (paddle->paddlePos[1] == _nextPos[1]) {
            //handle paddle collision         
            paddle->handleCollision(1);
            return 1; // paddle collision occured
        }       
        //scored
    }
    return 0; // paddle collision did not occur
}

void checkNextPosition(void) {
    getNextPosition();
    //only check wall collision if a paddle collision did not occur
    if (!checkPaddleCollision(_p1) && !checkPaddleCollision(_p2)) {
        checkWallCollision();
    }      
}

//Interrupt Service Routine

void interrupt ISR(void) {
    if (TMR0IE && TMR0IF) {
        //TMR0 interrupt: start A/D converted inputs, write new ball coordinates to LED-API
        TMR0IF = 0;

        if (_timerCounter > _speed) {
            checkNextPosition();
            _timerCounter = 0;
        }

        _timerCounter++;
        writePong();

        //start receiving input
        ADCON0bits.GODONE = 1; //start
    }

    if (ADIE && ADIF) {
        // refresh game, write new paddle coordinates to LED-API
        ADIF = 0;

        unsigned short ADvalue = ADRES; //grab the 10 bit value
        unsigned char value = (ADvalue / 85); // divide by 85 for a number between 0 and 6

        // select which paddle needs to refresh
        if (ADCON0bits.CHS == 12) {
            ADCON0bits.CHS = 10; //next refresh will be on channel 10    

            //Write the current(old) position to the paddlePosOld variable
            _p2->paddlePosOld[0] = _p2->paddlePos[0];
            _p2->paddlePos[0] = value;
            //Write the current(old) position to the paddlePosOld variable           
            _p2->paddlePosOld[1] = _p2->paddlePos[1];
            _p2->paddlePos[1] = value + 1;
        } else {
            ADCON0bits.CHS = 12; // next refresh will be on channel 12

            //Write the current(old) position to the paddlePosOld variable
            _p1->paddlePosOld[0] = _p1->paddlePos[0];
            _p1->paddlePos[0] = value;
            //Write the current(old) position to the paddlePosOld variable
            _p1->paddlePosOld[1] = _p1->paddlePos[1];
            _p1->paddlePos[1] = value + 1;
        }
    }
}

void main(void) {
    initialize();
    initializeLED();
    initializePong();

    //this will work: if there's input, an interrupt is called which
    // will temporarily stop the refreshing.
    while (1) {
        refresh();
    };
}
