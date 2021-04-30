/*
 * hardwareConfig.h
 *
 *  Created on: 22 mars 2021
 *      Author: clement
 */

#ifndef HARDWARECONFIG_H_
#define HARDWARECONFIG_H_

#include "main.h"
/*******************************************************************************************************/

/* Constantes */
/*******************************************************************************************************/

/* Constantes variables */
// LED
#define GPIO_LED_1		GPIO_PIN_8
#define GPIO_LED_2		GPIO_PIN_12
#define GPIO_LED1_ON	HAL_GPIO_WritePin(GPIOD, GPIO_LED_1, 1);
#define GPIO_LED1_OFF	HAL_GPIO_WritePin(GPIOD, GPIO_LED_1, 0);
#define GPIO_LED2_ON	HAL_GPIO_WritePin(GPIOD, GPIO_LED_2, 1);
#define GPIO_LED2_OFF	HAL_GPIO_WritePin(GPIOD, GPIO_LED_2, 0);
#define GPIO_LED1_TOGGLE	HAL_GPIO_TogglePin(GPIOD, GPIO_LED_1)
#define GPIO_LED2_TOGGLE	HAL_GPIO_TogglePin(GPIOD, GPIO_LED_2)
// UART4 Debug (PD0 : RX/ PD1 : TX)
#define GPIO_UART4_RX	GPIO_PIN_0
#define GPIO_UART4_TX	GPIO_PIN_1
// I2C (Adccelerometer KXTJ3-1057)
#define GPIO_I2C2_SCL	GPIO_PIN_10	// PB10
#define GPIO_I2C2_SDA	GPIO_PIN_11	// PB11
#define GPIO_KXTJ3_INT	GPIO_PIN_13	// PE13


/* Public variable */
extern TIM_HandleTypeDef    Tim3_Timeout_handle;
extern UART_HandleTypeDef   Uart4_DBG_Csl_handle;
extern I2C_HandleTypeDef   	I2C2_handle;
/*******************************************************************************************************/

/* Private variables */
/*******************************************************************************************************/

/* Public function */
e_bool SystemHardwareGlobalConfig	(void);
e_bool SystemClock_Config			(void);
e_bool SystemGPIO_Config			(void);
e_bool SystemTimer3_Config			(void);
e_bool SystemUart4_Config			(void);
e_bool SystemI2C2_Config			(void);
/*******************************************************************************************************/

#endif /* HARDWARECONFIG_H_ */
