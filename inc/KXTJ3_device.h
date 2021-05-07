/*
 * KXTJ3_device.h
 *
 *  Created on: 23 avr. 2021
 *      Author: clement
 */

#ifndef KXTJ3_DEVICE_H_
#define KXTJ3_DEVICE_H_

#include "i2cManager.h"
//*****************************************************************************************************************************************//

#define DELAY_BOOT_REQUIRED_KXTJ3	1000		// 1s (TIM3 used)
#define PI 3.1416

// Slave address device (Write)
#define KXTJ3_ADDRESS		0x1E
// RO Register
#define KXTJ3_XOUT_L		0x06
#define KXTJ3_XOUT_H		0x07
#define KXTJ3_YOUT_L		0x08
#define KXTJ3_YOUT_H		0x09
#define KXTJ3_ZOUT_L		0x0A
#define KXTJ3_ZOUT_H		0x0B
#define KXTJ3_DCSP_RESP		0x0C	/* This register can be used to verify proper integrated circuit functionality. It always has a byte value of
									 * 0x55h unless the DCST bit in CTRL_REG2 is set. At that point this value is set to 0xAAh. The byte value
									 * is returned to 0x55h after reading this register */
#define KXTJ3_WHO_AM_I		0x0F	/* This register can be used for supplier recognition, as it can be factory written to a known byte value. The
									 * default value is 0x35h. */
#define KXTJ3_INT_SOURCE1 	0x16
#define KXTJ3_INT_SOURCE2 	0x17
#define KXTJ3_STATUS_REG 	0x18	/* This register reports the status of the interrupt
#define KXTJ3_INT_REL 		0x1A	 * Latched interrupt source information (INT_SOURCE1, 0x16h and INT_SOURCE2, 0x17h) is cleared and
									 * physical interrupt latched pin (7) is changed to its inactive state when this register is read. */
#define KXTJ3_SELF_TEST		0x3A

// RW Register
#define KXTJ3_CTRL_REG1			0x1B
#define KXTJ3_CTRL_REG2			0x1D
#define KXTJ3_INT_CTRL_REG1		0x1E
#define KXTJ3_INT_CTRL_REG2		0x1F
#define KXT3J_DATA_CTRL_REG		0x21
#define KXT3J_WAKEUP_COUNTER		0x29
#define KXT3J_NA_COUNTER			0x2A
#define KXTJ3_WAKEUP_THRESHOLD_H 	0x6A
#define KXTJ3_WAKEUP_THRESHOLD_L 	0x6B

/* Configuration */
#define KXTJ3_SET_CONFA				0x80	/* Operationg Mode/ 8bits Mode/ Range +-2g/ WakeUp function disable */

//*****************************************************************************************************************************************//

typedef enum
{
	WHOAMI 		= 0,
	DCSP_RESP 	= 1,
	X_ACC		= 2,
	Y_ACC 		= 3,
	Z_ACC		= 4,
	X_Y_Z_ACC	= 5,
	X_ANGLE		= 6,
	Y_ANGLE		= 7,
	Z_ANGLE		= 8,
	X_Y_Z_ANGLE	= 9,
	X_Y_Z_ACC_BURST = 10,
	IGNORE
}e_operationToDecode;

// Object
typedef struct
{
	double x;
	double y;
	double z;
	double AngleX;
	double AngleY;
	double AngleZ;
}s_Ksxj3_accel_data_t;

typedef struct
{
	volatile s_i2c_object i2c_obj;
	s_Ksxj3_accel_data_t  ksxj3_accel_data;
}s_Ksxj3_object_t;
//*****************************************************************************************************************************************//

/* Publics functions */
void ManageDataKSXJ3			(uint8_t*);
void Ksxj3_checkConnexion 		(void);
void Ksxj3_checkFunctionality 	(void);
void Ksxj3_SetConfiguration 	(void);
void Ksxj3_GetXAxis 			(void);
void Ksxj3_GetYAxis 			(void);
void Ksxj3_GetZAxis 			(void);
void Ksxj3_GetXYZAxis 			(void);
void Ksxj3_GetAngle 			(e_operationToDecode angle);
void DecodeKsxj3_val 			(uint8_t *data, e_operationToDecode code);
void DisplayDataKsxj3			(const char* prefixe, double val, const char* suffixe);
//*****************************************************************************************************************************************//

#endif /* KXTJ3_DEVICE_H_ */
