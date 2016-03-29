/* 
 * File:   LED-API.c
 * Author: larsg
 *
 * Created on 20 maart 2016, 9:36
 */

#define COLUMNS 8 // define the amount of columns on the panel
#define ROWS 16 // define the amount of rows on the panel
#define PANELSIZE 8 // define size of panels on the panel

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
    if (row < 0 || row >= ROWS)
        return;
    if (column < 0 || column >= COLUMNS)
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

void drawSymbol(unsigned char chosenSymbol, unsigned char offSetX)
{
    char symbolOne[12][2] = {{2,5},{3,6},{4,7},{4,6},{4,5},{4,4},{4,3},{4,2},{4,1},{4,0},{3,0},{5,0}};
    char symbolTwo[14][2] = {{2,5},{3,6},{4,6},{5,6},{6,5},{6,4},{5,3},{4,2},{3,1},{2,0},{3,0},{4,0},{5,0},{6,0}};
    char symbolThree[12][2] = {{2,6},{3,7},{4,7},{5,6},{5,5},{4,4},{5,3},{5,2},{5,1},{4,0},{3,0},{2,1}};

    char symbolPlay[34][2] = {
    {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{2,6},{3,6},{3,5},{3,4},{2,4}, /*P*/
    {5,6},{5,5},{5,4},{5,3},{5,2},{5,1},{6,1}, /*L*/
    {8,1},{8,2},{8,3},{9,4},{10,4},{11,3},{11,2},{11,1}, /*A*/
    {13,1},{13,2},{13,3},{13,4},{12,5},{11,6},{14,5},{14,6}}; /*Y*/

    char symbolGo[34][2] = {
    {6,6},{5,7},{4,7},{3,7},{2,6},{2,5},{2,4},{2,3},{2,2},{2,1},{3,0},{4,0},{5,0},{6,1},{5,2},{4,2}, /*G*/
    {9,6},{9,5},{9,4},{9,3},{9,2},{9,1},{10,0},{11,0},{12,0},{13,1},{13,2},{13,4},{13,5},{13,6},{12,7},{11,7},{10,7}};/*O*/
    
    
    switch(chosenSymbol)
    {
        //symbolOne
        case 1:
            //draw symbol in with given x position
            for(int i = 0; i < 12; i++)
            {
                on(symbolOne[i + offSetX][0], symbolOne[i][1]);
            }
            break;
        //symbolTwo
        case 2:
            //draw symbol in with given x position
            for(int i = 0; i < 14; i++)
            {
                on(symbolTwo[i + offSetX][0], symbolTwo[i][1]);
            }
            break;
        //symbolThree
        case 3:          
            //draw symbol in with given x position
            for(int i = 0; i < 12; i++)
            {
                on(symbolThree[i + offSetX][0], symbolThree[i][1]);
            }
            break;
        //symbolPlay
        case 4:                
            //draw symbol in with given x position
            turnAllOff();
            for(int i = 0; i < 34; i++)
            {
                on(symbolPlay[i + offSetX][0], symbolPlay[i][1]);
            }
            break;
        //symbolGo
        case 5:         
            //draw symbol in with given x position
            turnAllOff();
            for(int i = 0; i < 34; i++)
            {
                on(symbolGo[i + offSetX][0], symbolGo[i][1]);
            }
            break;
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