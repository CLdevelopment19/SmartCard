/*
 * KXTJ3_device.c
 *
 *  Created on: 23 avr. 2021
 *      Author: clement
 */

#include "KXTJ3_device.h"
#include "debug_csl.h"
#include <math.h>
#include "toolBox.h"
//*****************************************************************************************************************************************//

/* Constantes */
const char* PREFIXE_X_ACCELEROMETER = "KSXJ3 XOUT = ";
const char* PREFIXE_Y_ACCELEROMETER = "KSXJ3 YOUT = ";
const char* PREFIXE_Z_ACCELEROMETER = "KSXJ3 ZOUT = ";
const char* PREFIXE_X_ANGLE = "KSXJ3 XINC = ";
const char* PREFIXE_Y_ANGLE = "KSXJ3 YINC = ";
const char* PREFIXE_Z_ANGLE = "KSXJ3 ZINC = ";
const char* SUFFIXE_ACCELEROMETER = "m/s2\n";
const char* SUFFIXE_ANGLE = "°\n";
//*****************************************************************************************************************************************//

static s_Ksxj3_object_t 	Ksxj3_object = {.i2c_obj.adr = KXTJ3_ADDRESS,
											.i2c_obj.dta_tx = NULL,
											.i2c_obj.dta_rx = NULL,
											.i2c_obj.size_tx = 0,
											.i2c_obj.size_rx = 0,
											.i2c_obj.status = IDLE,
											.i2c_obj.op = WRITE_OP,
											.ksxj3_accel_data.x = 0.0,
											.ksxj3_accel_data.y =0.0,
											.ksxj3_accel_data.z = 0.0,
											.ksxj3_accel_data.AngleX = 0.0,
											.ksxj3_accel_data.AngleY =0.0,
											.ksxj3_accel_data.AngleY = 0.0};

static e_operationToDecode 	operationToDecode = WHOAMI;
//*****************************************************************************************************************************************//

void Ksxj3_checkConnexion (void)
{
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_WHO_AM_I;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = READ_OP;

	Ksxj3_object.i2c_obj.status = READY_WRITE;

	operationToDecode = WHOAMI;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
}
//*****************************************************************************************************************************************//

void Ksxj3_checkFunctionality (void)
{
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_DCSP_RESP;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = READ_OP;

	Ksxj3_object.i2c_obj.status = READY_WRITE;

	operationToDecode = DCSP_RESP;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
}
//*****************************************************************************************************************************************//

void Ksxj3_SetConfiguration (void)
{
	static uint8_t buf_tx[2];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_CTRL_REG1;
	buf_tx[1] = KXTJ3_SET_CONFA;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = WRITE_OP;

	Ksxj3_object.i2c_obj.status = READY_WRITE;

	operationToDecode = IGNORE;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
}
//*****************************************************************************************************************************************//

void Ksxj3_GetXAxis (void)
{
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_XOUT_H;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = READ_OP;

	Ksxj3_object.i2c_obj.status = READY_WRITE;

	operationToDecode = X_ACC;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
}
//*****************************************************************************************************************************************//

void Ksxj3_GetYAxis (void)
{
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_YOUT_H;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = READ_OP;

	Ksxj3_object.i2c_obj.status = READY_WRITE;

	operationToDecode = Y_ACC;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
}
//*****************************************************************************************************************************************//

void Ksxj3_GetZAxis (void)
{
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_ZOUT_H;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = READ_OP;

	Ksxj3_object.i2c_obj.status = READY_WRITE;

	operationToDecode = Z_ACC;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
}
//*****************************************************************************************************************************************//

void Ksxj3_GetXYZAxis (void)
{
	static uint8_t inc = 0;
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_XOUT_H + inc;

	inc = (inc + 2)%8;
	if(inc != 0)
	{
		Ksxj3_object.i2c_obj.dta_tx = buf_tx;
		Ksxj3_object.i2c_obj.dta_rx = buf_rx;

		Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

		Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

		Ksxj3_object.i2c_obj.op = READ_OP;

		Ksxj3_object.i2c_obj.status = READY_WRITE;

		operationToDecode = X_Y_Z_ACC;

		LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
	}
}

void Ksxj3_GetXYZAxis_burstMode (void)
{
	static uint8_t inc = 0;
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[6];

	buf_tx[0] = KXTJ3_XOUT_L;

	Ksxj3_object.i2c_obj.dta_tx = buf_tx;
	Ksxj3_object.i2c_obj.dta_rx = buf_rx;

	Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);
	Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

	Ksxj3_object.i2c_obj.op = READ_OP;

	Ksxj3_object.i2c_obj.status = BURST_READ;

	operationToDecode = X_Y_Z_ACC_BURST;

	LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);

}
//*****************************************************************************************************************************************//

void Ksxj3_GetAngle (e_operationToDecode angle)
{
	static uint8_t inc = 0;
	static uint8_t buf_tx[1];
	static uint8_t buf_rx[1];

	buf_tx[0] = KXTJ3_XOUT_H + inc;

	inc = (inc + 2)%8;
	if(inc != 0)
	{
		Ksxj3_object.i2c_obj.dta_tx = buf_tx;
		Ksxj3_object.i2c_obj.dta_rx = buf_rx;

		Ksxj3_object.i2c_obj.size_tx = SIZE_ARRAY(buf_tx);

		Ksxj3_object.i2c_obj.size_rx = SIZE_ARRAY(buf_rx);

		Ksxj3_object.i2c_obj.op = READ_OP;

		Ksxj3_object.i2c_obj.status = READY_WRITE;

		operationToDecode = angle;

		LoadI2CObject(&Ksxj3_object.i2c_obj, ManageDataKSXJ3);
	}
}
//*****************************************************************************************************************************************//

void ManageDataKSXJ3 (uint8_t *data)
{
	static uint8_t inc = 0;

	switch (operationToDecode)
	{
	case WHOAMI:
		if(data[0] == 0x35)
		{
			DEBUG_LOG("SUCCES : KSXJ3 Alive !\n");
			Ksxj3_SetConfiguration();
			DEBUG_LOG("KSXJ3 Enable !\n");
		}else{
			DEBUG_LOG("FAILED : KSXJ3 Missing !\n");
		}
		break;
	case DCSP_RESP :
		if(data[0] == 0x55)
		{
			DEBUG_LOG("SUCCES : KSXJ3 Functionality Validate !\n");
		}else{
			DEBUG_LOG("FAILED : KSXJ3 Bad Functionality !\n");
		}
		break;
	case X_ACC :
		DecodeKsxj3_val(data, X_ACC);
		DisplayDataKsxj3(PREFIXE_X_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.x, SUFFIXE_ACCELEROMETER);
		break;
	case Y_ACC :
		DecodeKsxj3_val(data, Y_ACC);
		DisplayDataKsxj3(PREFIXE_Y_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.y, SUFFIXE_ACCELEROMETER);
		break;
	case Z_ACC :
		DecodeKsxj3_val(data, Z_ACC);
		DisplayDataKsxj3(PREFIXE_Z_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.z, SUFFIXE_ACCELEROMETER);
		break;
	case X_Y_Z_ACC :
		switch(inc)
		{
		case 0:
			DecodeKsxj3_val(data, X_ACC);
			inc++;
			break;
		case 1:
			DecodeKsxj3_val(data, Y_ACC);
			inc++;
			break;
		case 2:
			DecodeKsxj3_val(data, Z_ACC);
			DisplayDataKsxj3(PREFIXE_X_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.x, SUFFIXE_ACCELEROMETER);
			DisplayDataKsxj3(PREFIXE_Y_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.y, SUFFIXE_ACCELEROMETER);
			DisplayDataKsxj3(PREFIXE_Z_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.z, SUFFIXE_ACCELEROMETER);
			inc = 0;
			break;
		}
		Ksxj3_GetXYZAxis();
		break;
	case X_ANGLE:
		switch(inc)
		{
		case 0:
			DecodeKsxj3_val(data, X_ACC);
			inc++;
			break;
		case 1:
			DecodeKsxj3_val(data, Y_ACC);
			inc++;
			break;
		case 2:
			DecodeKsxj3_val(data, Z_ACC);
			Ksxj3_object.ksxj3_accel_data.AngleX = (180/PI)*(atan(Ksxj3_object.ksxj3_accel_data.x/Ksxj3_object.ksxj3_accel_data.z));
			DisplayDataKsxj3(PREFIXE_X_ANGLE, Ksxj3_object.ksxj3_accel_data.AngleX, SUFFIXE_ANGLE);
			inc = 0;
			break;
		}
		Ksxj3_GetAngle(X_ANGLE);
		break;
	case Y_ANGLE:
		switch(inc)
		{
		case 0:
			DecodeKsxj3_val(data, X_ACC);
			inc++;
			break;
		case 1:
			DecodeKsxj3_val(data, Y_ACC);
			inc++;
			break;
		case 2:
			DecodeKsxj3_val(data, Z_ACC);
			Ksxj3_object.ksxj3_accel_data.AngleY = (180/PI)*(atan(Ksxj3_object.ksxj3_accel_data.y/Ksxj3_object.ksxj3_accel_data.z));
			DisplayDataKsxj3(PREFIXE_Y_ANGLE, Ksxj3_object.ksxj3_accel_data.AngleY, SUFFIXE_ANGLE);
			inc = 0;
			break;
		}
		Ksxj3_GetAngle(Y_ANGLE);
		break;
	case Z_ANGLE:
		switch(inc)
		{
		case 0:
			DecodeKsxj3_val(data, X_ACC);
			inc++;
			break;
		case 1:
			DecodeKsxj3_val(data, Y_ACC);
			inc++;
			break;
		case 2:
			DecodeKsxj3_val(data, Z_ACC);
			Ksxj3_object.ksxj3_accel_data.AngleZ = (180/PI)*(atan(Ksxj3_object.ksxj3_accel_data.x/Ksxj3_object.ksxj3_accel_data.y));
			DisplayDataKsxj3(PREFIXE_Z_ANGLE, Ksxj3_object.ksxj3_accel_data.AngleZ, SUFFIXE_ANGLE);
			inc = 0;
			break;
		}
		Ksxj3_GetAngle(Z_ANGLE);
		break;
	case X_Y_Z_ANGLE:
		break;
	case X_Y_Z_ACC_BURST:
		Ksxj3_object.ksxj3_accel_data.x = data[1];
		Ksxj3_object.ksxj3_accel_data.y = data[3];
		Ksxj3_object.ksxj3_accel_data.z = data[5];
		DisplayDataKsxj3(PREFIXE_Z_ACCELEROMETER, Ksxj3_object.ksxj3_accel_data.z, SUFFIXE_ACCELEROMETER);
		break;
	default :
		break;
	}
}
//*****************************************************************************************************************************************//

void DecodeKsxj3_val (uint8_t *data, uint8_t code)
{
	uint8_t complement 	= 0;
	double	signe		= 1;

	if(data[0] >= 128){
		complement = ~(uint8_t)data[0];
		signe = -1;
	}else{
		complement = data[0];
		signe = 1;
	}

	switch(code)
	{
	case X_ACC:
		Ksxj3_object.ksxj3_accel_data.x = signe*complement;
		break;
	case Y_ACC:
		Ksxj3_object.ksxj3_accel_data.y = signe*complement;
		break;
	case Z_ACC:
		Ksxj3_object.ksxj3_accel_data.z = signe*complement;
		break;
	}
}
//*****************************************************************************************************************************************//

void DisplayDataKsxj3			(const char* prefixe, double val, const char* suffixe)
{
	uint8_t str_tmp[8] = {0};
	char display[32] = {0};
	uint8_t pointer = 0;
	uint8_t l = 0;

	memcpy(display, prefixe, strlen(prefixe));
	pointer = strlen(prefixe);
	l = DoubleToStr(str_tmp, val, 2);
	memcpy(&display[pointer], str_tmp, l);
	pointer += l;
	memcpy(&display[pointer], suffixe, strlen(suffixe));
	DEBUG_LOG((const char*)display);
}
//*****************************************************************************************************************************************//
