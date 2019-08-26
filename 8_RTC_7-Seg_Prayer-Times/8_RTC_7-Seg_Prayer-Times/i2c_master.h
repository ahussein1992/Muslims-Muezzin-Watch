#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#define I2C_READ 0x01
#define I2C_WRITE 0x00

#include "stdutils.h"

void i2c_init(void);
uint8 i2c_start(uint8 address);
uint8 i2c_write(uint8 data);
uint8 i2c_read_ack(void);
uint8 i2c_read_nack(void);
uint8 i2c_transmit(uint8 address, uint8* data, uint16 length);
uint8 i2c_receive(uint8 address, uint8* data, uint16 length);
uint8 i2c_writeReg(uint8 devaddr, uint8 regaddr, uint8* data, uint16 length);
uint8 i2c_readReg(uint8 devaddr, uint8 regaddr, uint8* data, uint16 length);
void i2c_stop(void);

#endif // I2C_MASTER_H
