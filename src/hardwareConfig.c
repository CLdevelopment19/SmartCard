/*
 * hardwareConfig.c
 *
 *  Created on: 22 mars 2021
 *      Author: clement
 */

#include "hardwareConfig.h"
/*******************************************************************************************************/

/* Public variable */
TIM_HandleTypeDef    Tim3_Timeout_handle;
UART_HandleTypeDef   Uart4_DBG_Csl_handle;
/*******************************************************************************************************/

/* Private functions */
static void CPU_CACHE_Enable(void);
/*******************************************************************************************************/

void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}
/*******************************************************************************************************/

e_bool SystemHardwareGlobalConfig(void)
{
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	HAL_Init();

	NVIC_SetPriorityGrouping(0U);

	/* GPIO */
	SystemGPIO_Config();
	/* OSC */
	if(!SystemClock_Config())
	{
		// TO DO ERROR MANAGMENT
		return FALSE;
	}

	/* TIMER3 */
	if(!SystemTimer3_Config())
	{
		// TO DO ERROR MANAGMENT
		return FALSE;
	}

	/* UART4 DBG CSL */
	if(!SystemUart4_Config())
	{
		// TO DO ERROR MANAGMENT
		return FALSE;
	}

	return TRUE;
}
/*******************************************************************************************************/

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  *         The USB clock configuration from PLLSAI:
  *            PLLSAIP                        = 8
  *            PLLSAIN                        = 384
  *            PLLSAIQ                        = 7
  * @param  None
  * @retval None
  */
e_bool SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	RCC_OscInitStruct.PLL.PLLR = 7;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
		//Error_Handler();
		return FALSE;
	}

	/* Activate the OverDrive to reach the 216 MHz Frequency */
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK)
	{
		//Error_Handler();
		return FALSE;
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
	if(ret != HAL_OK)
	{
		//Error_Handler();
		return FALSE;
	}

	return TRUE;
}
/*******************************************************************************************************/

e_bool SystemGPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/* -1- Enable GPIO Clock (to be able to program the configuration registers) */
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/* -2- Configure IO in output push-pull mode to drive external LEDs */
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	GPIO_InitStruct.Pin = GPIO_LED_1 | GPIO_LED_2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	return TRUE;
}
/*******************************************************************************************************/

/* TIM3 is connected to APB1 bus, here APB1 = HCLK/4 */
e_bool SystemTimer3_Config			(void)
{
	uint32_t uwPrescalerValue = 0;
	uwPrescalerValue = (uint32_t)((SystemCoreClock/ 4) / 5400) - 1;		// Set prescaler to set 10000Hz

	/* TIMx Peripheral clock enable */
	__TIM3_CLK_ENABLE();

	Tim3_Timeout_handle.Instance = TIM3;

	/* Initialize TIMx peripheral as follows:
	   + Period = 10000 - 1
	   + Prescaler = ((SystemCoreClock / 2)/10000) - 1
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	Tim3_Timeout_handle.Init.Period            = 10;				// 1ms
	Tim3_Timeout_handle.Init.Prescaler         = uwPrescalerValue;
	Tim3_Timeout_handle.Init.ClockDivision     = 0;
	Tim3_Timeout_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	Tim3_Timeout_handle.Init.RepetitionCounter = 0;
	Tim3_Timeout_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&Tim3_Timeout_handle) != HAL_OK)
	{
		// TO DO
		return FALSE;
	}

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	/* Start Channel1 */
	/*if (HAL_TIM_Base_Start_IT(&Tim3Handle) != HAL_OK)
	{
		// TO DO
		return FALSE;
	}*/
	//HAL_TIM_Base_Start_IT(&Tim3Handle);
	return TRUE;
}
/*******************************************************************************************************/

e_bool SystemUart4_Config			(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;

	GPIO_InitStruct.Pin = GPIO_UART4_TX | GPIO_UART4_RX;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


	/* Select SysClk as source of USART1 clocks */
	/*RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_UART4;
	RCC_PeriphClkInit.Usart1ClockSelection = RCC_UART4CLKSOURCE_SYSCLK;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);*/

	__UART4_CLK_ENABLE();

	// PD1 : UART4_TX
	// PD0 : UART4_RX
	/* Put the USART peripheral in the Asynchronous mode (UART Mode) */
	/* UART4 configured as follow:
	  - Word Length = 8 Bits
	  - Stop Bit = One Stop bit
	  - Parity = None
	  - BaudRate = 230400 baud
	  - Hardware flow control disabled (RTS and CTS signals) */
	Uart4_DBG_Csl_handle.Instance          = UART4;
	Uart4_DBG_Csl_handle.Init.BaudRate     = (uint32_t)BAUDRATE_UART4_DBG;
	Uart4_DBG_Csl_handle.Init.WordLength   = UART_WORDLENGTH_8B;
	Uart4_DBG_Csl_handle.Init.StopBits     = UART_STOPBITS_1;
	Uart4_DBG_Csl_handle.Init.Parity       = UART_PARITY_NONE;
	Uart4_DBG_Csl_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	Uart4_DBG_Csl_handle.Init.Mode         = UART_MODE_TX_RX;
	Uart4_DBG_Csl_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	if(HAL_UART_Init(&Uart4_DBG_Csl_handle) != HAL_OK)
	{
		return FALSE;
	}

	//__HAL_UART_ENABLE_IT(&Uart4_DBG_Csl_handle, UART_IT_IDLE);
	HAL_NVIC_SetPriority(UART4_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(UART4_IRQn);

	return TRUE;
}
