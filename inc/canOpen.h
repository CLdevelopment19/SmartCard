/*
 * canOpen.h
 *
 *  Created on: 19 mai 2021
 *      Author: clement
 */

#include "canBusManager.h"
/*******************************************************************************************************/

#ifndef CANOPEN_H_
#define CANOPEN_H_

#define ID_DEVICE				0x2C
#define ID_ALL_DEVICE			0x00
#define DEVICE_NAME				0x47433630		// GC60
#define DEVICE_HARDWARE			0x01000000		// Version hard 1
#define DEVICE_SOFTWARE			0x01000000		// Version soft 1

#define VENDOR_ID				0x47454F54		// GEOT

#define SDO_READ_CODE			0x40
#define SDO_WRITE_CODE			0x22
#define SDO_ABORD_CODE			0x80
#define SDO_READ_RESPONSE_CODE	0x42
#define SDO_WRITE_RESPONSE_CODE	0x60

#define NMT_ID							0x00
#define NMT_FUNCTION_OPERATIONAL		0x01
#define NMT_FUNCTION_PRE_OPERATIONAL	0x80
#define NMT_FUNCTION_STOP				0x02
#define NMT_FUNCTION_RESET_NODE			0x81
#define NMT_FUNCTION_RESET_COM			0x82

#define SDO_DEVICE_OBJECT_NAME_CODE		0x1008
#define SDO_DEVICE_OBJECT_HARDWARE_CODE	0x1009
#define SDO_DEVICE_OBJECT_SOFTWARE_CODE	0x100A

/*******************************************************************************************************/

typedef struct
{
	unsigned char 	command;
	unsigned char 	subIndex;
	unsigned char 	data[4];
	unsigned char 	dlc;
	unsigned int	object;
}canOpenObject;
/*******************************************************************************************************/

void CanOpenManager (canFormat_t frame);
/*******************************************************************************************************/


#endif /* CANOPEN_H_ */
