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
    GIE = 1; // gloval interrupt enabled
    PEIE=1; // peripheral interropt enabled
    
    //setup Timer0 for LED refresh
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
    if (TMR0IE && TMR0IF)
    {
        TMR0IF=0;
        refresh();
        TMR0ON = 1;
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
    
    delaySeconds(3);
    
    off(2, 2);
    on(3, 3);
    
    while(1);
}