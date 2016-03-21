/* 
 * File:   LED-API.c
 * Author: larsg
 *
 * Created on 20 maart 2016, 9:36
 */

#define COLUMNS 8 // define the amount of columns on the panel
#define ROWS 16 // define the amount of rows on the panel

#define _XTAL_FREQ 1000000

#include "LED-API.h"


//array which will hold the columns and rows to pull high on next refresh
unsigned char pixels[COLUMNS][ROWS];

void setColHigh(unsigned char column) {
    LATA |= (1 << column); // shift 1 by the column you want to pull high. Apply an or gate to it and the LATA register.
}

void setRowHigh(unsigned char row) {
    LATC |= (1 << row); // shift 1 by the row you want to pull high. Apply an or gate to it and the LATA register.
}

void initializeLED() {
    TRISA = 0b00000000; // configure all pins as output
    TRISC = 0b00000000; // configure all pins as outpur
    LATC = 0b00000000; // write 0 to all pins
    LATA = 0b00000000; // write 0 to all pins

    //write 0 to the pixels array
    for (unsigned char i; i < COLUMNS; i++) {
        for (unsigned char j; j < ROWS; j++) {
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

void off(unsigned char column, unsigned char row) {
    //check out of bounds
    if (row < 0 || row > ROWS)
        return;
    if (column < 0 || column > COLUMNS)
        return;

    pixels[column][row] = 0;
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