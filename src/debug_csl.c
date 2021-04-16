/*
 * debug_csl.c
 *
 *  Created on: 10 avr. 2021
 *      Author: clement
 */

#include "debug_csl.h"
/*******************************************************************************************************/

/* Locales variables */
static const char *PREFIX_ERROR = "ERR : ";
static const char *PREFIX_LOG	= "LOG : ";
static const char *PREFIX_INFO	= "INFO : ";

static ConsoleObj_t	consoleObj;
static uint8_t 		tmpBufferTx[2*MAX_BUF_TX_CONSOLE] = {0};
/*******************************************************************************************************/

/* Locales functions */
static void ManageTransmitDataConsole (const uint8_t* data, uint8_t size);
/*******************************************************************************************************/

/* ISR Console => UART4 IP */
void UART4_IRQHandler(void)
{
  HAL_UART_IRQHandler(&Uart4_DBG_Csl_handle);
}
/*******************************************************************************************************/

void ManageTransmitDataConsole (const uint8_t* data, uint8_t size)
{
	if(consoleObj.txReady)			// If tx is free, so last transmit data over uart is done, transmit is permit
	{
		memcpy(tmpBufferTx, data, size);
		consoleObj.txReady = RESET;
		HAL_UART_Transmit_IT(&Uart4_DBG_Csl_handle, tmpBufferTx, size);
	}else							// Store data in circular buffer to avoid losing data
	{
		if(consoleObj.nCharBufTx < MAX_BUF_TX_CONSOLE)
		{
			//GPIO_LED1_ON;
			WriteCircularBuffer(&consoleObj.bufTxConsole, data, size);
			consoleObj.nCharBufTx += size;
		}else
		{
			GPIO_LED2_ON
			consoleObj.timeout = RESET;
			while(consoleObj.timeout != SET);		// Take a break, wait data had been sent
			do{}while(!consoleObj.txReady);
			ManageTransmitDataConsole(data, size);
		}
	}
}
/*******************************************************************************************************/

void debug_manager (e_kindOfDbg_t type, const char* str)
{
	char out[256] = {0};
	uint8_t n_char = 0;
	// Add entry size string control to avoid overflow
	// TO DO ASSERT_SIZE
	switch(type){
	case DBG_ERROR:
		memcpy(out, PREFIX_ERROR, strlen(PREFIX_ERROR));
		n_char = strlen(PREFIX_ERROR);
		break;
	case DBG_LOG:
		memcpy(out, PREFIX_LOG, strlen(PREFIX_LOG));
		n_char = strlen(PREFIX_LOG);
		break;
	case DBG_INFO:
		memcpy(out, PREFIX_INFO, strlen(PREFIX_INFO));
		n_char = strlen(PREFIX_INFO);
		break;
	}

	memcpy(&out[n_char], str, strlen(str));
	ManageTransmitDataConsole((uint8_t*)out, strlen(out));
}
/*******************************************************************************************************/

void InitObjectConsole 	(void)
{
	memset(consoleObj.TxDocker, 0, 0);
	InitCircularBuffer(&consoleObj.bufRcvConsole);
	InitCircularBuffer(&consoleObj.bufTxConsole);
	consoleObj.nCharBufRcv = 0;
	consoleObj.nCharBufTx = 0;
	consoleObj.timeout = RESET;
	consoleObj.txReady = SET;
}
/*******************************************************************************************************/

void callbackUart4_console()
{
	consoleObj.txReady = SET;
	if(consoleObj.nCharBufTx != 0)
	{
		uint8_t lgt = consoleObj.nCharBufTx;
		consoleObj.nCharBufTx = 0;
		ReadCircularBuffer(&(consoleObj.bufTxConsole), consoleObj.TxDocker, lgt);
		ManageTransmitDataConsole((const uint8_t*)consoleObj.TxDocker, lgt);
	}
}
/*******************************************************************************************************/

void ConsoleTxTimeout (void)
{
	consoleObj.timeout = SET;
}
/*******************************************************************************************************/
