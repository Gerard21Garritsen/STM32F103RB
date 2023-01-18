//This is the source file which has all functions to control I2C peripheral
#include "I2C.h"

void I2C1_Initialize(void)
{
	RCC -> APB1ENR |= (uint32_t)(1 << 21);

	I2C1 -> CR2 |= (uint32_t)(0x24); //set up APB1 works with 36 MHz
	I2C1 -> CCR = (uint32_t)(0xB6); //load to reach 100 KHz
	I2C1 -> TRISE = (uint32_t)(37); //load to define SCL feedback loop
	I2C1 -> CR1 &= ~(uint32_t)(1 << 1); //select I2C Mode
	I2C1 -> CR1 |= (uint32_t)(0x1); //enable I2C1

}


void I2C_Start(void)
{
	//verify if bus is busy
	while(I2C1 -> SR2 & I2C_SR2_BUSY);

	I2C1 -> CR1 |= I2C_CR1_START;
	while(!(I2C1 -> SR1 & I2C_SR1_SB));

}


uint8_t I2C_WriteData(uint8_t data)
{
	uint8_t estado = 0x00;

	I2C1 -> DR = data;
	while(!(I2C1 -> SR1 & I2C_SR1_TXE))
	{
		estado = I2C_Check_ACK();

		if(estado != 0x00)
			break;

	}

	return estado;
}


uint8_t I2C_Check_ACK(void)
{
	uint8_t state;

	//if we have an ACK returned
	if(!(I2C1 -> SR1 & I2C_SR1_AF))
		state = 0x00;
	else
		state = 0xFF;

	return state;
}


void I2C_Send_ACK(uint8_t estado)
{
	//send ACK or NACK in an slave reception data
	if(estado)
		I2C1 -> CR1 |= I2C_CR1_ACK;
	else
		I2C1 -> CR1 &= ~(I2C_CR1_ACK);

}


void I2C_Restart(void)
{
	//set START bit
	I2C1 -> CR1 |= I2C_CR1_START;
	while(!(I2C1 -> SR1 & I2C_SR1_SB));


}


void I2C_Stop(void)
{
	//send Stop signal
	I2C1 -> CR1 |= I2C_CR1_STOP;
	while(!(I2C1 -> SR1 & I2C_SR1_STOPF));


}


void I2C1_Write1Byte(uint8_t address, uint8_t data, unsigned char I2C_End_Mode)
{
	uint8_t status;

	//send Start
	I2C_Start();

	//send address
	status = I2C_WriteData(address);

	if(status == 0x00)
		I2C_WriteData(data);

	if(I2C_End_Mode && status == 0x00)
		I2C_Restart();
	else
		I2C_Stop();

}


void I2C1_WriteNBytes(uint8_t address, uint8_t num, uint8_t* datas, unsigned char I2C_End_Mode)
{



}


uint8_t I2C_Read1Byte(uint8_t address, unsigned char I2C_End_Mode)
{
	uint8_t data;


	return data;
}


uint8_t* I2C_ReadNBytes(uint8_t address, uint8_t num, unsigned char I2C_End_Mode)
{
	uint8_t* datos;


	return datos;
}
