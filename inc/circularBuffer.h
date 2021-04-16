/*
 * circularBuffer.h
 *
 *  Created on: 11 janv. 2019
 *      Author: clatour
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include "main.h"

// Defines
#define SIZE_BUFFER 1024
//*****************************************************************************************************************************************//
// Typedef enum
typedef enum
{
	OK = 0,			// Data is available
	NOK,
	FULL,
	EMPTY
}e_err_circular_bf;
//*****************************************************************************************************************************************//

// Typedef structure
typedef struct
{
	volatile uint8_t 	buffer[SIZE_BUFFER];
	volatile uint16_t 	index_read;			// inc after each caractere remove (ie read)
	volatile uint16_t 	index_write;		// inc after each caractere added (ie write)
	e_err_circular_bf 	statut;
}s_circular_bf;
//*****************************************************************************************************************************************//

// Prototype function
void 				InitCircularBuffer	(s_circular_bf*);
e_err_circular_bf 	ReadCircularBuffer	(s_circular_bf*, void*, uint16_t);
e_err_circular_bf 	WriteCircularBuffer	(s_circular_bf*, const void*, uint16_t);

//*****************************************************************************************************************************************//

#endif /* CIRCULARBUFFER_H_ */
