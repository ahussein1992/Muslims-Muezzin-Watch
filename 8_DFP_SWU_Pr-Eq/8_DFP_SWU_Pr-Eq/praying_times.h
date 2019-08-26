/*
 * praying_times.h
 * Created: 1/1/2019 16:08:37
 *  Author: Se7S_92
 */ 
#ifndef PRAYING_TIMES_H_
#define PRAYING_TIMES_H_

#include <stdlib.h>
#include <math.h>
#include "stdutils.h"

uint8 hours, minutes;
float fajrTime, sunRiseTime, zuhrTime, asrTime, maghribTime, ishaTime;
float D,Alpha,Dec,sin_Dec,cos_Dec,sin_Lat,cos_Lat;

extern float Longitude, Latitude, TimeZone,FajrTwilight,IshaTwilight;
extern uint8 O_Location;

//convert Degree to Radian
double degToRad(double degree)
{
	return ((3.1415926 / 180) * degree);
}
//convert Radian to Degree
double radToDeg(double radian)
{
	return (radian * (180/3.1415926));
}
//make sure a value is between 0 and 360
double moreLess360(double value)
{
	while(value >= 360 || value < 0)
	{
		if(value > 360)
		value -= 360;
		
		else if (value <0)
		value += 360;
	}
	
	return value;
}
//make sure a value is between 0 and 24
double moreLess24(double value)
{
	while(value >= 24 || value < 0)
	{
		if(value > 24)
		value -= 24;
		
		else if (value <0)
		value += 24;
	}
	
	return value;
}
//convert the double number to Hours and Minutes
void doubleToHrMin(double number)
{
	hours = moreLess24(number);
	minutes = moreLess24(number - hours) * 60;
}

void Set_New_Location(uint8 N_Location)
{
	O_Location = N_Location;
	
	switch (O_Location)
	{
		case 1: //Cairo
			Longitude = 31.2;
			Latitude = 30;
			TimeZone = 2;
			FajrTwilight = -19.5;
			IshaTwilight = -17.5;
		break;
		
		case 2:	//Alex
			Longitude = 29.9;
			Latitude = 31.2;
			TimeZone = 2;
			FajrTwilight = -19.5;
			IshaTwilight = -17.5;
		break;
		
		case 3: //Suez
			Longitude = 32.5;
			Latitude = 29.9;
			TimeZone = 2;
			FajrTwilight = -19.5;
			IshaTwilight = -17.5;
		break;
		
		case 4: //Aswan
			Longitude = 32.9;
			Latitude = 24;
			TimeZone = 2;
			FajrTwilight = -19.5;
			IshaTwilight = -17.5;
		break;
		
		case 5: //Hurghada
			Longitude = 33.8;
			Latitude = 27.3;
			TimeZone = 2;
			FajrTwilight = -19.5;
			IshaTwilight = -17.5;
		break;
		
		case 6: //Qena
			Longitude = 32.7;
			Latitude = 26;
			TimeZone = 2;
			FajrTwilight = -19.5;
			IshaTwilight = -17.5;
		break;
	}
}
//Calculate Zuhr Praying time
void calc_Prayer_zuhr()
{
	double ST,Noon,UT_Noon;
	
	ST = 0.985647352 * D;
	ST = 100.46 + ST;
	Noon = Alpha - ST;
	Noon = moreLess360(Noon);
	UT_Noon = Noon - Longitude;/////////////////////////////////
	
	//Zuhr Time [Local noon]
	zuhrTime = UT_Noon / 15;
	zuhrTime = zuhrTime + TimeZone;
}
//Calculate Fajr Praying time
void calc_Prayer_fajr()
{
	register float D0,D1,D2;
	double Fajr_Arc;
	
	D0 = cos_Dec * cos_Lat;
	D2 = sin_Dec * sin_Lat;
	D1 = degToRad(FajrTwilight);
	D1 = sin(D1);
	D2 = D1 - D2;
	D2 = D2 / D0;
	Fajr_Arc = acos( D2);
	Fajr_Arc = radToDeg(Fajr_Arc);
	Fajr_Arc = Fajr_Arc / 15;
	
	fajrTime = zuhrTime - Fajr_Arc;
}
//Calculate Asr Praying time
void calc_Prayer_asr()
{
	register float D0,D1,D2;
	double Asr_Arc,Asr_Alt;
	
	D0 = Latitude - Dec;
	D0 = abs(D0);
	D0 = degToRad(D0);
	D0 = tan(D0);
	D0 = D0 + 1 ;
	D0 = atan(D0);
	Asr_Alt = radToDeg(D0);
	
	D0 = cos_Dec * cos_Lat;
	D2 = sin_Dec * sin_Lat;
	D1 = 90 - Asr_Alt;
	D1 = degToRad(D1);
	D1 = sin(D1);
	D2 = D1 - D2;
	D2 = D2 / D0;
	Asr_Arc = acos( D2);
	Asr_Arc = radToDeg(Asr_Arc);
	Asr_Arc = Asr_Arc / 15;
	
	asrTime = zuhrTime + Asr_Arc;
}
//Calculate Maghrib Praying time
void calc_Prayer_maghrib()
{
	register float D0,D1,D2;
	double Durinal_Arc;
	
	D0 = cos_Dec * cos_Lat;
	D2 = sin_Dec * sin_Lat;
	D1 = degToRad(-0.8333);
	D1 = sin(D1);
	D2 = D1 - D2;
	D2 = D2 / D0;
	Durinal_Arc = acos( D2);
	Durinal_Arc = radToDeg(Durinal_Arc);
	Durinal_Arc = Durinal_Arc / 15;
	
	maghribTime = zuhrTime + Durinal_Arc;
	
	sunRiseTime = zuhrTime - Durinal_Arc;
}
//Calculate Isha Praying time
void calc_Prayer_isha()
{
	register float D0,D1,D2;
	double isha_Arc;
	
	D0 = cos_Dec * cos_Lat;
	D2 = sin_Dec * sin_Lat;
	D1 = degToRad(IshaTwilight);
	D1 = sin(D1);
	D2 = D1 - D2;
	D2 = D2 / D0;
	isha_Arc = acos( D2);
	isha_Arc = radToDeg(isha_Arc);
	isha_Arc = isha_Arc / 15;
	
	ishaTime = zuhrTime + isha_Arc;
}

//	call this function to calculate praying times 
//	calculating needed constants then call the needed praying time  
void calcPrayerTimes1(uint16 year, uint8 month, uint8 day)
{
	register float D0,D1,D2,latitude_r;
	double L,M,Lambda,Obliquity;
	
	latitude_r = degToRad(Latitude);
	cos_Lat = cos(latitude_r);
	sin_Lat = sin(latitude_r);

	D0 = (30.6 * month);
	D0 = D0 + day;
	D0 = D0 - 730531.5;
	D1 = month + 9;
	D1 = D1 / 12;
	D1 = D1 * 7;
	D1 = D1 / 4;
	D1 = D1 - D0;
	D2 = 365.25 * year;
	D = D2 - D1;////////////////////////////////////////////////////////////////
	
	L = 0.9856474 * D;
	L = 280.461 + L;
	L = moreLess360(L);//////////////////////////////////////////////////
	
	M = 0.9856003 * D;
	M = 357.528 + M;
	M = moreLess360(M);//////////////////////////////////////////////////
	
	D0 = degToRad(2 * M);
	D0 = sin(D0);
	D0 = 0.02 * D0;
	D1 = degToRad(M);
	D1 = sin(D1);
	D1 = 1.915 * D1;
	D1 = D1 + D0;
	Lambda = L + D1;
	Lambda = moreLess360(Lambda);//////////////////////////////////////
	
	Obliquity = 0.0000004 * D;
	Obliquity = 23.439 - Obliquity;///////////////////////////////////
	
	D0 = degToRad(Obliquity);
	D0 = cos(D0);
	D1 = degToRad(Lambda);
	D1 = tan(D1);
	D2 = D0 * D1 ;
	Alpha = atan (D2);
	Alpha = radToDeg(Alpha);
	Alpha = moreLess360(Alpha);///////////////////////////////////
	
	D0 = (int)(Alpha / 90);
	D1 = (int)(Lambda / 90);
	D1 = D1 - D0 ;
	D1 = 90 * D1;
	Alpha = Alpha + D1 ;///////////////////////////////////////
	
	D0 = degToRad(Lambda);
	D0 = sin(D0);
	D1 = degToRad(Obliquity);
	D1 = sin(D1);
	D1 = D1 * D0;
	D1 = asin(D1);
	
	sin_Dec = sin(D1);
	cos_Dec = cos(D1);
	Dec = radToDeg(D1);  ///////////////////////////////////
	
	calc_Prayer_zuhr();
	calc_Prayer_fajr();
	calc_Prayer_asr();
	calc_Prayer_maghrib();
	calc_Prayer_isha();
	
	return;
}

#endif /* PRAYING_TIMES_H_ */