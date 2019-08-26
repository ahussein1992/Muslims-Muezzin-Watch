/***************************************************************************************************
15.0: Initial version 
***************************************************************************************************/

#include "rtc.h"
#include "i2c_master.h"

/***************************************************************************************************
                         void RTC_Init(void)
****************************************************************************************************
 * I/P Arguments: none.
 * Return value    : none

 * description :This function is used to Initialize the Ds1307 RTC.
***************************************************************************************************/
void RTC_Init(void)
{
    i2c_init();                             // Initialize the I2c module.
    i2c_start(C_Ds1307WriteMode_U8);        // Start I2C communication

    i2c_write(C_Ds1307ControlRegAddress_U8);// Select the Ds1307 ControlRegister to configure Ds1307

    i2c_write(0x00);                        // Write 0x00 to Control register to disable SQW-Out

    i2c_stop();                             // Stop I2C communication after initializing DS1307
}

/***************************************************************************************************
                    void RTC_SetDateTime(rtc_t *rtc)
****************************************************************************************************
 * I/P Arguments: rtc_t *: Pointer to structure of type rtc_t. Structure contains hour,min,sec,day,date,month and year 
 * Return value    : none

 * description  :This function is used to update the Date and time of Ds1307 RTC.
                 The new Date and time will be updated into the non volatile memory of Ds1307.
        Note: The date and time should be of BCD format, 
             like 0x12,0x39,0x26 for 12hr,39min and 26sec.    
                  0x15,0x08,0x47 for 15th day,8th month and 47th year.                 
***************************************************************************************************/
void RTC_SetDateTime(rtc_t *rtc)
{
    i2c_start(C_Ds1307WriteMode_U8);                          // Start I2C communication

    i2c_write(C_Ds1307SecondRegAddress_U8); // Request sec RAM address at 00H

    i2c_write(rtc->sec);                    // Write sec from RAM address 00H
    i2c_write(rtc->min);                    // Write min from RAM address 01H
    i2c_write(rtc->hour);                    // Write hour from RAM address 02H
    i2c_write(rtc->weekDay);                // Write weekDay on RAM address 03H
    i2c_write(rtc->date);                    // Write date on RAM address 04H
    i2c_write(rtc->month);                    // Write month on RAM address 05H
    i2c_write(rtc->year);                    // Write year on RAM address 06h

    i2c_stop();                              // Stop I2C communication after Setting the Date
}

/***************************************************************************************************
                     void RTC_GetDateTime(rtc_t *rtc)
****************************************************************************************************
 * I/P Arguments: rtc_t *: Pointer to structure of type rtc_t. Structure contains hour,min,sec,day,date,month and year 
 * Return value    : none

 * description  :This function is used to get the Date(d,m,y) from Ds1307 RTC.

    Note: The date and time read from Ds1307 will be of BCD format, 
          like 0x12,0x39,0x26 for 12hr,39min and 26sec.    
               0x15,0x08,0x47 for 15th day,8th month and 47th year.              
***************************************************************************************************/
void RTC_GetDateTime(rtc_t *rtc)
{
    i2c_start(C_Ds1307WriteMode_U8);                           // Start I2C communication

    i2c_write(C_Ds1307SecondRegAddress_U8); // Request Sec RAM address at 00H

    i2c_stop();                                // Stop I2C communication after selecting Sec Register

    i2c_start(C_Ds1307ReadMode_U8);                            // Start I2C communication
    
    rtc->sec = i2c_read_ack();                // read second and return Positive ACK
    rtc->min = i2c_read_ack();                 // read minute and return Positive ACK
    rtc->hour= i2c_read_ack();               // read hour and return Positive ACK
    rtc->weekDay = i2c_read_ack();           // read weekDay and return Positive ACK
    rtc->date= i2c_read_ack();              // read Date and return Positive ACK
    rtc->month=i2c_read_ack();            // read Month and return Positive ACK
    rtc->year =i2c_read_nack();             // read Year and return Negative/No ACK

    i2c_stop();                              // Stop I2C communication after reading the Date
}
