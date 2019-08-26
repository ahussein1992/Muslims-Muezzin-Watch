
#ifndef F_CPU
#define F_CPU	16000000
#endif

#include "DFPlayer.h"
#include "8P_USART.h"
#include <util/delay.h>

ISR(USART_RXC_vect)
{
	/* Disable Rx Interrupt */
	usart_rx_flag = 1;
	DFP_HandleMessage();
}

void USART_Init()
{
	/* Enable U2X0 */
	UCSRA |= _BV(U2X);
	
	/* baud rate=9600 & Fosc=16MHz -->  UBBR=( Fosc / (8 * baud rate) ) - 1 = 103 */
	UBRRH = 0;
	UBRRL = 207;
	
	UCSRB = _BV(RXEN) | _BV(TXEN); //enable UART as transmitter and receiver.

	UCSRC = _BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1); //8-bit data, NO parity, one stop bit and asynch

	
	/* Enable Receive Interrupt */
	UCSRB |= _BV(RXCIE);
}

bool USART_RxByte(uint8 *data)
{	
	uint16 loop = USART_TIMEOUT;
	do 
	{
		if(UCSRA & _BV(RXC))
		{
			*data = UDR;			
			return true;
		}
		_delay_ms(1);		
	} while (--loop);
	
	return false;
}

void USART_RxByte_IT()
{
	/* Enable Rx Interrupt */	
	UCSRB |= _BV(RXCIE);
}

void USART_TxByte(uint8 data)
{
	while((UCSRA & _BV(UDRE)) == 0);			
	UDR = data;	
}

bool USART_RxBuffer(uint8 *buffer, uint16 len)
{
	for(int i=0; i < len; i++)
	{
		if(!USART_RxByte(&buffer[i]))
		{
			return false;
		}
		
		/* DFPlayer dedicated code */		
		if(i == 2 && buffer[2] != 0x06) return false;
	}	
	
	return true;
}

void USART_TxBuffer(uint8 *buffer, uint16 len)
{
	for(int i=0; i < len; i++)
	{
		USART_TxByte(buffer[i]);
	}
}

bool USART_Available()
{
	if(UCSRA & _BV(RXC))
	{
		return true;
	}
	
	return false;
}