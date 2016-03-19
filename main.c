/* 
 * File:   main.c
 * Author: Lars Gardien
 * Co-author: Nick de Visser
 *
 * Created on 19 maart 2016, 16:38
 */
#define _XTAL_FREQ 1000000

#include <xc.h>
#include "config.h"

/*
 * 
 */
void delaySeconds(int numberOfSeconds);
void on(unsigned char row, unsigned char column);
void pongAlgorithm(void);


void main(void) {
    TRISA = 0b00000000;
    TRISC = 0b00000000;
    
    LATA = 0b11111111;
    LATC = 0b00000001;
    for(int i = 0; i <= 7; i++)
    {
        delaySeconds(5);
        LATC = LATC << 1;
    } 
}

void delaySeconds(int numberOfSeconds)
{
    for(int i = 0; i < numberOfSeconds * 100; i++)
        __delay_ms(10);
}

void on(unsigned char row, unsigned char column) 
{
    
}

void off(unsigned char row, unsigned char column) 
{
    
}

void pongAlgorithm(void)
{
    
}