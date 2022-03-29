//This project is an example of how to configure GPIO inputs with Std Libraries
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

void GPIOA_Initialize(void); //Function to initialize all PORTA pins

void GPIOB_Initialize(void); //Function to initialize all PORTB pins

int main(void)
{
	GPIOA_Initialize();
	GPIOB_Initialize();

	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0x00) //read pin state
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET); //if onboard button is pressed turn on the led
		else
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);

	}

	return 0;
}

void GPIOA_Initialize(void)
{
	//Enable PORTA clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//Configure pin PA5 as output Push-pull
	GPIO_InitTypeDef GPIOA_StructureInit;

	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_5;
	GPIOA_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

}

void GPIOB_Initialize(void)
{
	//Enable PORTB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//configure pin PC13 as input with Pull-up internal resistor
	GPIO_InitTypeDef GPIOC_StructureInit;

	GPIOC_StructureInit.GPIO_Mode = GPIO_Mode_IPU;
	GPIOC_StructureInit.GPIO_Pin = GPIO_Pin_13;

	GPIO_Init(GPIOC, &GPIOC_StructureInit);

}
