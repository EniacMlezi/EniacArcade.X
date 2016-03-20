/* 
 * File:   LED-API.c
 * Author: larsg
 *
 * Created on 20 maart 2016, 9:36
 */

#define setColHigh(column) (LATA |= (1 << column))
// if bit on n# 'column' in LAT register is high, modify. else don't modify. 
#define setColLow(column) (LATA = (LATA & (1 << column)) ? LATA ^ (1 << column) : LATA)

#define setRowHigh(row) (LATC |= (1 << row))
// if bit on n# 'column' in LAT register is high, modify. else don't modify.
#define setRowLow(row) (LATC = (LATC & (1 << row)) ? LATC ^ (1 << row) : LATC)

#define COLUMNS 8
#define ROWS 16

#include "LED-API.h"

//array which will hold the columns and rows to pull high on next refresh
unsigned char pixels[COLUMNS][ROWS];

void initializeLED() {
    TRISA = 0b00000000; // configure all pins as output
    TRISC = 0b00000000; // configure all pins as outpur
    LATC = 0b00000000; // write 0 to all pins
    LATA = 0b00000000; // write 0 to all pins

    //write 0 to the pixels array
    for(unsigned short i; i < COLUMNS; i++)
    {
        for (unsigned short j; j < ROWS; j++)
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
    for(unsigned char theRow = 0; theRow < ROWS; theRow++)
    {
        setRowHigh(theRow);
        for (unsigned char theCol = 0; theCol < COLUMNS; theCol++) {
            if (pixels[theCol][theRow] == 1)
                setColHigh(theCol);
            else
                setColLow(theCol);        
        }
        setRowLow(theRow);
    }  
}