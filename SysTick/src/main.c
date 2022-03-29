/*This project is an example of how to configure SysTick Timer
 * to overflows each 1ms and use it to make delays*/
#include "stm32f10x.h"

//configure RCC clock to use HSE with PLL x 9
void RCC_ClockInit(void);

void GPIO_Initialize(void);

void SysTick_Init(void);

void Delay_ms(uint16_t time);




int main(void)
{
	RCC_ClockInit();
	GPIO_Initialize();
	SysTick_Init();



	while(1)
	{
		GPIOA -> ODR ^= (uint32_t)(1 << 5); //switch pin PA5 state
		Delay_ms(100);

	}

	return 0;

}


void RCC_ClockInit(void)
{
	FLASH -> ACR |= (uint32_t)(0x12);

	RCC -> CFGR = (uint32_t)(0x0);
	RCC -> CR &= ~(1 << 24);
	RCC -> CFGR = (uint32_t)(0x0);

	RCC -> CR |= RCC_CR_HSEON;
	while(!(RCC -> CR & RCC_CR_HSERDY));

	RCC -> CFGR |= RCC_CFGR_PLLXTPRE_HSE;
	RCC -> CFGR |= RCC_CFGR_PLLSRC_HSE;
	RCC -> CFGR |= RCC_CFGR_PLLMULL9;
	RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2;
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1;
	RCC -> CFGR |= RCC_CFGR_ADCPRE_DIV6;

	RCC -> CR |= RCC_CR_PLLON;
	while(!(RCC -> CR & RCC_CR_PLLRDY));

	RCC -> CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC -> CFGR & RCC_CFGR_SWS_PLL));

}


void GPIO_Initialize(void)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA -> CRL = 0x44244444;

}


void SysTick_Init(void)
{
	SysTick -> CTRL = (uint32_t)(0x0);
	SysTick -> CTRL |= (uint32_t)(1 << 2); //select AHB clock source

	SysTick -> LOAD = 72000000/1000;
	SysTick -> CTRL |= (uint32_t)(0x1); //turn on SysTick

}


void Delay_ms(uint16_t time)
{

	for(uint16_t i = 0; i < time; i++)
	{
		while(!(SysTick -> CTRL & (uint32_t)(1 << 16)));

		SysTick -> CTRL &= ~(1 << 16);

	}

}
