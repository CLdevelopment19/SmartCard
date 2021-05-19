/*
 * canBusManager.h
 *
 *  Created on: 7 mai 2021
 *      Author: clement
 */

#ifndef CANBUSMANAGER_H_
#define CANBUSMANAGER_H_

#include "main.h"
#include "hardwareConfig.h"
#include "circularBuffer.h"
/*******************************************************************************************************/

#define SIZE_PAYLOAD_DATA_FRAME		8
#define NUMBER_MAILBOX_AVAILABLE	3
/*******************************************************************************************************/

typedef struct
{
	uint16_t 	id;
	uint8_t		dlc;
	uint8_t		data[SIZE_PAYLOAD_DATA_FRAME];
}canFormat_t;			// Size 2 bytes + 1 bytes + 8 bytes = 11 bytes (88 bits)

typedef struct
{
	CAN_TxHeaderTypeDef	headerFrame;		// Size 21 bytes
	canFormat_t			payload;
	s_circular_bf		docker;
	uint16_t			buffer_pending;
	volatile uint8_t	mailbox_free;		// 3 mailbox are available with stm32f7
	uint32_t			mailBox;
}canMessageTxObject_t;

typedef struct
{
	CAN_RxHeaderTypeDef headerFrame;		// Size 28 bytes
	canFormat_t			payload;
	s_circular_bf		docker;
	uint16_t			buffer_pending;
	void (*callBackDecodeMsg)(canFormat_t);
}canMessageRxObject_t;
/*******************************************************************************************************/

e_bool InitCanObject 	();
void PrepareFullTrameStandardCan (canMessageTxObject_t*, canFormat_t);
e_bool ManageTxMessage	(canFormat_t);
e_bool ManageRxMessage	();
/*******************************************************************************************************/

#endif /* CANBUSMANAGER_H_ */
