//This project is an example of how to configure HSE with PLL clock using Std Libraries
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

void RCC_Initialize(void);

void GPIOA_Initialize(void);

int main(void)
{
	RCC_Initialize();
	GPIOA_Initialize();

	while(1)
	{


	}

	return 0;
}

void RCC_Initialize(void)
{
	//Switch off PLL and Reset CR clock
	RCC_PLLCmd(DISABLE);
	RCC_DeInit(); //reset CR register to their default mode

	//Turn on HSE clock
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_WaitForHSEStartUp() != SUCCESS);

	//configure Flash latency with 2 states and enable Prefetch buffer
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);

	//Configure Prescalers
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	RCC_HCLKConfig(RCC_SYSCLK_Div1); //Prescaler AHB is one
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //select HSE as PLL source and PLL x 9
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //select PLL as clock source

	//select out MCO pin PLLCLK/2
	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);

}

void GPIOA_Initialize(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_StructureInit;

	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_8;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_StructureInit);

}
