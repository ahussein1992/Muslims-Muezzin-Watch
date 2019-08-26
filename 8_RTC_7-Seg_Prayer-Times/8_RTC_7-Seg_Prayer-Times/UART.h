#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>


void UART_Init(void);								//Initialize UART Registers
void UART_SendByte(const unsigned char data);		//send byte of data 
uint8 UART_RecieveByte(void);						//Receive byte of data
void UART_SendString(const unsigned char *Str);		//Send full String
void UART_ReceiveString(unsigned char *Str);		// Receive string until #

void UART_Init(void)
{

	//UCSRA = (1<<U2X); //double transmission speed
	
	UCSRB = (1<<RXEN) | (1<<TXEN); //enable UART as transmitter and receiver.
	
	UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1); //8-bit data, NO parity, one stop bit and asynch
	
	/* baud rate=115200 & Fosc=16MHz -->  UBBR=( Fosc / (8 * baud rate) ) - 1 = 16 */
	/* baud rate=9600 & Fosc=16MHz -->  UBBR=( Fosc / (8 * baud rate) ) - 1 = 207 */
	UBRRH = 0;
	UBRRL = 103;
}

void UART_SendByte(const unsigned char data)
{
	while(!(UCSRA & (1<<UDRE))){} //UDRE flag is set when the buffer is empty and ready for transmitting a new byte so wait until this flag is set to one.
	UDR = data;
}

uint8 UART_RecieveByte(void)
{
	while(!(UCSRA & (1<<RXC))){} //RXC flag is set when the UART receive data so until this flag is set to one
	return UDR;
}

void UART_SendString(const unsigned char *Str)
{
	unsigned char i = 0;
	while(Str[i] != '\0')
	{
		UART_SendByte(Str[i]);
		i++;
	}
}

void UART_ReceiveString(unsigned char *Str)
{
	unsigned char i = 0;
	Str[i] = UART_RecieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_RecieveByte();
	}
	Str[i] = '\0';
}

#endif