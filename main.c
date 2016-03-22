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
    
}

//Interrupt Service Routine
void interrupt ISR(void)
{
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