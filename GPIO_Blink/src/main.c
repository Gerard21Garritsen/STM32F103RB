/*This project is an example of how to configure GPIO pin
 *to make the Blink test project*/
#include "stm32f10x.h"

#define RCC_AHB_Clock 72000000 //set AHB clock and peripherals

void RCC_ClockConfigure(void);

void SysTick_Initialize(void);

void GPIO_Initialize(void);

void Delay_ms(uint16_t time);



int main(void)
{
	RCC_ClockConfigure();
	SysTick_Initialize();
	GPIO_Initialize();


	while(1)
	{
		GPIOA -> ODR ^= (uint32_t)(1 << 5);
		Delay_ms(100);

	}

	return 0;
}


void RCC_ClockConfigure(void)
{
	//switch off PLL and reset CFGR register
	RCC -> CFGR &= ~(uint32_t)(0x3);
	RCC -> CR &= ~(RCC_CR_PLLON);
	RCC -> CR &= ~(RCC_HSE_ON);
	RCC -> CFGR &= ~(uint32_t)(0xFFFFFFFF);

	//switch off Prefetch buffer and change it to 2 wait states
	FLASH -> ACR &= ~(FLASH_ACR_PRFTBE);
	FLASH -> ACR &= ~(FLASH_ACR_HLFCYA);
	FLASH -> ACR &= ~(FLASH_ACR_LATENCY_2);
	FLASH -> ACR |= FLASH_ACR_LATENCY_2;
	FLASH -> ACR |= FLASH_ACR_PRFTBE;
	while(!(FLASH -> ACR & FLASH_ACR_PRFTBS));

	//turn on HSE clock
	RCC -> CR |= RCC_CR_HSEON;
	while(!(RCC -> CR & RCC_CR_HSERDY));

	//turn on CSS detector
	RCC -> CR |= RCC_CR_CSSON;

	//configure Prescalers
	RCC -> CFGR |= RCC_CFGR_HPRE_DIV1;
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2; //prescaler APB1 is 2 to get 36 MHz
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1; //prescaler APB2 is 1 to get 72 MHz
	RCC -> CFGR |= RCC_CFGR_ADCPRE_DIV6; //prescaler ADC is 6 to get 12 MHz
	RCC -> CFGR |= RCC_CFGR_PLLSRC_HSE;
	RCC -> CFGR |= RCC_CFGR_PLLMULL9; //set PLL 9 to get 72 MHz

	//turn on PLL
	RCC -> CR |= RCC_CR_PLLON;
	while(!(RCC -> CR & RCC_CR_PLLON));

	//switch to PLL as AHB clock source
	RCC -> CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC -> CFGR & RCC_CFGR_SWS_PLL));



}


void SysTick_Initialize(void)
{
	//switch off SysTick and clear flag
	SysTick -> CTRL &= ~(SysTick_CTRL_ENABLE);
	SysTick -> CTRL |= SysTick_CTRL_CLKSOURCE;
	SysTick -> LOAD = RCC_AHB_Clock/1000;
	SysTick -> VAL = (uint32_t)(0x00);

	//turn on SysTick Timer
	SysTick -> CTRL |= SysTick_CTRL_ENABLE;

}


void GPIO_Initialize(void)
{
	//enable PORTA clock
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN;

	//configure pin PA5 as GPIO Output, 50 MHz and Push-pull
	GPIOA -> CRL &= ~(uint32_t)(1 << 22);
	GPIOA -> CRL |= 0x3 << 20;

	GPIOA -> BSRR |= (uint32_t)(1 << 21); //clear PA5

}


void Delay_ms(uint16_t time)
{
	SysTick -> VAL = (uint32_t)(0x00);

	for(uint16_t i = 0; i < time; i++)
	{
		while(!(SysTick -> CTRL & SysTick_CTRL_COUNTFLAG));

		SysTick -> CTRL &= ~(SysTick_CTRL_COUNTFLAG);

	}

}
