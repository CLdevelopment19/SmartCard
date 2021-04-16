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
#include <string.h>
/*******************************************************************************************************/

/* Locales variables */

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

	DEBUG_ERROR("Test\r\n");
	DEBUG_LOG("Test\r\n");
	DEBUG_INFO("Test\r\n");
	DEBUG_ERROR("END Test\r\n");

	while(1){

	}
}
/*******************************************************************************************************/


/*******************************************************************************************************/
