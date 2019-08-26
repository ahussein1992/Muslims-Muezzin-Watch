/*
 * stdutils.h
 * Se7S_92
 * Last modification 8-24-2019
 * for more libraries for AVR Microcontrollers visit https://github.com/ahussein1992/AVR-Libraries
 */ 

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <string.h>

#ifndef _STD_UTIL_H_
#define	_STD_UTIL_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)
 
/***************************************************************************************************
									Common Macros
 ***************************************************************************************************/
/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* Rotate right the register value with specific number of rotates */
#define ROR(REG,num) ( REG= (REG>>num) | (REG<<(8-num)) )

/* Rotate left the register value with specific number of rotates */
#define ROL(REG,num) ( REG= (REG<<num) | (REG>>(8-num)) )

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) ( !(REG & (1<<BIT)) )

/***************************************************************************************************
    Basic data types for 8051/PIC/AVR 8bit controllers
 ***************************************************************************************************/

/**----------char 8-bit--------
     char (-128 to 127)
     signed char (-128 to 127)
     unsigned char (0 - 255)
	-----------------------------*/

/**---------int 16-bit----------
	 int (-32768 to 32767)
	 signed int (-32768 to 32767)
     unsigned int (0 to 65535)
	 -----------------------------*/

typedef unsigned char uint8;
typedef signed char sint8;
typedef unsigned short uint16;
typedef signed short sint16;
typedef unsigned long uint32;
typedef signed long sint32;

#define C_SINT8_MAX   0x7F
#define C_SINT8_MIN  -128

#define C_UINT8_MAX   0xFFu
#define C_UINT8_MIN   0x00u

#define C_SINT16_MAX  32767
#define C_SINT16_MIN -32768

#define C_UINT16_MAX  0xFFFFu
#define C_UINT16_MIN  0x00u

#define C_SINT32_MAX  2147483647
#define C_SINT32_MIN -2147483648

#define C_UINT32_MAX  0xFFFFFFFFu
#define C_UINT32_MIN  0x00u
/***************************************************************************************************/
/***************************************************************************************************
                           Definition of common Bit-Masks
 ***************************************************************************************************/
#define  Mask_BIT0   0x0001u
#define  Mask_BIT1   0x0002u
#define  Mask_BIT2   0x0004u
#define  Mask_BIT3   0x0008u
#define  Mask_BIT4   0x0010u
#define  Mask_BIT5   0x0020u
#define  Mask_BIT6   0x0040u
#define  Mask_BIT7   0x0080u
#define  Mask_BIT8   0x0100u
#define  Mask_BIT9   0x0200u
#define  Mask_BIT10  0x0400u
#define  Mask_BIT11  0x0800u
#define  Mask_BIT12  0x1000u
#define  Mask_BIT13  0x2000u
#define  Mask_BIT14  0x4000u
#define  Mask_BIT15  0x8000u
/**************************************************************************************************/
/***************************************************************************************************
                           Port Direction configurations
 ***************************************************************************************************/
#define C_PinOutput_U8     0x01u
#define C_PinInput_U8      0x00u
#define C_PortOutput_U8    0xFFu
#define C_PortInput_U8     0x00u

#define INPUT              C_PinInput_U8
#define OUTPUT             C_PinOutput_U8
#define LOW                0x00u
#define HIGH               0x01u
/**************************************************************************************************/
/***************************************************************************************************
                              Commonly used constants
 **************************************************************************************************/
#define C_ZERO_U8          0x00u
#define C_NULL_U8          0x00u
#define NULL_CHAR          0x00u
#ifndef NULL
#define NULL 0
#endif

#define C_NOTOK_U8         0x00u
#define C_OK_U8            0x01u

#define C_INVALID_U8       0x00u
#define C_VALID_U8         0x01u

#define C_FAILED_U8        0x00u
#define C_SUCCESSFUL_U8    0x01u
#define C_BUSY_U8          0x02u

#define C_BINARY_U8           2u
#define C_DECIMAL_U8         10u
#define C_HEX_U8             16u
/**************************************************************************************************/
/***************************************************************************************************
                                Standard Enumerations and Constants
***************************************************************************************************/
typedef enum
{
	FALSE,
	TRUE
}Bool;

typedef enum
{
	false,
	true
}bool;

typedef enum
{
    E_FAILED,
    E_SUCCESS,
    E_BUSY,
    E_TIMEOUT
}Status_et;

typedef enum
{
	E_BINARY=2,
	E_DECIMAL = 10,
	E_HEX = 16
}NumericSystem_et;
/**************************************************************************************************/ 
/***************************************************************************************************
                                Macros for Bit Manipulation
 ****************************************************************************************************/
#define  util_GetBitMask(bit)          (1<<(bit))
#define  util_BitSet(x,bit)            ((x) |=  util_GetBitMask(bit))
#define  util_BitClear(x,bit)          ((x) &= ~util_GetBitMask(bit))
#define  util_BitToggle(x,bit)         ((x) ^=  util_GetBitMask(bit))
#define  util_UpdateBit(x,bit,val)     ((val)? util_BitSet(x,bit): util_BitClear(x,bit))


#define  util_GetBitStatus(x,bit)      (((x)&(util_GetBitMask(bit)))!=0u)
#define  util_IsBitSet(x,bit)          (((x)&(util_GetBitMask(bit)))!=0u)
#define  util_IsBitCleared(x,bit)      (((x)&(util_GetBitMask(bit)))==0u)


#define  util_AreAllBitsSet(x,BitMask) (((x)&(BitMask))==BitMask)
#define  util_AreAnyBitsSet(x,BitMask) (((x)&(BitMask))!=0x00u)
/**************************************************************************************************/
/***************************************************************************************************
                             Macros to find the mod of a number
 ***************************************************************************************************/
#define util_GetMod8(dividend,divisor)  (uint8) (dividend - (divisor * (uint8) (dividend/divisor)))
#define util_GetMod16(dividend,divisor) (uint16)(dividend - (divisor * (uint16)(dividend/divisor)))
#define util_GetMod32(dividend,divisor) (uint32_t)(dividend - (divisor * (uint32_t)(dividend/divisor)))
/***************************************************************************************************/
/***************************************************************************************************
                          Macros for Dec2Ascii,Hec2Ascii and Acsii2Hex conversion
 ****************************************************************************************************/
#define util_Dec2Ascii(Dec)	 ((Dec)+0x30)
#define util_Ascii2Dec(Asc)	 ((Asc)-0x30)
#define util_Hex2Ascii(Hex)  (((Hex)>0x09) ? ((Hex) + 0x37): ((Hex) + 0x30)) 
#define util_Ascii2Hex(Asc)  (((Asc)>0x39) ? ((Asc) - 0x37): ((Asc) - 0x30))
/***************************************************************************************************/
/***************************************************************************************************
                     Macros to extract the nibbles
 ***************************************************************************************************/
#define util_ExtractNibble0to4(x)    (uint8) ((x) & 0x0Fu)
#define util_ExtractNibble4to8(x)    (uint8) (((x)>>4)  & 0x0Fu)
#define util_ExtractNibble8to12(x)   (uint8) (((x)>>8)  & 0x0Fu)
#define util_ExtractNibble12to16(x)  (uint8) (((x)>>12) & 0x0Fu)
/**************************************************************************************************/
/***************************************************************************************************
                     Macros to extract the Byte
***************************************************************************************************/
#define util_ExtractByte0to8(x)    (uint8) ((x) & 0xFFu)
#define util_ExtractByte8to16(x)   (uint8) (((x)>>8) & 0xFFu)
#define util_ExtractByte16to28(x)  (uint8) (((x)>>16) & 0xFFu)
#define util_ExtractByte28to32(x)  (uint8) (((x)>>28) & 0xFFu)
/**************************************************************************************************/
/***************************************************************************************************
                      Other Macros
***************************************************************************************************/
#define util_GetMax(num1,num2)     (((num1)>(num2))? (num1): (num2)) 
#define util_GetMin(num1,num2)     (((num1)<(num2))? (num1): (num2)) 

#define util_swap(x,y)             (x ^= y ^= x ^= y)
#define util_GetAbsolute(x)	       (((x) < 0) ? -(x) : (x))
/**************************************************************************************************/

#endif	

