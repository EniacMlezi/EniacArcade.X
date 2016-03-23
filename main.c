/* 
 * File:   main.c
 * Author: larsg
 *
 * Created on 19 maart 2016, 16:38
 */
#define _XTAL_FREQ 1000000

#include <xc.h>
#include "Header/config.h"
#include "Header/LED-API.h"

void initialize()
{ 
    // set osc speed to 8MHz and x4 pll
    
    GIE = 1; // global interrupt enabled
    PEIE=1; // peripheral interrupt enabled
    
    //initialize A/D converter
    
    
    //setup Timer0 for game refresh
    T0PS0 = 0; 
    T0PS1 = 0;
    T0PS2 = 0; // set prescaler to 1:2 for 500us timer when xtal is 1MHz
    PSA = 0; // Timer0 prescaler enabled
    T0CS = 0; // set Timer0 clock to internal instruction cycle clock
    T08BIT = 1; // Timer0 is configured as an 8-bit timer/counter 
    TMR0IE = 1; // set TIMER0 Interrupt Enable bit to enabled     
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
        
    }
    
    if(ADIE && ADIF)
    {
        // refresh game, write new paddle coordinates to LED-API
        ADIF = 0;
    }
}

void delaySeconds(int numberOfSeconds)
{
    for(int i = 0; i < numberOfSeconds * 100; i++)
        __delay_ms(10);
}

void main(void) { 
    initializeLED();
    initialize();
    
    on(2, 2);
    
    //this will work: if there's input, an interrupt is called which
    // will temporarily stop the refreshing.
    while(1){ refresh(); };
}