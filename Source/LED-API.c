/* 
 * File:   LED-API.c
 * Author: larsg
 *
 * Created on 20 maart 2016, 9:36
 */

#define COLUMNS 8 // define the amount of columns on the panel
#define ROWS 16 // define the amount of rows on the panel
#define PANELSIZE 8

#define _XTAL_FREQ 32000000

#include "LED-API.h"


//array which will hold the columns and rows to pull high on next refresh
unsigned char pixels[COLUMNS][ROWS];

void setColHigh(unsigned char column) {
    LATA |= (1 << column); // shift 1 by the column you want to pull high. Apply an or gate to it and the LATA register.
}

void setRowHigh(unsigned char row) {
    if(row < PANELSIZE)
        LATC |= (1 << row); // shift 1 by the row you want to pull high. Apply an or gate to it and the LATA register.
    else
        LATD |= (1 << (row - PANELSIZE));
}

void initializeLED() {
    TRISA = 0b00000000; // configure all pins as output
    TRISC = 0b00000000; // configure all pins as output
    TRISD = 0b00000000; // configure all pins as output  
    LATA = 0b00000000; // write 0 to all pins
    LATC = 0b00000000; // write 0 to all pins
    LATD = 0b00000000; // write 0 to all pins

    //write 0 to the pixels array
    for (unsigned char i = 0; i < COLUMNS; i++) {
        for (unsigned char j = 0; j < ROWS; j++) {
            pixels[i][j] = 0;
        }
    }
}

void on(unsigned char column, unsigned char row) {
    //check out of bounds
    if (row < 0 || row > ROWS)
        return;
    if (column < 0 || column > COLUMNS)
        return;
    
    pixels[column][row] = 1;
}

void rowOn(unsigned char row)
{
    if (row < 0 || row > ROWS)
        return;
    
    for(unsigned char i = 0; i < COLUMNS; i++)
    {
        pixels[i][row] = 1;
    }
}

void off(unsigned char column, unsigned char row) {
    //check out of bounds
    if (row < 0 || row > ROWS)
        return;
    if (column < 0 || column > COLUMNS)
        return;

    pixels[column][row] = 0;
}

void rowOff(unsigned char row)
{
    if (row < 0 || row > ROWS)
        return;
    
    for(unsigned char i = 0; i < COLUMNS; i++)
    {
        pixels[i][row] = 0;
    }
}

void turnAllOff()
{
    //write 0 to the pixels array
    for (unsigned char i = 0; i < COLUMNS; i++) {
        for (unsigned char j = 0; j < ROWS; j++) {
            pixels[i][j] = 0;
        }
    }
}

void allOff() {
    LATA = 0; // write 0 to LATA
    LATC = 0; // write 0 to LATC
    LATD = 0; // write 0 to LATD
}

void refresh() {
    //iterate all columns
    for (unsigned char theCol = 0; theCol < COLUMNS; theCol++) {
        //iterate all rows
        for (unsigned char theRow = 0; theRow < ROWS; theRow++) {
            //if the column row combination should be high, pull the row high
            if (pixels[theCol][theRow] == 1) 
                setRowHigh(theRow);          
        }
        // when all rows for theCol are pulled high, pull the entire column
        // high for 500us. giving all LEDs in this column the same uptime.
        // afterwards turn of all rows and columns
        setColHigh(theCol);
        __delay_us(500);
        allOff();
    }
}