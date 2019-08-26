/*
 * Atmega8 connected with another atmega8 by Software serial to get the real time an date from it & calculate time for praying times 
	then play Adan sound for each using small MP3 player "DFPlayer" 
 * 
 * 
 * Atmega8_DFPlayer_SWUART.c
 *
 * Created: 12/22/2018 15:41:12
 * Author : Se7S_92
 */ 
#define F_CPU	16000000UL
#define Songs_Max_Count 14
#define Songs_Full_Hour 17

#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5
#define LEDS_Port PORTB
#define LEDS_Port_Dir DDRB

#define Recording_Record PD7
#define Recording_Play PD6
#define Recording_Port PORTD
#define Recording_Port_Dir DDRD

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <avr/Interrupt.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

#include "stdutils.h"
#include "DFPlayer.h"
#include "8P_USART.h"
#include "8P_SWUART.h"
#include "praying_times.h"

void Check_Time(void);
void Recieve_New_Data(void);
void Check_DFPlayer(void);
void Stop_DFPlayer(void);
void Play_Song(uint8 Choosen_Song);
void Play_Adan(uint8 Root);
void set_new_praying_times(uint8 year,uint8 month ,uint8 date);

uint8 Rx_Data[7] = {0};
uint8 O_Year = 0, O_Month = 0, O_Date = 0 , O_Location = 2;	
uint8 Playing_Adan = 0, Playing_Song = 0;
uint8 Songs_Interrupt = 0, Played_Song = 2;

float Longitude , Latitude , TimeZone ,FajrTwilight ,IshaTwilight ;

extern float fajrTime, sunRiseTime, zuhrTime, asrTime, maghribTime, ishaTime;
extern uint8 hours, minutes;

uint8 fajrTime_hour,fajrTime_min,zuhrTime_hour,zuhrTime_min,asrTime_hour,asrTime_min,
		maghribTime_hour,maghribTime_min,ishaTime_hour,ishaTime_min;

int main(void)
{
	LEDS_Port_Dir = (1<<LED1) | (1<<LED2) | (1<<LED3) | (1<<LED4);	Recording_Port_Dir = (1<<Recording_Play) | (1<<Recording_Record);	DFPlayer_En_Port_Dir |= (1<<DFPlayer_En_Pin);
	sei();
	MCUCR = (1<<ISC10) | (1<<ISC11) ;		//enable interrupt at rising edge of PD2 & PD3
	GICR |= (1<<INT1);
	
	LEDS_Port |= (1<<LED4);
	SWU_Initialize();
	
	Set_New_Location(O_Location);
 	/* Replace with your application code */
	while (1)
	{
		LEDS_Port |= (1<<LED4);
		
		SWU_RxByte(&Rx_Data[0]);
		if(Rx_Data[0] == 0x80)
			Recieve_New_Data();
		
		Check_DFPlayer();
	}
}

ISR(INT1_vect)
{
	_delay_ms(100);
	while(PIND & (1<<PD3)){}
	if (Playing_Adan == 0)
	{
		LEDS_Port ^= (1<<LED1);
		Songs_Interrupt = 1;
		Played_Song++;
	}
}

static inline int bcd_decimal(uint8 hex)
{
	int dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
	return dec;
}

static inline uint8 decimal_bcd(uint8 num)
{
	uint8 bcd = ((num / 10)<<4) | (num % 10);
	return bcd;
}

void Recieve_New_Data()
{
	for(uint8 i = 1 ; i < 7 ; i++)
		SWU_RxByte(&Rx_Data[i]);
	
	if (Rx_Data[1] != O_Year || Rx_Data[2] != O_Month || Rx_Data[3] != O_Date || Rx_Data[6] != O_Location)
		set_new_praying_times(Rx_Data[1],Rx_Data[2],Rx_Data[3]);
	
	/* check time to play Adan  */
	if(Playing_Adan == 0)
		Check_Time();
	
	if(Playing_Adan == 1 || Playing_Song == 1  )
	{
		LEDS_Port ^= (1<<LED1);
		LEDS_Port ^= (1<<LED2);
		LEDS_Port ^= (1<<LED3);
		_delay_ms(100);
	}
	Rx_Data[0] = 0x00;	
}

void Check_DFPlayer()
{
	/* If track is finished, stop */
	if (Songs_Interrupt != 1)
	{
		if(DFPlayerInfo.FinishedTrack != 0)
		{
			DFPlayerInfo.FinishedTrack = 0;
			Stop_DFPlayer();
		}
	}
	else if (Songs_Interrupt == 1)
	{
		if (Playing_Song == 0)
			DFP_Initialize();
			
		Play_Song(Played_Song);
	}
}

void Play_Song(uint8 Choosen_Song)
{
	static uint8 Old_Song=0;
	
	if (Choosen_Song < Songs_Max_Count)
	{
		if(Old_Song != Choosen_Song)
		{
			LEDS_Port |= (1<<LED2);
			Old_Song = Choosen_Song;
			Playing_Song= 1;
			DFP_PlayTrackInRoot(Played_Song);
		}
		
		if(DFPlayerInfo.FinishedTrack != 0)
		{
			DFPlayerInfo.FinishedTrack = 0;
			Played_Song++;
		}
	}
	else
	{
		Stop_DFPlayer();
		Old_Song=0;
	}
}

void Stop_DFPlayer()
{
	Played_Song = 2;
	Songs_Interrupt = 0;
	Playing_Song = 0;
	Playing_Adan = 0;
	LEDS_Port &= ~((1<<LED1) | (1<<LED2) | (1<<LED3));
	DFPlayer_En_Port &=~ (1<<DFPlayer_En_Pin);
	DFP_Reset();
}

void Check_Time()
{
	if ((Rx_Data[4] == zuhrTime_hour		&&  Rx_Data[5] == zuhrTime_min	)||
		(Rx_Data[4] == asrTime_hour		&&  Rx_Data[5] == asrTime_min		)||
		(Rx_Data[4] == maghribTime_hour	&&  Rx_Data[5] == maghribTime_min	)||
		(Rx_Data[4] == ishaTime_hour		&&  Rx_Data[5] == ishaTime_min	) )
	{
		Play_Adan(2);	
	}
	else if ((Rx_Data[4] == fajrTime_hour &&  Rx_Data[5] == fajrTime_min) )
	{
		Play_Adan(1);
	}
	else if (Rx_Data[5] == 0)
	{
		Play_Adan(Songs_Full_Hour);
	}
	
}

void set_new_praying_times(uint8 year,uint8 month ,uint8 date )
{
	O_Year = year;
	O_Month = month;
	O_Date = date;
	
	if (Rx_Data[6] != O_Location)
		Set_New_Location(Rx_Data[6]);
	
	
	uint16 N_Year = bcd_decimal(year);
	month = bcd_decimal(month);
	date = bcd_decimal(date);
	
	N_Year = N_Year + 2000;
	
	calcPrayerTimes1(N_Year,month,date);
	
	doubleToHrMin(fajrTime);
		fajrTime_hour = decimal_bcd(hours);
			fajrTime_min = decimal_bcd(minutes);
	doubleToHrMin(zuhrTime);
		zuhrTime_hour = decimal_bcd(hours);
			zuhrTime_min = decimal_bcd(minutes);
	doubleToHrMin(asrTime);
		asrTime_hour = decimal_bcd(hours);
			asrTime_min = decimal_bcd(minutes);
	doubleToHrMin(maghribTime);
		maghribTime_hour = decimal_bcd(hours);
			maghribTime_min = decimal_bcd(minutes);
	doubleToHrMin(ishaTime);
		ishaTime_hour = decimal_bcd(hours);
			ishaTime_min = decimal_bcd(minutes);
}

void Play_Adan(uint8 Root)
{
	if (Playing_Song == 1)
		Stop_DFPlayer();
	
	LEDS_Port |= (1<<LED3);
	DFP_Initialize();
	DFP_PlayTrackInRoot(Root);
	Playing_Adan = 1;			//DFPlayer is on
}
