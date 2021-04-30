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
#include <string.h>
/*******************************************************************************************************/

/* Locales variables */
uint8_t ext_flag_test = 0;
static e_bool KXTJ3_ready = FALSE;
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

	/*while(HAL_I2C_Master_Transmit(&I2C2_Accelerometer_handle, (uint16_t)KXTJ3_ADDRESS, (uint8_t*)0x35, 1, 10000)!= HAL_OK)
	{
		if (HAL_I2C_GetError(&I2C2_Accelerometer_handle) != HAL_I2C_ERROR_AF)
		{

		}
	}*/

	/*while(HAL_I2C_Master_Transmit_IT(&I2C2_handle, (uint16_t)0x1E, (uint8_t*)&aTxBuffer, 1)!= HAL_OK)
	{
		if (HAL_I2C_GetError(&I2C2_handle) != HAL_I2C_ERROR_AF)
		{

		}
	}

	while(HAL_I2C_Master_Receive_IT(&I2C2_handle, (uint16_t)0x1F, &aRxBuffer, 1) != HAL_OK)
	  {

	    if (HAL_I2C_GetError(&I2C2_handle) != HAL_I2C_ERROR_AF)
	    {

	    }
	  }*/

	ext_flag_test = 0;

	while(1){

		if(ext_flag_test)
		{
			ext_flag_test = 0;
			Ksxj3_GetAngle(Y_ANGLE);
			//Ksxj3_GetZAxis();
		}
	}
}
/*******************************************************************************************************/

void WaitDeviceKXTJ3Ready ()
{
	KXTJ3_ready = TRUE;
}

/*******************************************************************************************************/
