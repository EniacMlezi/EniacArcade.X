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
#include "Header/config.h"
#include "Header/LED-API.h"

//Structure of a ball
struct ballData
{
    //position, bPos[0] = ROW Y bPos[1] = COL X
    unsigned char bPos[2];
    //[1-6] 
    unsigned char bDirection;
};

//Structure of a player
struct playerData
{  
    //0: lowest 1: highest
    unsigned char paddlePosOld[2];
    //0: lowest 1: highest
    unsigned char paddlePos[2];
    //amount of lives of an player
    unsigned char lives;
};

//Structure instances
struct ballData _ball;
struct playerData _p1;
struct playerData _p2;
unsigned char _nextPos[2];

unsigned int _speed = 200;
unsigned int _timerCounter = 0;

bit delaySeconds(int numberOfSeconds)
{
    for(int i = 0; i < numberOfSeconds * 100; i++)
        __delay_ms(10);
    
    return 1;
}

//shows player lives and resets ball structure values
void startPong(void)
{    
    //Display player lives
    turnAllOff();
    drawSymbol(_p1.lives, 0);
    drawSymbol(_p2.lives, 8);
    while(!delaySeconds(2))
    {
        refresh();
    }
    turnAllOff();
    
    //3, 2, 1, GO
    
    //Seed random algorithm
   //rndInit()
    //random ball generation
    _ball.bDirection = 3;//should be random
    //_ball.bDirection = getRandom(0, 7);
    _ball.bPos[0] = 2;//should be random
    //_ball.bPos[0] = getRandom(7, 10);
    _ball.bPos[1] = 4;//should be random
    //_ball.bPos[1] = getRandom(1, 8);
    
    _nextPos[0] = _ball.bPos[0];
    _nextPos[1] = _ball.bPos[1];
}


void initializePong(void)
{
    //main menu?
    
    //set player lives
    _p1.lives = 3;
    _p2.lives = 3;
    
    startPong();
    
    //start the timer
    TMR0ON = 1; // set Timer0 On/Off Control bit to on  
}

void initialize()
{ 
    // set osc speed to 8MHz and PLLx4 for 32MHz Clock Frequency
    OSCTUNEbits.PLLEN = 1; // Frequency Multiplier PLL for INTOSC Enabled
    OSCTUNEbits.TUN = 011111; // Run at maximum frequency
    
    OSCCONbits.SCS = 00; // set primary oscillator(internal) as System Clock
    OSCCONbits.IRCF = 111; // set Internal Oscillator Frequency Select bits to 8MHz 
    //Get oscillator clock value as seed for the random gen
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
    //stop the timer
    TMR0ON = 0; // set Timer0 On/Off Control bit to off  
}

//Display the new paddle- and led positions
void writePong(void)
{  
    //write all positions
    
    //write ball positions
    off(_ball.bPos[1], _ball.bPos[0]);
    _ball.bPos[0] = _nextPos[0];
    _ball.bPos[1] = _nextPos[1];
    on(_ball.bPos[1], _ball.bPos[0]);
    
    //write paddle positions
    off(_p1.paddlePosOld[0], 0);
    off(_p1.paddlePosOld[1], 0);
 
    off(_p2.paddlePosOld[0], 15);
    off(_p2.paddlePosOld[1], 15);
    
    on(_p1.paddlePos[0], 0);
    on(_p1.paddlePos[1], 0);
    on(_p2.paddlePos[0], 15);
    on(_p2.paddlePos[1], 15);
    
}

//Works (Don't edit!!)
void getNextPosition(void)
{   
    switch(_ball.bDirection)
    {
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

void checkNextPosition(void)
{
    getNextPosition();
    if(_nextPos[0] == 0)
    {
        if(_p1.paddlePos[0] == _nextPos[1])
        {
            //paddle collision           
            switch(_ball.bDirection)
            {
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
            getNextPosition();
            return;
        }
        else if(_p1.paddlePos[1] == _nextPos[1])
        {
            //paddle collision
            switch(_ball.bDirection)
            {
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
            getNextPosition();
            return;
        }
        //scored
        
        _p1.lives--;
         if(_p1.lives == 0)
        {
            //drawsymbol
            drawSymbol(7, 8);
            
            //restart
            initializePong();
        }
        else
        {
            startPong();
        }
    }
    else if(_nextPos[0] == 15)
    {
        if(_p2.paddlePos[0] == _nextPos[1])
        {
           switch(_ball.bDirection)
            {
                //process 6, 5, 4. redirect ball
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
           getNextPosition();
           return;
        }
        else if( _p2.paddlePos[1] == _nextPos[1])
        {
            //paddle collision
            switch(_ball.bDirection)
            {
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
            getNextPosition();
            return;
        }
        //scored       
        _p2.lives--;
        if(_p2.lives == 0)
        {
            //drawsymbol
            drawSymbol(6, 0);
            
            //restart
            initializePong();
        }
        else
        {
            startPong();
        }
    }
    else if(_nextPos[1] < 0 || _nextPos[1] > 7)
    {
        switch(_ball.bDirection)
        {
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
        return;
    }
}

//Interrupt Service Routine
void interrupt ISR(void)
{
    if(TMR0IE && TMR0IF)
    {
        //TMR0 interrupt: start A/D converted inputs, write new ball coordinates to LED-API
        TMR0IF = 0;
        
        if(_timerCounter > _speed)
        {
            checkNextPosition();
            _timerCounter = 0;       
        }
        
        _timerCounter++;
        writePong();
        
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
            
            //Write the current(old) position to the paddlePosOld variable
            _p1.paddlePosOld[0] = _p1.paddlePos[0];
            _p1.paddlePos[0] = value;
             //Write the current(old) position to the paddlePosOld variable           
            _p1.paddlePosOld[1] = _p1.paddlePos[1];
            _p1.paddlePos[1] = value + 1;
        }
        else
        {
            ADCON0bits.CHS = 12; // next refresh will be on channel 12
            
            //Write the current(old) position to the paddlePosOld variable
            _p2.paddlePosOld[0] = _p2.paddlePos[0];
            _p2.paddlePos[0] = value;
            //Write the current(old) position to the paddlePosOld variable
            _p2.paddlePosOld[1] = _p2.paddlePos[1];
            _p2.paddlePos[1] = value + 1;
        }
    }
}

void main(void) 
{
    initialize();
    initializeLED();
    initializePong();
    
    //this will work: if there's input, an interrupt is called which
    // will temporarily stop the refreshing.
    while(1){ refresh(); };
}