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

void delaySeconds(int numberOfSeconds)
{
    for(int i = 0; i < numberOfSeconds * 100; i++)
        __delay_ms(10);
}

void main(void) { 
    initializeLED();
   
    on(5, 6);
    on(3, 2);
    while(1)
    {
        refresh();
    }
}


