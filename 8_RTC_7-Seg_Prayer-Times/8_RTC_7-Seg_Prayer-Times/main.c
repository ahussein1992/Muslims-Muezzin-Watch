/*
 * Atmega 8 connected with RTC_DS1307 & 6 * 7-Segment displays time and date (4 for time & 2 for day of the month)
 * RTC & 7-Segment to display time and day.c
 * Created: 12/5/2018 4:00:40 PM
 * Author : Se7S_92
 * Connection of 7-Segment   PB0:PB3 Data Pins & C0-C1-C2-C3-B4-B5 Control Pins 
 */ 

#define F_CPU 16000000UL
#define Blinking_Times_Max 60
#define Segment_Wait_Time 2

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "stdutils.h"
#include "i2c_master.h"
#include "rtc.h"
#include "uart.h"

static inline int bcd_decimal(uint8 hex);
void Display_Time1();
void Display_Time2(uint8 Choosen_Segment);
void Send_Date_Time_DFPlayer (rtc_t *rtc);
void Receive_Time(void);
void Set_Time(void);
void EEPROM_write(uint16 Address, uint8 Data);
uint8 EEPROM_read(uint16 Address);

static uint8 New_Time[6], N_Year[2], N_Month[2], O_Location , N_Location;
uint8 Setting_interrupt = 0, Blinking_Times = 0;
rtc_t rtc;

int main(void)
{
	DDRB |= 0xFF ;
	DDRC = (1<<PC0) | (1<<PC1)	| (1<<PC2) | (1<<PC3);
	DDRD &= ~((1<<PD2)|(1<<PD3));
	DDRD |= (1<<PD4);
	PORTD |= (1<<PD4);
	
	 /* Replace with your application code */
	sei();
	MCUCR = (1<<ISC00) | (1<<ISC01) ;		//enable Setting_interrupt at rising edge of PD2
	GICR |= (1<<INT0);	
	
	RTC_Init();
	UART_Init();
	_delay_ms(500);
	
	O_Location = EEPROM_read(1);
	
    while (1) 
    {
		if (Setting_interrupt == 1)
		{
			Receive_Time();
			Set_Time();
		}
		RTC_GetDateTime(&rtc);
		Display_Time1(&rtc);
		Send_Date_Time_DFPlayer(&rtc);
 	 }
}

ISR(INT0_vect)
{
	cli();
	Setting_interrupt = 1;
}

static inline int bcd_decimal(uint8 hex)
{        
 	int dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
	return dec;
}

void display_Segment_0()
{
	PORTC |= (1<<PC0 );
	PORTC &= ~ ((1<<PC1 ) | (1<<PC2 ) | (1<<PC3 ));
	PORTB &= ~ ((1<<PB4 ) | (1<<PB5 ));
	PORTB = New_Time[0];
}

void display_Segment_1()
{
	PORTC |= (1<<PC1);
	PORTC &= ~ ((1<<PC0 ) | (1<<PC2 ) | (1<<PC3 ));
	PORTB &= ~ ((1<<PB4 ) | (1<<PB5 ));
	PORTB = New_Time[1];
}

void display_Segment_2()
{
	PORTC |= (1<<PC2);
	PORTC &= ~ ((1<<PC1 ) | (1<<PC0 ) | (1<<PC3 ));
	PORTB &= ~ ((1<<PB4 ) | (1<<PB5 ));
	PORTB = New_Time[2];
}

void display_Segment_3()
{
	PORTC |= (1<<PC3);
	PORTC &= ~ ((1<<PC1 ) | (1<<PC2 ) | (1<<PC0 ));
	PORTB &= ~ ((1<<PB4 ) | (1<<PB5 ));
	PORTB = New_Time[3];
}

void display_Segment_4()
{
	PORTB = New_Time[4];
	PORTB |= (1<<PB5 );
	PORTB &= ~ (1<<PB4);
	PORTC &= ~ ((1<<PC1 ) | (1<<PC2 ) | (1<<PC0 ) | (1<<PC3));
}

void display_Segment_5()
{
	PORTB = New_Time[5];
	PORTB |= (1<<PB4);
	PORTB &= ~ (1<<PB5);
	PORTC &= ~ ((1<<PC1 ) | (1<<PC2 ) | (1<<PC0 ) | (1<<PC3));
}

void Stop_Display()
{
	PORTB &=~ ((1<<PB4) | (1<<PB5));
	PORTC &=~ ((1<<PC1 ) | (1<<PC2 ) | (1<<PC0 ) | (1<<PC3));
}

void Display_Time1()
{
	New_Time[0] = (rtc.hour & 0xF0) >> 4;
	New_Time[1] = rtc.hour & 0x0F ;
	New_Time[2] = (rtc.min & 0xF0) >> 4;
	New_Time[3] = rtc.min & 0x0F ;
	New_Time[4] = (rtc.date & 0xF0) >> 4;
	New_Time[5] = rtc.date & 0x0F ;
	
	display_Segment_0();	_delay_ms(Segment_Wait_Time);
	display_Segment_1();	_delay_ms(Segment_Wait_Time);
	display_Segment_2();	_delay_ms(Segment_Wait_Time);
	display_Segment_3();	_delay_ms(Segment_Wait_Time);
	display_Segment_4();	_delay_ms(Segment_Wait_Time);
	display_Segment_5();	_delay_ms(Segment_Wait_Time);
	
	Stop_Display();
}

void Display_Time2(uint8 Choosen_Segment)
{
	switch (Choosen_Segment)
	{
		case 0:
			display_Segment_1();		_delay_ms(Segment_Wait_Time);
			display_Segment_2();		_delay_ms(Segment_Wait_Time);
			display_Segment_3();		_delay_ms(Segment_Wait_Time);
			display_Segment_4();		_delay_ms(Segment_Wait_Time);
			display_Segment_5();		_delay_ms(Segment_Wait_Time);
						
			Blinking_Times++;
						
			if( (Blinking_Times_Max*3/4) < Blinking_Times )
			{
				if(Blinking_Times>Blinking_Times_Max) Blinking_Times=0;
				Stop_Display();			_delay_ms(Segment_Wait_Time);
			}
			else 
			{
				display_Segment_0();	_delay_ms(Segment_Wait_Time);
			}
		break;
					
		case 1:
			display_Segment_0();		_delay_ms(Segment_Wait_Time);
			display_Segment_2();		_delay_ms(Segment_Wait_Time);
			display_Segment_3();		_delay_ms(Segment_Wait_Time);
			display_Segment_4();		_delay_ms(Segment_Wait_Time);
			display_Segment_5();		_delay_ms(Segment_Wait_Time);
					
			Blinking_Times++;
					
			if( (Blinking_Times_Max*3/4) < Blinking_Times )
			{
				if(Blinking_Times>Blinking_Times_Max) Blinking_Times=0;
				Stop_Display();			_delay_ms(Segment_Wait_Time);
			}
			else
			{
				display_Segment_1();	_delay_ms(Segment_Wait_Time);
			}
		break;
					
		case 2:
			display_Segment_0();		_delay_ms(Segment_Wait_Time);
			display_Segment_1();		_delay_ms(Segment_Wait_Time);
			display_Segment_3();		_delay_ms(Segment_Wait_Time);
			display_Segment_4();		_delay_ms(Segment_Wait_Time);
			display_Segment_5();		_delay_ms(Segment_Wait_Time);
					
			Blinking_Times++;
					
			if( (Blinking_Times_Max*3/4) < Blinking_Times )
			{
				if(Blinking_Times>Blinking_Times_Max) Blinking_Times=0;
				Stop_Display();			_delay_ms(Segment_Wait_Time);
			}
			else
			{
				display_Segment_2();	_delay_ms(Segment_Wait_Time);
			}
		break;
					
		case 3:
			display_Segment_0();		_delay_ms(Segment_Wait_Time);
			display_Segment_1();		_delay_ms(Segment_Wait_Time);
			display_Segment_2();		_delay_ms(Segment_Wait_Time);
			display_Segment_4();		_delay_ms(Segment_Wait_Time);
			display_Segment_5();		_delay_ms(Segment_Wait_Time);
					
			Blinking_Times++;
					
			if( (Blinking_Times_Max*3/4) < Blinking_Times )
			{
				if(Blinking_Times>Blinking_Times_Max) Blinking_Times=0;
				Stop_Display();			_delay_ms(Segment_Wait_Time);
			}
			else
			{
				display_Segment_3();	_delay_ms(Segment_Wait_Time);
			}
		break;
					
		case 4:
			display_Segment_0();		_delay_ms(Segment_Wait_Time);
			display_Segment_1();		_delay_ms(Segment_Wait_Time);
			display_Segment_2();		_delay_ms(Segment_Wait_Time);
			display_Segment_3();		_delay_ms(Segment_Wait_Time);
			display_Segment_5();		_delay_ms(Segment_Wait_Time);
					
			Blinking_Times++;
					
			if( (Blinking_Times_Max*3/4) < Blinking_Times )
			{
				if(Blinking_Times>Blinking_Times_Max) Blinking_Times=0;
				Stop_Display();			_delay_ms(Segment_Wait_Time);
			}
			else
			{
				display_Segment_4();	_delay_ms(Segment_Wait_Time);
			}
		break;
					
		case 5:
			display_Segment_0();		_delay_ms(Segment_Wait_Time);
			display_Segment_1();		_delay_ms(Segment_Wait_Time);
			display_Segment_2();		_delay_ms(Segment_Wait_Time);
			display_Segment_3();		_delay_ms(Segment_Wait_Time);
			display_Segment_4();		_delay_ms(Segment_Wait_Time);
					
			Blinking_Times++;
					
			if( (Blinking_Times_Max*3/4) < Blinking_Times )
			{
				if(Blinking_Times>Blinking_Times_Max) Blinking_Times=0;
				Stop_Display();			_delay_ms(Segment_Wait_Time);
			}
			else
			{
				display_Segment_5();	_delay_ms(Segment_Wait_Time);
			}
		break;
	}			
	Stop_Display();		
}

void Send_Date_Time_DFPlayer (rtc_t *rtc)
{	
	UART_SendByte(0x80);
	UART_SendByte(rtc->year);
	UART_SendByte(rtc->month);
	UART_SendByte(rtc->date);
	UART_SendByte(rtc->hour);
	UART_SendByte(rtc->min);
	UART_SendByte(O_Location);
}

void Receive_Time()
{
	uint8 Choosen_Segment = 4, Time_Set = 0;
	uint8 Old_Time[6], i;
	
	for (i=0; i<6; i++)
		Old_Time[i] = New_Time[i];
	
	Blinking_Times = 0;
	
	New_Time[0] = '\0';
	New_Time[1] = '\0';
	New_Time[2] = '\0';
	New_Time[3] = '\0';
	New_Time[4] = (O_Location & 0xF0) >> 4;
	New_Time[5] = O_Location & 0x0F ;
	
	_delay_ms(100);
	while(PIND & (1<<PD2)) {}
	
	while (Time_Set < 3) 
	{
		if (PIND & (1<<PD2))
		{	
			while(PIND & (1<<PD2)) {}
			Choosen_Segment++;
			Blinking_Times = 0;
			if (Choosen_Segment > 5) 
			{
				Time_Set ++;
				Choosen_Segment = 0;
				if (Time_Set == 1)
				{
					Choosen_Segment = 2;
					N_Location = (New_Time[4] * 10) + (New_Time[5]);
					New_Time[0] = 2;
					New_Time[1] = 0;
					New_Time[2] = (rtc.year & 0xF0) >> 4;
					New_Time[3] = rtc.year & 0x0F ;
					New_Time[4] = (rtc.month & 0xF0) >> 4;
					New_Time[5] = rtc.month & 0x0F ;
				}
				if (Time_Set == 2)
				{
					Choosen_Segment = 0;
					N_Year[0]  = New_Time[2]; N_Year[1]  = New_Time[3];
					N_Month[0] = New_Time[4]; N_Month[1] = New_Time[5];
					for (i=0; i<6; i++)
					New_Time[i] = Old_Time[i];
				}
			}
		}
		
		if (PIND & (1<<PD3))
		{	
			while(PIND & (1<<PD3)) {}
			New_Time[Choosen_Segment]++; 
			if( New_Time[Choosen_Segment] > 9 )  
				New_Time[Choosen_Segment] = 0;
		}
	
		Display_Time2(Choosen_Segment);	
	}
}

void Set_Time()
{
	rtc.sec  = 0x00;																// Write sec from RAM address 00H
	rtc.hour = ((New_Time[0] << 4) & 0xF0) | (New_Time[1] & 0x0F) ;					// Write hour from RAM address 02H												// Write weekDay on RAM address 03H
	rtc.min  = ((New_Time[2] << 4) & 0xF0) | (New_Time[3] & 0x0F) ;					// Write min from RAM address 01H
	rtc.date = ((New_Time[4] << 4) & 0xF0) | (New_Time[5] & 0x0F) ;					// Write date on RAM address 04H
	rtc.month= ((N_Month[0]  << 4) & 0xF0) | (N_Month[1]  & 0x0F) ;					// Write month on RAM address 05H
	rtc.year = ((N_Year[0]   << 4) & 0xF0) | (N_Year[1]   & 0x0F) ;					// Write year on RAM address 06h
	rtc.weekDay = rtc.weekDay;
	RTC_SetDateTime(&rtc);
	
	if (N_Location != O_Location)
	{
		O_Location = N_Location;
		EEPROM_write(1,O_Location);
		while(EECR & (1<<EEWE))
		;
	}
	
	PORTD &= ~ (1<<PD4);	//reset Micro 
}

void EEPROM_write(uint16 Address, uint8 Data)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	/* Set up address and data registers */
	EEAR = Address;
	EEDR = Data;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}

uint8 EEPROM_read(uint16 Address)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
	;
	/* Set up address register */
	EEAR = Address;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}