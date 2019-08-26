/*
 * _8P_USART.h
 */ 

#ifndef _8P_USART_H_
#define _8P_USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stdutils.h"

#define USART_TIMEOUT	100	/* loop */



void USART_Init(void);
void USART_Disable(void);	
bool USART_RxByte(uint8 *data);
void USART_TxByte(uint8 data);
void USART_RxByte_IT();
bool USART_RxBuffer(uint8 *buffer, uint16 len);
void USART_TxBuffer(uint8 *buffer, uint16 len);
bool USART_Available(void);

volatile uint8 usart_rx_flag;

#endif /* _8P_USART_H_ */