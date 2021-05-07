/*
 * analog_device.c
 *
 *  Created on: 1 mai 2021
 *      Author: clement
 */

#include "analog_device.h"
/*******************************************************************************************************/

volatile static s_AnalogDevice_obj analogDevice_obj = {.sample = {0}, .somme = 0, .average = 0.0};
/*******************************************************************************************************/

void ADC_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&ADC1_handle);
}

void DMA2_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(ADC1_handle.DMA_Handle);
}
/*******************************************************************************************************/

void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef* AdcHandle)
{
	unsigned long tmp = 0;
	GPIO_LED1_TOGGLE;
	for(int i = 0; i < NUM_SAMPLE_GET; i++){
		tmp = tmp + analogDevice_obj.sample[i];
	}
	analogDevice_obj.somme = tmp;
	analogDevice_obj.average = (double)(((double)analogDevice_obj.somme)/NUM_SAMPLE_GET);
}
/*******************************************************************************************************/

void StartConversionADC (ADC_HandleTypeDef* AdcHandle)
{
	HAL_ADC_Start_DMA(AdcHandle, (uint32_t*)analogDevice_obj.sample, NUM_SAMPLE_GET);
}
/*******************************************************************************************************/

void StopConversionADC (ADC_HandleTypeDef* AdcHandle)
{
	HAL_ADC_Stop_DMA(AdcHandle);
}
/*******************************************************************************************************/

/*void HAL_ADC_ConvHalfCpltCallback (ADC_HandleTypeDef* AdcHandle)
{

}*/
