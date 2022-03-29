/*This project is an example of how to configure I/O
 * On these different projects we're gonna working with STM32F103RB board
 * which has one user led and one user button, user led (LD3) is connected in PA5 pin
 * and user button is connected in PC13 pin
*/
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"
			
void Delay_ms(uint16_t value); //delay ms function


int main(void)
{
	//Enable PORTA clock
	RCC -> APB2ENR |= 1 << 2;

	//configure pin PA5
	GPIOA -> CRL &= 0x00000000;
	GPIOA -> CRL |= 0x00200000;


	while(1)
	{
		//blink code
		GPIOA -> ODR |= 0x00000020;
		Delay_ms(100);
		GPIOA -> ODR &= 0x00000000;
		Delay_ms(100);

	}

	return 0;
}

void Delay_ms(uint16_t value)
{

	for(uint32_t i = 0; i < value * 4000; i++)
		asm("NOP");

}
