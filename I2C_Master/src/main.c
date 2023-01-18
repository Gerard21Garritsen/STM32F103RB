/*This project is an example of how to configure I2C Bare Metal*/
#include "stm32f10x.h"
#include "I2C.h"

#define Core_Clock 72000000 //CPU clock frequency

void RCC_ClockInitialize(void);

void GPIO_Initialize(void);

void SysTick_Initialize(void);

void Delay_ms(uint16_t time);




int main(void)
{
	RCC_ClockInitialize();
	SysTick_Initialize();
	GPIO_Initialize();
	I2C1_Initialize();

	I2C1_Write1Byte(0x70, 0xA0, 0x00);

	GPIOA -> BSRR |= (uint32_t)(1 << 5);



	while(1)
	{



	}



}


void RCC_ClockInitialize(void)
{
	//reset CR and CFGR registers
	RCC -> CFGR = (uint32_t)(0x00);
	RCC -> CR &= ~(1 << 24); //turn off PLL
	RCC -> CR &= ~(0x010D0000);
	RCC -> CFGR = (uint32_t)(0x00);

	//Turn on HSE clock
	RCC -> CR |= (1 << 16);
	while(!(RCC -> CR & 0x00020000));

	//disable Prefetch buffer and set up to 2 wait states
	FLASH -> ACR &= ~( 1 << 4);
	while(FLASH -> ACR & 0x00000010);
	FLASH -> ACR |= (uint32_t)(0x02);
	FLASH -> ACR |= (uint32_t)(0x10);
	while(!(FLASH -> ACR & 0x00000010));

	/*set up AHB prescaler with one, APB1 with 2, APB2 with one, ADC with 6
	 *PLL source comes from HSE divide by one and PLLMUL 9*/
	RCC -> CFGR |= (uint32_t)(0x7 << 18); //PLLMUL 9
	RCC -> CFGR |= (uint32_t)(0x1 << 16); //PLL source comes from HSE
	RCC -> CFGR |= (uint32_t)(0x4 << 8); //APB1 divided by 2
	RCC -> CFGR |= (uint32_t)(0x2 << 14); //ADC divided by 6

	//Turn on PLL
	RCC -> CR |= (uint32_t)(1 << 24);
	while(!(RCC -> CR & (uint32_t)(1 << 25)));

	//Switch to HSE clock
	RCC -> CFGR |= (uint32_t)(0x2);
	while(!(RCC -> CFGR & (uint32_t)(0x2 << 2)));

}


void GPIO_Initialize(void)
{
	//enable AFIO clock
	RCC -> APB2ENR |= (uint32_t)(0x01);

	//configure PA5 as Digital output Push-pull
	RCC -> APB2ENR |= (uint32_t)(1 << 2);
	GPIOA -> CRL = 0x44344444;

	//switch PB6 and PB7 to PB8 and PB9 pins (SCL and SDA)
	AFIO -> MAPR |= (uint32_t)(0x2);

	//configure PB8 and PB9 as Open-Drain with Alternate Function (SCL and SDA)
	RCC -> APB2ENR |= (uint32_t)(1 << 3);
	GPIOB -> CRH = 0x444444FF;

	//clear PA5
	GPIOA -> BSRR |= (uint32_t)(1 << 20);

}


void SysTick_Initialize(void)
{
	/*set up clock SysTick source from AHB bus
	 *load to overflow each 1 ms and turn on it*/
	SysTick -> CTRL |= (uint32_t)(1 << 2);
	SysTick -> CTRL &= ~(uint32_t)(1 << 16);
	SysTick -> LOAD = Core_Clock/1000;
	SysTick -> CTRL |= (uint32_t)(0x1);

}


void Delay_ms(uint16_t time)
{
	SysTick -> VAL = (uint32_t)(0x0);

	for(uint32_t i = 0; i < time; i++)
	{
		while(!(SysTick -> CTRL & (uint32_t)(1 << 16)));

		SysTick -> CTRL &= ~(1 << 16);

	}

}
