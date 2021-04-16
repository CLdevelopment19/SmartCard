/*
 * debug_csl.h
 *
 *  Created on: 10 avr. 2021
 *      Author: clement
 */

#ifndef DEBUG_CSL_H_
#define DEBUG_CSL_H_


#include <string.h>
#include "hardwareConfig.h"
#include "circularBuffer.h"
/*******************************************************************************************************/

/* Constante */
#define MAX_BUF_TX_CONSOLE		256
#define	TIMEOUT_TX				10			// 10ms  (Calcul time to transmit 256octets in one shot)

typedef enum
{
	DBG_ERROR = 0,
	DBG_LOG,
	DBG_INFO,
}e_kindOfDbg_t;

typedef struct
{
	s_circular_bf bufRcvConsole;
	s_circular_bf bufTxConsole;
	uint16_t		nCharBufRcv;
	uint16_t		nCharBufTx;
	uint8_t			TxDocker[2*MAX_BUF_TX_CONSOLE];
	ITStatus		timeout;
	ITStatus		txReady;
}ConsoleObj_t;

#define DEBUG_ERROR(str) debug_manager(DBG_ERROR, str)
#define DEBUG_LOG(str) debug_manager(DBG_LOG, str)
#define DEBUG_INFO(str) debug_manager(DBG_INFO, str)
/*******************************************************************************************************/

/* Prototypes functions */
void InitObjectConsole 		(void);
void debug_manager 			(e_kindOfDbg_t type, const char* str);
void callbackUart4_console	(void);
void ConsoleTxTimeout 		(void);
/*******************************************************************************************************/

#endif /* DEBUG_CSL_H_ */
