//This is the header file which has header functions to control I2C peripheral

#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"


void I2C1_Initialize(void);

void I2C_Start(void);

uint8_t I2C_Check_ACK(void);

void I2C_Send_ACK(uint8_t estado);

void I2C_Restart(void);

void I2C_Stop(void);

void I2C1_Write1Byte(uint8_t address, uint8_t data, unsigned char I2C_End_Mode);

void I2C1_WriteNBytes(uint8_t address, uint8_t num, uint8_t* datas, unsigned char I2C_End_Mode);

uint8_t I2C_Read1Byte(uint8_t address, unsigned char I2C_End_Mode);

uint8_t* I2C_ReadNBytes(uint8_t address, uint8_t num, unsigned char I2C_End_Mode);


#endif /* I2C_H_ */
