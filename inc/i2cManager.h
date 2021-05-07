/*
 * i2cManager.h
 *
 *  Created on: 23 avr. 2021
 *      Author: clement
 */

#ifndef I2CMANAGER_H_
#define I2CMANAGER_H_

#include "main.h"
#include "hardwareConfig.h"
//*****************************************************************************************************************************************//

#define MAX_ITERATION_FAILED	5
//*****************************************************************************************************************************************//

/* Enumeration */
typedef enum
{
	READY_WRITE = 0,
	READY_READ,
	DATA_AVAILABLE,
	BURST_READ,
	ERR,
	IDLE,
}e_i2c_status;

typedef enum
{
	READ_OP = 0,
	WRITE_OP,
	READWRITE_OP
}e_i2c_operation;

typedef struct
{
	uint16_t 		adr;
	uint8_t			*dta_tx;
	uint8_t			*dta_rx;
	uint8_t 		size_tx;
	uint8_t 		size_rx;
	e_i2c_status 	status;
	e_i2c_operation op;
}s_i2c_object;
//*****************************************************************************************************************************************//

/* Publics functions */
void	LoadI2CObject		(volatile s_i2c_object* object, void (*callback)(uint8_t*));
e_bool 	Manage_i2c2_FSM 	(volatile s_i2c_object* i2c);
//*****************************************************************************************************************************************//

#endif /* I2CMANAGER_H_ */
