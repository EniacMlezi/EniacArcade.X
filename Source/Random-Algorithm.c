/* 
 * File:   LED-API.c
 * Author: Nick de Visser
 *
 * Created on 20 maart 2016, 9:36
 */
#include <stdlib.h>
#include <time.h>
#include <p18f4520.h>
#include <xc.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "Header/config.h"
#include "Header/LED-API.h"

static unsigned char _lcg = 84;
static unsigned char _glfsr = 1;

void rndInit(long seed)
{
	if(seed != 0)
		_lcg = seed;
	else
		_lcg = 100;
	_glfsr = (unsigned char)(_lcg ^ 0x55);
	if(_glfsr == 0)
		_glfsr == 1;
}

//Get a random value between the given interval (maximal: 255 minimal:0)
unsigned char getRandom(char minimum, char maximum)
{
	_lcg = (7 * _lcg + 0x11);

	//When _lcg is even
	if((_lcg & 1) == 1)
	{
		_glfsr = (unsigned char)(((_glfsr ^ 0x87) >> 1) | 0x80);
		char x =  (unsigned char)(_glfsr ^_lcg);
		x &= ((maximum - 1) - minimum);
		x += minimum;
		return x;
        
        //return (unsigned char)(((_glfs ^ _lcg) & ((maximum - 1 ) - minumum))) + minimum;
	}
	//When _lcg is uneven
	else
	{
		_glfsr = (unsigned char)(_glfsr >> 1);
		char x =  (unsigned char)(_glfsr ^_lcg);
		x &= (maximum - minimum - 2);
		x += minimum;
		return x;
        //return (unsigned char)(((_glfs ^ _lcg) & ((maximum - 2 ) - minumum))) + minimum;
	}
}
