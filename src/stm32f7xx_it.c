/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f7xx_it.h"
#include "hardwareConfig.h"
#include "debug_csl.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// Handlers
static void (*rx_done_handler_csl)(void) 	= 0;
static void (*tx_done_handler_csl)(void) 	= &callbackUart4_console;
static void (*trig_timeout_csl)(void)		= &ConsoleTxTimeout;

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}
/*******************************************************************************************************/

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Tim3_Timeout_handle);
}
/*******************************************************************************************************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t delay_timeout_csl = 0;

	delay_timeout_csl = (delay_timeout_csl +1)%TIMEOUT_TX;
	if(!delay_timeout_csl)
	{
		//GPIO_LED1_TOGGLE;
		(*trig_timeout_csl)();
	}
}
/*******************************************************************************************************/

/* Define in debug_csl file */
__weak void UART4_IRQHandler(void)
{

}
/*******************************************************************************************************/

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance == Uart4_DBG_Csl_handle.Instance)
	{
		(*tx_done_handler_csl)();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle->Instance == Uart4_DBG_Csl_handle.Instance)
	{
		(*rx_done_handler_csl)();
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{

}
