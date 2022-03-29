/*This project is an example of how to configure External Interrupts programming Bare Metal
 * On this project also we configured two external interrupts with different priority
*/
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

void RCC_Initialize(void); //function to configure HSE clock (8 MHz) and use it to feed CPU clock and peripherals

void GPIO_Initialize(void); //function to configure pins PA5, PC13 and PB1

void Interrupt_Initialize(void); //function to configure External Interrupt on pin PC13 and PB1

void Delay_ms(uint16_t time); //function to make software delays

void EXTI1_IRQHandler(void); //Interrupt routine for EXTI1

void EXTI15_10_IRQHandler(void); //Interrupt routine for EXTI15 - 10

int main(void)
{
	RCC_Initialize();
	GPIO_Initialize();
	Interrupt_Initialize();

	while(1)
	{
		for(uint8_t i = 0; i < 100; i++)
			asm("NOP");
	}

	return 0;

}

void RCC_Initialize(void)
{
	//Reset RCC_CFGR register and switch off PLL
	RCC -> CFGR = 0x00000000; //reset CFGR register
	RCC -> CR &= ~(RCC_CR_PLLON); // switch off PLL
	RCC -> CFGR = 0x00000000; //reset CFGR register

	//Enable HSE clock
	RCC -> CR |= RCC_CR_HSEON;
	while(!(RCC -> CR & RCC_CR_HSERDY) );

	//select HSE clock as source of CPU clock
	RCC -> CFGR |= 0x1;

	//verify if HSE is CPU clock
	while(!(RCC -> CFGR & RCC_CFGR_SWS_HSE));

}

void GPIO_Initialize(void)
{
	//Enable PORTA, PORTB and PORTC clock
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN;

	//Configure Pin PA5 as output Push-pull, PC13 and PB1 as input with Pull-up
	GPIOA -> CRL &= (uint32_t)(0x0 << 20); //reset PA5 state
	GPIOB -> CRL &= (uint32_t)(0x0 << 4); //reset PB1 state
	GPIOC -> CRH &= (uint32_t)(0x0 << 20); //reset PC13 state

	GPIOA -> CRL |= (uint32_t)(0x2 << 20);
	GPIOB -> CRL |= (uint32_t)(0x8 << 4);
	GPIOC -> CRH |= (uint32_t)(0x8 << 20);

	GPIOC -> ODR |= (uint32_t)(1 << 13); //Enable Pull-up on PC13
	GPIOB -> ODR |= (uint32_t)(1 << 1); //Enable Pull-up on PB1
	GPIOA -> BSRR |= 1 << 21; //set on low pin PA5

}

void Interrupt_Initialize(void)
{
	//enable AFIO clock to be able to use EXTI peripheral
	RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN;

	//define pin PC13 as EXTI13 line source and PB1 as EXTI1 line
	AFIO -> EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;
	AFIO -> EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB;

	//configure priority group 2 to use 2 bits for priority and 2 bits for subpriority
	SCB -> AIRCR =  (uint32_t) (0x05FA0000) | (uint32_t) (0x500);

	/*configure priority 0,1 and subpriority 0,0 for PC13 and PB1*/
	NVIC -> IP[40] |= (0x00); //priority for PC13 IRQ 40
	NVIC -> IP[7] |= (0x40); //priority for PB1 IRQ 7

	//configure EXTI interrupt and EXTI line
	EXTI -> FTSR |= (uint32_t) (1 << 13) | (uint32_t) (1 << 1); //enable interrupt on falling edge
	EXTI -> IMR |= (uint32_t) (1 << 13) | (uint32_t) (1 << 1); //enable interrupt on line EXTI 13 and EXTI1

	//configure NVIC
	NVIC -> ISER[0] = (uint32_t)(1 << 7); //enable interrupt 7 (EXTI1)
	NVIC -> ISER[1] = (uint32_t)(1 << 8); //enable interrupt 40 (EXTI15_10)

}

void Delay_ms(uint16_t time)
{
	for(uint32_t i = 0; i < time * 500; i++)
		asm("NOP");

}

void EXTI1_IRQHandler(void)
{
	if((EXTI -> PR & (uint32_t)(1 << 1)) != (uint32_t)(0x00000)) //check if EXTI1 line was trigger
	{
		for(uint8_t i = 0; i < 6; i++)
		{
			GPIOA -> BSRR |= 1 << 5;
			Delay_ms(100);
			GPIOA -> BSRR |= 1 << 21;
			Delay_ms(100);
		}

		EXTI -> PR = (uint32_t)(0x0002); //clear flag
	}

}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI -> PR & (uint32_t)(1 << 13)) != (uint32_t)(0x0000)) //check if EXTI Line 13 was trigger
	{
		//blink 4 times each 100 ms LED2 onboard
		for(uint8_t i = 0; i < 4; i++)
		{
			GPIOA -> BSRR |= 1 << 5;
			Delay_ms(500);
			GPIOA -> BSRR |= 1 << 21;
			Delay_ms(500);

		}

		EXTI -> PR = (uint32_t)(0x02000); //clear flag
	}

}
