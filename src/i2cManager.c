/*
 * i2cManager.c
 *
 *  Created on: 23 avr. 2021
 *      Author: clement
 */

#include "i2cManager.h"
/*******************************************************************************************************/

/* Locales Variables */
static volatile 	ITStatus 		prv_flag_i2c2_transmit	= SET;
static volatile 	ITStatus 		prv_flag_i2c2_receive 	= RESET;
static volatile 	s_i2c_object	*prv_image_i2cObject;
static void 		(*treatmentData)(uint8_t* data) 		= 0;
/*******************************************************************************************************/

void I2C2_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&I2C2_handle);
}
//*****************************************************************************************************************************************//

void I2C2_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&I2C2_handle);
}
//*****************************************************************************************************************************************//

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	if(I2cHandle->Instance == I2C2_handle.Instance)
	{
		prv_flag_i2c2_transmit = SET;
		Manage_i2c2_FSM((s_i2c_object*)prv_image_i2cObject);

	}
}
//*****************************************************************************************************************************************//

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	if(I2cHandle->Instance == I2C2_handle.Instance)
	{
		prv_flag_i2c2_receive = SET;
		Manage_i2c2_FSM((s_i2c_object*)prv_image_i2cObject);
	}
}

void HAL_I2C_MemRxCpltCallback	(I2C_HandleTypeDef *I2cHandle)
{
	if(I2cHandle->Instance == I2C2_handle.Instance)
	{
		prv_flag_i2c2_receive = SET;
		Manage_i2c2_FSM((s_i2c_object*)prv_image_i2cObject);
	}
}
//*****************************************************************************************************************************************//

e_bool Manage_i2c2_FSM (volatile s_i2c_object* i2c)
{
	static uint8_t iteration_failed = 0;

	switch(i2c->status)
	{
	case IDLE:
		prv_flag_i2c2_transmit = SET;
		i2c->status = READY_WRITE;
		break;
	case READY_WRITE:
		if(prv_flag_i2c2_transmit)
		{
			iteration_failed = 0;
			prv_flag_i2c2_transmit = RESET;
			if(HAL_I2C_Master_Transmit_IT(&I2C2_handle, (uint16_t)i2c->adr, (uint8_t *)i2c->dta_tx, i2c->size_tx) != HAL_OK)
			{
				return FALSE;
			}
			if(i2c->op == READ_OP)
				i2c->status = READY_READ;
			else
				i2c->status = IDLE;
		}else
		{
			i2c->status = READY_WRITE;
			if((iteration_failed++)==MAX_ITERATION_FAILED)
				i2c->status = ERR;
		}
	break;
	case READY_READ:
		if(prv_flag_i2c2_transmit)
		{
			prv_flag_i2c2_transmit = RESET;
			iteration_failed = 0;
			if(HAL_I2C_Master_Receive_IT(&I2C2_handle, (uint16_t)i2c->adr+1, (uint8_t *)i2c->dta_rx, i2c->size_rx) != HAL_OK)
			{
				return FALSE;
			}
			i2c->status = DATA_AVAILABLE;
		}else
		{
			i2c->status = READY_READ;
			if((iteration_failed++)==MAX_ITERATION_FAILED)
				i2c->status = ERR;
		}
	break;
	case BURST_READ:
		if(prv_flag_i2c2_transmit)
		{
			prv_flag_i2c2_transmit = RESET;
			iteration_failed = 0;
			if(HAL_I2C_Mem_Read_IT(&I2C2_handle, i2c->adr+1, i2c->dta_tx[0], 1, (uint8_t *)i2c->dta_rx, i2c->size_rx) != HAL_OK)
			{
				return FALSE;
			}
			i2c->status = DATA_AVAILABLE;
		}else
		{
			i2c->status = BURST_READ;
			if((iteration_failed++)==MAX_ITERATION_FAILED)
				i2c->status = ERR;
		}
		break;
	case DATA_AVAILABLE:
		if(prv_flag_i2c2_receive)
		{
			iteration_failed = 0;
			prv_flag_i2c2_receive = RESET;
			prv_flag_i2c2_transmit = SET;
			i2c->status = IDLE;
			treatmentData(i2c->dta_rx);
		}else
		{
			i2c->status = DATA_AVAILABLE;
			if((iteration_failed++)==MAX_ITERATION_FAILED)
				i2c->status = ERR;
		}
		break;
	case ERR:
		while (HAL_I2C_GetState(&I2C2_handle) != HAL_I2C_STATE_READY);
		i2c->status = IDLE;
	}

	return TRUE;
}
//*****************************************************************************************************************************************//

void	LoadI2CObject		(volatile s_i2c_object* object, void (*callback)(uint8_t* data))
{
	prv_image_i2cObject = object;
	treatmentData = callback;

	Manage_i2c2_FSM((s_i2c_object*)prv_image_i2cObject);

}
//*****************************************************************************************************************************************//
