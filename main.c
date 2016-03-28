/* 
 * File:   main.c
 * Author: larsg
 *
 * Created on 19 maart 2016, 16:38
 */
#ifndef __XTAL_FREQ
#define _XTAL_FREQ 32000000
#endif

#include <p18f4520.h>
#include <xc.h>
#include "Header/config.h"
#include "Header/LED-API.h"


unsigned char channel = 12;


void delaySeconds(int numberOfSeconds)
{
    for(int i = 0; i < numberOfSeconds * 100; i++)
        __delay_ms(10);
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
    
    //initialize A/D converter for inuts
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
            on(value, 0); 
            on(value + 1, 0); // draw paddle
        }
        else
        {
            ADCON0bits.CHS = 12; // next refresh will be on channel 12
            rowOff(15); // turn off the paddle's row
            on(value, 15);
            on(value + 1, 15);// draw paddle
        }
    }
}

void main(void) { 
    initializeLED();
    initialize();
    
    //this will work: if there's input, an interrupt is called which
    // will temporarily stop the refreshing.
    while(1){ refresh(); };
}