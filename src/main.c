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
/*******************************************************************************************************/

/* Locales variables */
uint8_t ext_flag_test = 0;
static e_bool KXTJ3_ready = FALSE;
uint8_t test[6] = {0};
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

	ext_flag_test = 0;

	StartConversionADC(&ADC1_handle);

	while(1){

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
