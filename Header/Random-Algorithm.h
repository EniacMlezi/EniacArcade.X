/* 
 * File:   RandomAlgorithm.h
 * Author: nickd
 *
 * Created on March 28, 2016, 10:33 PM
 */

#ifndef RANDOMALGORITHM_H
#define	RANDOMALGORITHM_H
#include <xc.h>

extern void rndInit(unsigned long seed);
extern char getRandom(unsigned char minimum, unsigned char maximum);

#endif	/* RANDOMALGORITHM_H */
