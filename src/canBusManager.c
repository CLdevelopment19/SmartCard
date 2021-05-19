/*
 * canBusManager.c
 *
 *  Created on: 7 mai 2021
 *      Author: clement
 */
#include "canBusManager.h"
#include <string.h>

/* Privates variables */
static canMessageTxObject_t canMessageTx;
static canMessageRxObject_t canMessageRx;
/*******************************************************************************************************/

/* ISR CAN1 TX */
void CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CAN1_handle);
}
/*******************************************************************************************************/

/* ISR CAN1 RX FIFO0 */
void CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CAN1_handle);
}
/*******************************************************************************************************/

e_bool InitCanObject 	(void(*DecodeFunctionCan)(canFormat_t*))
{
	InitCircularBuffer(&canMessageTx.docker);
	canMessageTx.buffer_pending = 0;
	canMessageTx.mailbox_free = NUMBER_MAILBOX_AVAILABLE;
	memset(canMessageTx.payload.data, 0, SIZE_ARRAY(canMessageTx.payload.data));

	InitCircularBuffer(&canMessageRx.docker);
	canMessageRx.buffer_pending = 0;
	memset(canMessageRx.payload.data, 0, SIZE_ARRAY(canMessageRx.payload.data));

	canMessageRx.callBackDecodeMsg = DecodeFunctionCan;

	return TRUE;
}
/*******************************************************************************************************/

void PrepareFullTrameStandardCan (canMessageTxObject_t* out, canFormat_t payloadIn)
{
	out->headerFrame.StdId = payloadIn.id;
	out->headerFrame.IDE = CAN_ID_STD;
	out->headerFrame.DLC = payloadIn.dlc;
	out->headerFrame.RTR = CAN_RTR_DATA;
	out->headerFrame.ExtId = 0;
	out->headerFrame.TransmitGlobalTime = DISABLE;
	memcpy(out->payload.data, payloadIn.data, payloadIn.dlc);
}
/*******************************************************************************************************/

e_bool ManageTxMessage	(canFormat_t msg)
{
	if(!canMessageTx.mailbox_free)			//No mailbox available
	{
		// Push message in buffer, transmission will be delayed
		if(WriteCircularBuffer(&canMessageTx.docker, &msg, sizeof(msg)) == FULL)
		{
			// TO DO : If buffer is full => WAIT? OVERWRITE?
		}else{
			canMessageTx.buffer_pending++;
		}
	}else{
		PrepareFullTrameStandardCan(&canMessageTx, msg);
		canMessageTx.mailbox_free--;
		HAL_CAN_AddTxMessage(&CAN1_handle, &canMessageTx.headerFrame, canMessageTx.payload.data, &canMessageTx.mailBox);
	}
	return TRUE;
}
/*******************************************************************************************************/

e_bool ManageRxMessage	()
{
	if(!canMessageRx.buffer_pending)			// No frame received
	{
		if((canMessageTx.buffer_pending != 0) && (canMessageTx.mailbox_free == NUMBER_MAILBOX_AVAILABLE))
		{
			canFormat_t frameTx;
			ReadCircularBuffer(&canMessageTx.docker, &frameTx, sizeof(frameTx));
			PrepareFullTrameStandardCan(&canMessageTx, frameTx);
			canMessageTx.mailbox_free--;
			HAL_CAN_AddTxMessage(&CAN1_handle, &canMessageTx.headerFrame, canMessageTx.payload.data, &canMessageTx.mailBox);
			canMessageTx.buffer_pending--;
		}
	}else
	{
		canFormat_t frameRcv;
		ReadCircularBuffer(&canMessageRx.docker, &frameRcv, sizeof(frameRcv));
		(*(canMessageRx.callBackDecodeMsg))(frameRcv);
		canMessageRx.buffer_pending--;
	}
	return TRUE;
}
/*******************************************************************************************************/

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1_handle.Instance)
	{
		canMessageTx.mailbox_free++;
	}
}
//*****************************************************************************************************************************************//

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1_handle.Instance)
	{
		canMessageTx.mailbox_free++;
	}
}
//*****************************************************************************************************************************************//

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1_handle.Instance)
	{
		canMessageTx.mailbox_free++;
	}
}
//*****************************************************************************************************************************************//

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1_handle.Instance)
	{
		//TO DO : FIFO FULL => emergy !! Need to be flush immediatly
	}
}
//*****************************************************************************************************************************************//

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	// Get RX message
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &(canMessageRx.headerFrame), canMessageRx.payload.data) != HAL_OK)
	{
		// ERROR : TO DO
	}else
	{
		canMessageRx.payload.dlc = canMessageRx.headerFrame.DLC;
		canMessageRx.payload.id = canMessageRx.headerFrame.StdId;
		WriteCircularBuffer(&canMessageRx.docker, &canMessageRx.payload, sizeof(canMessageRx.payload));
		canMessageRx.buffer_pending++;
	}
}
//*****************************************************************************************************************************************//

