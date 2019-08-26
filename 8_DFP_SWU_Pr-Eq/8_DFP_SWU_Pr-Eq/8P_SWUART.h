/*
 * _8P_SWUART.h
 */ 


#ifndef __8P_SWUART_H_
#define __8P_SWUART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdutils.h"

#ifndef F_CPU
#define F_CPU	16000000L
#endif

#define SWU_DDR		DDRB
#define SWU_PORT	PORTB
#define SWU_PIN		PINB
#define SWU_RX		PB0
#define SWU_TX		PB1

#define BAUDRATE	9600
#define BIT_TIME	(uint16_t)(F_CPU / BAUDRATE + 0.5)		/* Clocks per one bit (rounded) */

extern volatile uint8 sRxDone;

void SWU_Initialize(void);
void SWU_RxByte(uint8 *data);
void SWU_TxByte(uint8 data);
void SWU_RxBuffer(uint8 *buffer, uint8 length);
void SWU_TxBuffer(uint8 *buffer, uint8 length);

#endif /* __8P_SWUART_H_ */