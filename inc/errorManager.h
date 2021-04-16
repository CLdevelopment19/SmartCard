/*
 * errorManager.h
 *
 *  Created on: 22 mars 2021
 *      Author: clement
 */

#ifndef ERRORMANAGER_H_
#define ERRORMANAGER_H_
/*******************************************************************************************************/

#include "main.h"
/*******************************************************************************************************/

typedef enum
{
	HARDWARE_PERIPH = 0,
	SOFTWARE
}e_KindOfErr;
/*******************************************************************************************************/

typedef struct
{
	e_KindOfErr kindof;
	void (*CallBack)(void);
}errorObj_t;
/*******************************************************************************************************/

#endif /* ERRORMANAGER_H_ */
