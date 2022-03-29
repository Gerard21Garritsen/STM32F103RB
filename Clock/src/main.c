//This project is an example of how to configure HSE oscillator and reach 72 MHz
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

void Clock_Initialize(void); //Function to configure HSE with PLL clock and reach 72 MHz

void GPIO_Initialize(void); //Function to enable MCO pin

void Delay_ms(uint16_t time);

int main(void)
{
	Clock_Initialize();
	GPIO_Initialize();

	while(1)
	{
		GPIOA -> BSRR |= 1 << 5;
		Delay_ms(100);
		GPIOA -> BSRR |= 1 << 21;
		Delay_ms(100);

	}

	return 0;
}

void Clock_Initialize(void)
{
	//Reset CFGR register and switch off PLL
	RCC -> CFGR = 0x00000000;
	RCC -> CR &= ~(RCC_CR_PLLON);
	RCC -> CFGR = 0x00000000;

	//Turn on HSE oscillator
	RCC -> CR |= RCC_CR_HSEON; //turn on HSE
	while(!(RCC -> CR & RCC_CR_HSERDY)); //wait until HSE is ready

	//Configure wait latency states depend on SYSCLCK speed that we want
	FLASH -> ACR |= FLASH_ACR_LATENCY_2 | FLASH_ACR_PRFTBE; //set 2 wait states as latency Flash

	//Configure different prescaler and multiplier into RCC_CFGR register
	RCC -> CFGR |= 0x1D << 16; //PLLMUL x 9 and PLL source from HSE
	RCC -> CFGR |= 0x84 << 8; //ADC 1:6, APB2 without and APB1 1:2
	RCC -> CFGR |= 0x00 << 0; //AHB without

	RCC -> CR |= RCC_CR_PLLON; //turn on PLL
	while(!(RCC -> CR & RCC_CR_PLLRDY)); //wait til PLL is ready

	RCC -> CFGR |= RCC_CFGR_MCO_PLL; //we're gonna out PLL/2 frequency to test CPU clock frequency

	RCC -> CFGR |= RCC_CFGR_SW_PLL; //switch to PLL source
	while(!(RCC -> CFGR & RCC_CFGR_SWS_PLL)); //wait switch to PLL is ready

}

void GPIO_Initialize(void)
{
	//Enable PORTA clock
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN; //enable clock PORTA

	//GPIO pin 8 configure to get MCO function
	GPIOA -> CRH &= 0x00;
	GPIOA -> CRH |= 0x0B; //Alternate Function Push-pull with 50 MHz

	GPIOA -> CRL &= 0x00 << 20;
	GPIOA -> CRL |= 0x03 << 20; //Out with Push-pull and 50 MHz

}

void Delay_ms(uint16_t time)
{
	for(uint32_t i = 0; i < time * 4000; i++)
		asm("NOP");

}
