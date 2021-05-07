/*
 * analog_device.h
 *
 *  Created on: 1 mai 2021
 *      Author: clement
 */

#ifndef ANALOG_DEVICE_H_
#define ANALOG_DEVICE_H_

#include "main.h"
#include "hardwareConfig.h"
/*******************************************************************************************************/

/* Constantes */
/* Time conversion
 * Tconv = (Sampling Time + Cycles)/FADC where cycles is 12 with 12bits accuracy
 * If buffer is 512, and Sampling Time is 480 then each interrupt occur every 9.33ms
 * If buffer is 512, and Sampling Time is 144 then each interrupt occur every 2.95ms
 */
#define	NUM_SAMPLE_GET	512U
#define DIVISEUR_BRIDGE	1
/*******************************************************************************************************/

typedef struct
{
	uint16_t sample[NUM_SAMPLE_GET];
	unsigned long somme;
	double   average;
}s_AnalogDevice_obj;
/*******************************************************************************************************/

/* Publics variables */
/*******************************************************************************************************/

/* Public function */
void StartConversionADC (ADC_HandleTypeDef* AdcHandle);
void StopConversionADC (ADC_HandleTypeDef* AdcHandle);
/*******************************************************************************************************/

#endif /* ANALOG_DEVICE_H_ */
