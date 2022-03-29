//This is an example project of how to make blink project using Std Peripheral Libraires
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
			
void GPIOA_Initialize(void);

void Delay_ms(uint16_t time);

int main(void)
{
	GPIOA_Initialize(); //call PORTA initialize structure

	while(1)
	{
		//Blink routine
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET); //set on high pin PA5
		Delay_ms(100); //make a delay for 100 ms
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET); //set on low pin PA5
		Delay_ms(100);

	}

	return 0;
}

void GPIOA_Initialize(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //enable PORTA clock

	GPIO_InitTypeDef GPIOA_StructureInit; //initialize structure

	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP; //pin as output with Push-pull
	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_5; //pin 5
	GPIOA_StructureInit.GPIO_Speed = GPIO_Speed_2MHz; //configure slew rate pin as 2 MHz

	GPIO_Init(GPIOA, &GPIOA_StructureInit); //initialize structure

}

void Delay_ms(uint16_t time)
{
	for(uint32_t i = 0; i < time * 4000; i++)
		asm("NOP");

}

