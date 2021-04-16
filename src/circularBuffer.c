/*
 * circularBuffer.c
 *
 *  Created on: 11 janv. 2019
 *      Author: clatour
 */

#include <stdint.h>
#include <string.h>
#include "circularBuffer.h"

//*****************************************************************************************************************************************//

void InitCircularBuffer(s_circular_bf* obj)
{
	memset((uint8_t*)obj->buffer, 0, SIZE_BUFFER);
	obj->index_read = 0;
	obj->index_write = 0;
	obj->statut = EMPTY;
}
//*****************************************************************************************************************************************//

e_err_circular_bf ReadCircularBuffer(s_circular_bf* obj, void *dta, uint16_t size)
{
	if(obj->statut != EMPTY)
	{
		// Check if the word is at end of buffer
		if(obj->index_read + size > SIZE_BUFFER)
		{
			for(int i = 0; i <size; i++)
			{
				*((uint8_t*)dta+i) = obj->buffer[obj->index_read++];
				obj->index_read = obj->index_read%SIZE_BUFFER;
			}
		}else
		{
			// Copy data
			memcpy((uint8_t*)dta, (uint8_t*)&obj->buffer[obj->index_read], size);
			// Inc index write
			obj->index_read = (obj->index_read+size)%SIZE_BUFFER;
		}
		// Check statut
		if(obj->index_read == obj->index_write)
			obj->statut = EMPTY;
		else
			obj->statut =  OK;
	}else
		obj->statut = EMPTY;
	return obj->statut;
}
//*****************************************************************************************************************************************//

e_err_circular_bf WriteCircularBuffer(s_circular_bf* obj, const void *dta, uint16_t size)
{
	// Check if space is available
	if(obj->statut != FULL)
	{
		// Check if the word will be write at the end of the buffer
		if(obj->index_write + size > SIZE_BUFFER)
		{
			//return obj->statut = FULL;
			for(int i = 0; i < size; i++)
			{
				obj->buffer[obj->index_write++] = (uint8_t)*((uint8_t*)dta+i);
				obj->index_write = obj->index_write%SIZE_BUFFER;
			}
		}else
		{
			// Copy data
			memcpy((uint8_t*)&obj->buffer[obj->index_write], (uint8_t*)dta, size);
			// Inc index write
			obj->index_write = (obj->index_write+size)%SIZE_BUFFER;
		}
		// Check statut
		if(obj->index_write == obj->index_read)
			obj->statut = FULL;
		else
			obj->statut =  OK;
	}else
	{
		obj->statut = FULL;
	}
	return obj->statut;
}
//*****************************************************************************************************************************************//
