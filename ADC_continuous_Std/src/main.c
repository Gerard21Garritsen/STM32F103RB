//This is an example of how to configure ADC on Continuous Mode
#include <stdio.h>
#include "stm32f10x.h"

#define CPU_FREQ 72000000 //AHB bus clock frequency is 72 MHz

//configure RCC clock to working at 72 MHz
void RCC_ClockConfig(void);

//configure SysTick timer to oveflows each 1 ms without ISR
void SysTick_Initialize(void);

//function to make some delays take as base time SysTick timer
void Delay_ms(uint16_t time);

//configure pin PA5 as GPIO Push-pull Output (onboard Led)
void GPIO_Config(void);

//configure USART with 8 bit lenght, no Parity, 1 Stop bit and 9600 bauds
void USART1_Initialize(void);

//function to send a char through USART1
void USART1_WriteChar(char caracter);

//function to send a string through USART1
void USART1_WriteString(char* cadena);

//configure ADC on Continuous Mode with Regular Channel (ADC_IN4 PA4)
void ADC_Initialize(void);

//function to read ADC result conversion
uint16_t ADC_GetConversion(void);



int main(void)
{
	//global variables
	char buffer[25];
	float res = 0.0;

	RCC_ClockConfig();
	SysTick_Initialize();
	GPIO_Config();
	USART1_Initialize();
	ADC_Initialize();



	while(1)
	{
		res = ADC_GetConversion() * 3.3/4096.0;

		sprintf(buffer, "ADC Conversion:%.3f\n\r", res);
		USART1_WriteString(buffer);
		Delay_ms(500);

	}

	return 0;
}


void RCC_ClockConfig(void)
{
	//Reset to their default state CFGR register and disable PLL
	RCC_DeInit();
	RCC_PLLCmd(DISABLE);

	//Enable HSE clock (8 MHz) and wait for it
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_WaitForHSEStartUp() != SUCCESS);

	//Enable Prefetch Buffer and set 2 cycles of Latency
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);

	//Configure AHB bus, APB1, APB2 and ADC prescalers
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);

	//Select PLL clock source, select multiply factor an enable it
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);

	//Switch to PLL as main clock source
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);

}


void SysTick_Initialize(void)
{
	SysTick -> CTRL = (uint32_t)(0x4); //select core clock as clock source
	SysTick -> VAL = (uint32_t)(0x0);
	SysTick -> LOAD = CPU_FREQ/1000;

	SysTick -> CTRL |= (uint32_t)(0x1); //turn on SysTick

}


void Delay_ms(uint16_t time)
{
	SysTick -> VAL = (uint32_t)(0x0);
	SysTick -> CTRL &= ~(uint32_t)(1 << 16);

	for(uint32_t i = 0; i < time; i++)
	{
		while(!(SysTick -> CTRL & (uint32_t)(0x10000)));

		SysTick -> CTRL &= ~(uint32_t)(0x10000);

	}

}


void GPIO_Config(void)
{
	//Enable PORTA clock
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN, ENABLE);

	//Initialize GPIOA structure and configure pin PA5
	GPIO_InitTypeDef GPIOA_StructureInit;

	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_5;
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOA_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

	//configure pins PA9 and PA10 for USART1 (Tx and Rx)
	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_9;
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_10;
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

	//configure pin PA4 for ADC_4 channel
	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_4;
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_AIN; //Analog Input

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

}


void USART1_Initialize(void)
{
	//Enable USART1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_USART1EN, ENABLE);

	//reset USART1 registers
	USART_DeInit(USART1);

	//Initialize USART1 structure
	USART_InitTypeDef USART1_StructureInit;

	USART1_StructureInit.USART_WordLength = USART_WordLength_8b;
	USART1_StructureInit.USART_Parity = USART_Parity_No;
	USART1_StructureInit.USART_StopBits = USART_StopBits_1;
	USART1_StructureInit.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART1_StructureInit.USART_BaudRate = 9600;
	USART1_StructureInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(USART1, &USART1_StructureInit);

	//turn on USART1
	USART_Cmd(USART1, ENABLE);

}

void USART1_WriteChar(char caracter)
{
	USART_SendData(USART1, caracter);

	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);

}


void USART1_WriteString(char* cadena)
{
	while(*cadena)
	{
		USART1_WriteChar(*cadena);

		cadena++;

	}

}


void ADC_Initialize(void)
{
	//Enable ADC clock
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN, ENABLE);

	//reset ADC1 registers
	ADC_DeInit(ADC1);

	//configure ADC
	ADC_InitTypeDef ADC_StructureInit;

	ADC_StructureInit.ADC_Mode = ADC_Mode_Independent; //Independent Mode
	ADC_StructureInit.ADC_NbrOfChannel = 4;
	ADC_StructureInit.ADC_ContinuousConvMode = ENABLE; //enable Continuous Mode
	ADC_StructureInit.ADC_ScanConvMode = DISABLE; //disable Scan Mode
	ADC_StructureInit.ADC_DataAlign = ADC_DataAlign_Right; //ADC result right justified

	ADC_Init(ADC1, &ADC_StructureInit);

	//configure sample time and rank for Regular Channels
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);

	//turn on ADC
	ADC_Cmd(ADC1, ENABLE);

	//Begin Calibration process
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) != RESET);

	//turn on ADC again
	ADC_Cmd(ADC1, ENABLE);


}


uint16_t ADC_GetConversion(void)
{
	uint16_t conv = 0;

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);

	conv = ADC_GetConversionValue(ADC1);

	return conv;

}
