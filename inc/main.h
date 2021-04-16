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
// UART SPEED CONFIGURATION
#define BAUDRATE_UART4_DBG	230400U
/*******************************************************************************************************/

/* Public variables */
typedef enum{
	FALSE = 0,
	TRUE
}e_bool;
/*******************************************************************************************************/

extern uint8_t test_str[32];
/* Private variables */
/*******************************************************************************************************/

/* Public functions */
/*******************************************************************************************************/



#endif /* MAIN_H_ */
