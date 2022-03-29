//This project is an example of how to work with digital inputs
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
			

int main(void)
{
	//Enable PORTA and PORTC clock
	RCC -> APB2ENR |= 0x14;

	//Configure pin PA5 as digital output and Push-pull mode
	GPIOA -> CRL &= 0x00000000;
	GPIOA -> CRL |= 0x00200000;
	GPIOA -> ODR &= 0x00000000; //reset PORTA


	//Configure pin PC13 as digital input with internal Pull-up
	GPIOC -> CRH &= 0x000000000;
	GPIOC -> CRH |= 0x00800000; //configure PC13 as input
	GPIOC -> ODR |= 0x00002000; //enable Pull-up resistor


	while(1)
	{
		//read button PC13 state
		if(!(GPIOC -> IDR & 0x00002000))
			GPIOA -> BSRR |= 1 << 5; // set on high if button was pressed
		else
			GPIOA -> BSRR |= 1 << 21; //set on low if button leave pressed

	}

	return 0;
}
