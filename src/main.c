/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "main.h"
#include "hardwareConfig.h"
#include "debug_csl.h"
#include "KXTJ3_device.h"
#include "analog_device.h"
#include <string.h>
#include "canBusManager.h"
#include "canOpen.h"
/*******************************************************************************************************/

/* Locales variables */
uint8_t ext_flag_test = 0;
static e_bool KXTJ3_ready = FALSE;
uint8_t test[6] = {0};
canFormat_t CanFrameTest = {.dlc 	= 0x08,
								.id  	= 0x11,
								.data[0]= 0x30,
								.data[1]= 0x2C,
								.data[2]= 0x20,
								.data[3]= 0x00,
								.data[4]= 0x00,
								.data[5]= 0x00,
								.data[6]= 0x0D,
								.data[7]= 0x0A};
/*******************************************************************************************************/

int main(void)
{
	if(!SystemHardwareGlobalConfig())
	{
		GPIO_LED1_OFF;
		GPIO_LED2_OFF;
	}else
	{
		//GPIO_LED1_ON;
		//GPIO_LED2_ON;
	}

	InitCanObject(CanOpenManager);

	InitObjectConsole();
	HAL_TIM_Base_Start_IT(&Tim3_Timeout_handle);

	DEBUG_INFO("**** Smart Card ****\r\n");

	KXTJ3_ready = FALSE;
	do{}while(!KXTJ3_ready);

	Ksxj3_checkConnexion();
	//Ksxj3_checkFunctionality();

	/*KXTJ3_ready = FALSE;
	do{}while(!KXTJ3_ready);*/

	//HAL_I2C_Mem_Read(&I2C2_handle, KXTJ3_ADDRESS+1, 0x06, 1, test, 6, 1000);

	ManageTxMessage(CanFrameTest);
	CanFrameTest.data[0] += 1;
	ManageTxMessage(CanFrameTest);
	CanFrameTest.data[0] += 1;
	ManageTxMessage(CanFrameTest);
	CanFrameTest.data[0] += 1;
	ManageTxMessage(CanFrameTest);
	CanFrameTest.data[0] += 1;
	ManageTxMessage(CanFrameTest);

	ext_flag_test = 0;

	//StartConversionADC(&ADC1_handle);

	while(1){

		ManageRxMessage();
		if(ext_flag_test)
		{
			ext_flag_test = 0;
			//Ksxj3_GetAngle(Y_ANGLE);
			//Ksxj3_GetXYZAxis_burstMode();
		}
	}
}
/*******************************************************************************************************/

void WaitDeviceKXTJ3Ready ()
{
	KXTJ3_ready = TRUE;
}

/*******************************************************************************************************/
