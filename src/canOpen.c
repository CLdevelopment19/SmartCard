/*
 * canOpen.c
 *
 *  Created on: 19 mai 2021
 *      Author: clement
 */

#include "canOpen.h"
/*******************************************************************************************************/

void CanOpenManager (canFormat_t frame)
{
	canOpenObject objectCan;
	canFormat_t CanFrameAnswer;
	GPIO_LED1_TOGGLE;

	if(frame.id == NMT_ID)			// NMT
	{
		if(frame.data[1] == ID_DEVICE || frame.data[1] == ID_ALL_DEVICE)
		{
			switch(frame.data[0])
			{
			case NMT_FUNCTION_OPERATIONAL:
				break;
			case NMT_FUNCTION_PRE_OPERATIONAL :
				break;
			case NMT_FUNCTION_STOP :
				break;
			case NMT_FUNCTION_RESET_NODE :
				break;
			case NMT_FUNCTION_RESET_COM:
				break;
			default:
				break;
			}
		}
	}else if(frame.id == (ID_DEVICE + 0x600))			// SDO
	{
		objectCan.dlc = frame.dlc;
		if(objectCan.dlc == 8)
		{
			objectCan.command = frame.data[0];
			objectCan.object = frame.data[2];
			objectCan.object =  (objectCan.object<<8) | frame.data[1];
			objectCan.subIndex = frame.data[3];
			objectCan.data[0] = frame.data[4];
			objectCan.data[1] = frame.data[5];
			objectCan.data[2] = frame.data[6];
			objectCan.data[3] = frame.data[7];

			if((objectCan.object == SDO_DEVICE_OBJECT_NAME_CODE) && (objectCan.command == SDO_READ_CODE))
			{
				CanFrameAnswer.id = 0x580 + ID_DEVICE;
				CanFrameAnswer.dlc = 8;
				CanFrameAnswer.data[0] = SDO_READ_RESPONSE_CODE;
				CanFrameAnswer.data[1] = (unsigned char)objectCan.object;
				CanFrameAnswer.data[2] = (unsigned char)(objectCan.object>>8);
				CanFrameAnswer.data[3] = 0x00;
				CanFrameAnswer.data[4] = (unsigned char)(DEVICE_NAME>>24);
				CanFrameAnswer.data[5] = (unsigned char)(DEVICE_NAME>>16);
				CanFrameAnswer.data[6] = (unsigned char)(DEVICE_NAME>>8);
				CanFrameAnswer.data[7] = (unsigned char)(DEVICE_NAME);
				ManageTxMessage(CanFrameAnswer);
			}
			else if((objectCan.object == SDO_DEVICE_OBJECT_HARDWARE_CODE) && (objectCan.command == SDO_READ_CODE))
			{
				CanFrameAnswer.id = 0x580 + ID_DEVICE;
				CanFrameAnswer.dlc = 8;
				CanFrameAnswer.data[0] = SDO_READ_RESPONSE_CODE;
				CanFrameAnswer.data[1] = (unsigned char)objectCan.object;
				CanFrameAnswer.data[2] = (unsigned char)(objectCan.object>>8);
				CanFrameAnswer.data[3] = 0x00;
				CanFrameAnswer.data[4] = (unsigned char)(DEVICE_HARDWARE>>24);
				CanFrameAnswer.data[5] = (unsigned char)(DEVICE_HARDWARE>>16);
				CanFrameAnswer.data[6] = (unsigned char)(DEVICE_HARDWARE>>8);
				CanFrameAnswer.data[7] = (unsigned char)(DEVICE_HARDWARE);
				ManageTxMessage(CanFrameAnswer);
			}
			else if((objectCan.object == SDO_DEVICE_OBJECT_SOFTWARE_CODE) && (objectCan.command == SDO_READ_CODE))
			{
				CanFrameAnswer.id = 0x580 + ID_DEVICE;
				CanFrameAnswer.dlc = 8;
				CanFrameAnswer.data[0] = SDO_READ_RESPONSE_CODE;
				CanFrameAnswer.data[1] = (unsigned char)objectCan.object;
				CanFrameAnswer.data[2] = (unsigned char)(objectCan.object>>8);
				CanFrameAnswer.data[3] = 0x00;
				CanFrameAnswer.data[4] = (unsigned char)(DEVICE_SOFTWARE>>24);
				CanFrameAnswer.data[5] = (unsigned char)(DEVICE_SOFTWARE>>16);
				CanFrameAnswer.data[6] = (unsigned char)(DEVICE_SOFTWARE>>8);
				CanFrameAnswer.data[7] = (unsigned char)(DEVICE_SOFTWARE);
				ManageTxMessage(CanFrameAnswer);
			}
		}
	}else if(frame.id == (ID_DEVICE + 0x180))			// PDO
	{

	}
}
