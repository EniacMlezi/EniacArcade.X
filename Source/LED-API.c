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

#define setBit(x, n) (x[n/8] |= (1 << (n%8))
#define clrBit(x, n) (x[n/8] &= ~(1 << (n%8)))
#define getBit(x, n) ( (x[n/8] & (1 << (n%8) )) != 0)

#include "LED-API.h"

unsigned char columns[1];
unsigned char rows[1];

void initializeLED() {
    TRISA = 0b00000000;
    TRISC = 0b00000000;
    LATC = 0b00000000;
    LATA = 0b00000000;

    //TODO: for loop when implementing 16 : 8
    columns[0] = 0;
    rows[0] = 0;
    
}

void on(unsigned char row, unsigned char column) {
    if (row < 0 || row > 7)
        return;
    if (column < 0 || column > 7)
        return;

    setBit(pixels[0], column);
    setBit(rows, row);
}

void off(unsigned char row, unsigned char column) {
    if (row < 0 || row > 7)
        return;
    if (column < 0 || column > 7)
        return;

    clrBit(columns, column);
    clrBit(rows, row);
}

void refresh() {   
    for(unsigned char theRow = 0; theRow < 8; theRow++)
    {
        setRowHigh(theRow);
        for (unsigned char theCol = 0; theCol < 8; theCol++) {
            if (getBit(columns, theCol) && getBit(rows, theRow))
                setColHigh(theCol);
            else
                setColLow(theCol);          
        }
        setRowLow(theRow);
    }
    
}