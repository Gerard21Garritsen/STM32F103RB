/*This project is an example of how to configure USART0
 * with a baud rate of 115200 bps*/
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "stm32f10x.h"

//configure RCC clock to reach 72 MHz, using HSE with PLL x 9
void RCC_ClockInitialize(void);

//configure SysTick timer to oveflows each 1 ms
void SysTick_Initialize(void);

void Delay_ms(uint16_t time);

//configure USART0 working t 115200 bps
void USART0_Initialize(void);

void USART0_WriteString(char cadena[25]);




int main(void)
{
	char buffer[25];
	double valor = 3.141;

	RCC_ClockInitialize();
	SysTick_Initialize();
	USART0_Initialize();
	//sprintf(buffer, "El valor es: %.3f", valor);
	USART0_WriteString(buffer);


	while(1)
	{


	}

	return 0;

}


void RCC_ClockInitialize(void)
{
	FLASH_SetLatency(FLASH_Latency_2);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	RCC_DeInit(); //reset RCC
	RCC_HSEConfig(RCC_HSE_ON); //enable HSE
	while(RCC_WaitForHSEStartUp() != SUCCESS);

	RCC_SYSCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //configure ADC prescaler with 6
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //PLL clock source comes from HSE
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //select PLL as main clock source for core and peripherals
	RCC_PLLCmd(ENABLE); //turn on PLL
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET);
	while(RCC_GetSYSCLKSource() != 0x08); //wait until PLL is main CLK source

}


void SysTick_Initialize(void)
{
	SysTick -> CTRL = (uint32_t)(0x0);
	SysTick -> CTRL |= (uint32_t)(1 << 2);
	SysTick -> LOAD = 72000000/1000;
	SysTick -> CTRL |= (uint32_t)(0x1); //turn on SysTick timer

}


void Delay_ms(uint16_t time)
{
	SysTick -> VAL = (uint32_t)(0x0);
	SysTick -> CTRL &= ~(1 << 16); //clear flag

	for(uint16_t i = 0; i < time; i++)
	{
		while(!(SysTick -> CTRL & (uint32_t)(1 << 16)));

		SysTick -> CTRL &= ~(1 << 16); //clear flag

	}

}


void USART0_Initialize(void)
{
	//configure GPIO pins for work with USART1

	//enable GPIOA and USART1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitTypeDef GPIOA_StructureInit; //GPIOA structure

	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_9; //Pin PA9 is Tx
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOA_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_10; //pin PA10 is Rx
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

	//configure USART1
	USART_InitTypeDef USART1_StructureInit;

	USART_DeInit(USART1);
	USART1_StructureInit.USART_WordLength = USART_WordLength_8b;
	USART1_StructureInit.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //Full duplex
	USART1_StructureInit.USART_StopBits = USART_StopBits_1;
	USART1_StructureInit.USART_Parity = USART_Parity_No; //sin paridad
	USART1_StructureInit.USART_BaudRate = 115200;
	USART1_StructureInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(USART1, &USART1_StructureInit);

	USART_Cmd(USART1, ENABLE); //turn on USART1

}


void USART0_WriteString(char *cadena)
{
	uint8_t i = 0; //indice recorre la cadena

	while(*cadena != '\0')
	{
		USART_SendData(USART1, *cadena);

		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);

		cadena++;

	}


}
