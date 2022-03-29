//This project is an example of how to use Timers 2 to 5 as Free Running with Interrupts
#include "stm32f10x.h"

//Interrupt TIM2 Routine
void TIM2_IRQHandler(void);

//Configure clock to use external crystal (8 MHz) without PLL as main CPU and peripherals clock
void RCC_Initialize(void);

//Configure Pin PA5 as digital out Push-pull at 2 MHz
void GPIO_Initialize(void);

//Configure Timer 2 as Free Running Mode to overflows every 500 ms
void TIM2_Initialize(void);



int main(void)
{
	RCC_Initialize();
	GPIO_Initialize();
	TIM2_Initialize();


	while(1)
	{


	}

	return 0;

}

void TIM2_IRQHandler(void)
{
	if(TIM2 -> SR & TIM_SR_UIF)
	{
		//Blink led routine
		GPIOA -> ODR ^= (uint32_t)(1 << 5);
	}

	TIM2 -> SR &= ~ TIM_SR_UIF; // clear flag

}

void RCC_Initialize(void)
{
	//configure in reset state mode CFGR register and switch off PLL
	RCC -> CFGR &= (uint32_t)(0x0000); //reset CFGR registers
	RCC -> CR &= ~RCC_CR_PLLON; //switch off PLL
	RCC -> CFGR = (uint32_t)(0x0000);

	//Enable external oscillator and use it as main clock
	RCC -> CR |= RCC_CR_HSEON;
	while(!(RCC -> CR & RCC_CR_HSERDY));

	//select HSE clock as main CPU and peripherals clock (8 MHz external OSC)
	RCC -> CFGR |= RCC_CFGR_SW_HSE;
	while(!(RCC -> CFGR & RCC_CFGR_SWS_HSE));

	//Disable HSI clock
	RCC -> CR &= ~RCC_CR_HSION;

}

void GPIO_Initialize(void)
{
	RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN; //enable GPIOA PORT clock

	//configure Pin PA5 as output Push-pull 2 MHz
	GPIOA -> CRL = (uint32_t)(0x44044444);
	GPIOA -> CRL |= (uint32_t)(0x2 << 20);

}

void TIM2_Initialize(void)
{
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN; //enable Timer 2 clock

	//configure Timer 2 as Free Running Mode with
	TIM2 -> CR1 = 0x0000; //Clock division 1:1, TIMx_ARR buffered and upcounting
	TIM2 -> EGR |= (uint16_t)(0x1); //enable re-initialize counter after an update event

	TIM2 -> SR = 0x0000; //reset Status register
	TIM2 -> CNT = 0x0000; //reset counter
	TIM2 -> PSC = 0x007F; //set Prescaler 1:128
	TIM2 -> ARR = 31249; //load with 31249 to temporize 500 ms

	//Interrupt configuration
	SCB -> AIRCR = (uint32_t)(0x05FA << 16) | (uint32_t)(0x300);
	TIM2 -> DIER |= TIM_DIER_UIE; //enable Interrupt on TIM2
	NVIC -> IP[28] |= (uint32_t)(0x00); //configure highest priority
	NVIC -> ISER[0] |= (uint32_t) (1 << 28); //enable interrupt by TIM2 overflow

	TIM2 -> CR1 |= (uint16_t)(0x0001); //turn on Timer 2

}
