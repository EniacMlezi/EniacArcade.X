/* 
 * File:   LED-API.c
 * Author: larsg
 *
 * Created on 20 maart 2016, 9:36
 */

#define COLUMNS 8
#define ROWS 8

#define _XTAL_FREQ 1000000

#include "LED-API.h"


//array which will hold the columns and rows to pull high on next refresh
unsigned char pixels[COLUMNS][ROWS];

void setColHigh(unsigned char column)
{
    LATA |= (1 << column);
}
void setColLow(unsigned char column)
{
    if(LATA & (1 << column))
        LATA &= ~(1 << column);   
}

void setRowHigh(unsigned char row)
{
    LATC |= (1 << row);
}
void setRowLow(unsigned char row)
{
   if(LATC & (1 << row))
        LATC &= ~(1 << row);   
}

void allOff()
{
    LATA = 0;
    LATC = 0;
}

void initializeLED() {
    TRISA = 0b00000000; // configure all pins as output
    TRISC = 0b00000000; // configure all pins as outpur
    LATC = 0b00000000; // write 0 to all pins
    LATA = 0b00000000; // write 0 to all pins

    //write 0 to the pixels array
    for(unsigned char i; i < COLUMNS; i++)
    {
        for (unsigned char j; j < ROWS; j++)
        {
            pixels[i][j] = 0;
        }
    }
}

void on(unsigned char row, unsigned char column) {
    //check out of bounds
    if (row < 0 || row > ROWS)
        return;
    if (column < 0 || column > COLUMNS)
        return;
    
    pixels[column][row] = 1;
}

void off(unsigned char row, unsigned char column) {
    //check out of bounds
    if (row < 0 || row > ROWS)
        return;
    if (column < 0 || column > COLUMNS)
        return;

    pixels[column][row] = 0;
}

void refresh() {
    while(1)
    {
        for(unsigned char theCol = 0; theCol < COLUMNS; theCol++)
        {     
            allOff();
            for (unsigned char theRow = 0; theRow < ROWS; theRow++) {
            if (pixels[theCol][theRow] == 1)
                setRowHigh(theRow);     
            }         
            setColHigh(theCol);
            __delay_ms(1);
        }
    }
}