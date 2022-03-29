/*This project is an example of how to configure USART1
 * with a baud rate of 9600 bauds
 */
#include "stm32f10x.h"

//configure RCC clock using HSE directly to feed core and peripherals (8 MHz)
void RCC_ClockConfig(void);

//configure SysTick timer to overflows each 100 ms
void SysTick_Initialize(void);

//configure USART1 to work with 9600 bauds
void USART1_Config(void);

void USART1_Write1Byte(unsigned char dato);

//this function generate delays using SysTick timer
void delay_ms(uint16_t time);




int main(void)
{
	RCC_ClockConfig();
	SysTick_Initialize();
	USART1_Config();



	while(1)
	{
		USART1_Write1Byte('G');
		delay_ms(500);

	}

	return 0;

}


void RCC_ClockConfig(void)
{
	//switch off PLL and reset CFGR registers
	RCC -> CFGR = (uint32_t)(0x0);
	RCC -> CR &= ~(uint32_t)(1 << 24);
	RCC -> CFGR = (uint32_t)(0x0);

	//configure Wait States depend on CPU speed
	FLASH -> ACR = (uint32_t)(0x10); //enable prefetch and zero wait states

	//turn on HSE
	RCC -> CR |= (uint32_t)(1 << 16);
	while(!(RCC -> CR & (uint32_t)(1 << 17)));

	//configure prescalers and switch to HSE clock as main clock
	RCC -> CFGR |= (uint32_t)(0x1); //witch to HSE clock
	while(!(RCC -> CFGR & (uint32_t)(0x5)));


}


void SysTick_Initialize(void)
{
	//reset CTRL register
	SysTick -> CTRL = (uint32_t)(0x0);

	SysTick -> CTRL |= (uint32_t)(0x4); //select internal clock as main clock
	SysTick -> LOAD = (uint32_t)(8000000/1000); //load buffer to temporize 1 ms
	SysTick -> VAL = (uint32_t)(0x0); //reset value to zero
	SysTick -> CTRL |= (uint32_t)(0x1); //turn on SysTick Timer

}


void USART1_Config(void)
{
	//configure USART1 GPIO pins PA9 (Tx) and PA10(Rx)
	RCC -> APB2ENR |= (1 << 2) | (1 << 14); //enable GPIOA and USART1 clocks

	//configure pins PA9 as Alternate Push-pull and PA10 as Input with Pull-up
	GPIOA -> CRH = 0x444448A4;
	GPIOA -> IDR |= (uint32_t)(1 << 10); //enable PA10 pin Pull-up resistor

	//Configure USART1 peripheral
	//enable USART1 and enable Tx
	USART1 -> BRR = (uint32_t)(52 << 4) | (uint32_t)(0x2); //load BRR to work USART with 9600 bps
	USART1 -> CR1 |= (uint32_t)(1 << 13) | (uint32_t)(0xC);
	USART1 -> SR = (uint32_t)(0x0); //reset Status register

}

void USART1_Write1Byte(unsigned char dato)
{
	USART1 -> DR = dato;

	while(!(USART1 -> SR & (1 << 6))); //wait until TXE bit is set

}


void delay_ms(uint16_t time)
{
	SysTick -> VAL = (uint32_t)(0x0); //reset counter
	SysTick -> CTRL &= ~(1 << 16); //clear flag

	for(uint32_t i = 0; i < time; i++)
	{
		while(!(SysTick -> CTRL & (uint32_t)(0x10000)));

		SysTick -> CTRL &= ~(1 << 16); //clear flag

	}


}
