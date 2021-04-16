/*
 * toolBox.h
 *
 *  Created on: 15 janv. 2019
 *      Author: clatour
 */

#ifndef TOOLBOX_H_
#define TOOLBOX_H_

#include <stdint.h>
#include "string.h"
#include "main.h"
/***********************************************************************************/

/* Prototype function */
void 		rmv_lastCarStr	(char *, uint8_t);
void 		strcopy			(void *dest, const void *src, uint8_t ofs_dest, uint8_t ofs_src, uint8_t n_towrite);
uint8_t 	IntToStr		(uint8_t *str, int32_t n, e_bool eof);
uint8_t 	DoubleToStr 	(uint8_t *str, double n, uint8_t precision);
uint8_t 	decToBCD		(uint8_t valInDec);
uint8_t 	BCDToDec		(uint8_t valInBCD);
uint8_t 	split_str		(char*, const char*, char);
uint16_t 	StrToInt		(uint8_t*, uint8_t);
double	 	StrToDouble		(uint8_t*);
uint16_t 	ABS				(uint16_t, uint16_t);
double 		MAX				(double, double);

#endif /* TOOLBOX_H_ */
