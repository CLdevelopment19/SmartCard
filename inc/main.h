/*
 * main.h
 *
 *  Created on: 22 mars 2021
 *      Author: clement
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include "stm32f7xx.h"

/* Constantes */
#define SIZE_ARRAY(tab)						(sizeof(tab)/sizeof(tab[0]))

// UART SPEED CONFIGURATION
#define BAUDRATE_UART4_DBG	230400U
/*******************************************************************************************************/

/* Public variables */
typedef enum{
	FALSE = 0,
	TRUE
}e_bool;

/* Device I2C2 */
typedef enum
{
	NONE = 0,
	KSXJ3 = 1
}e_I2C2_device;
/*******************************************************************************************************/

extern uint8_t test_str[32];
extern uint8_t ext_flag_test;

/* Private variables */
/*******************************************************************************************************/

/* Public functions */
void WaitDeviceKXTJ3Ready ();
/*******************************************************************************************************/



#endif /* MAIN_H_ */
