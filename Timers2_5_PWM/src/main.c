/*This project is an example of how to configure
 * PWM mode Edge aligned (EPWM) with a frequency of
 * 1 KHz and a Duty Cycle of 30%
*/
#include "stm32f10x.h"

//configure RCC clock using external oscillator (8 MHz) directly
void RCC_ClockConfig(void);

//configure pin PA0 (CH1) to use it as PWM output
void GPIO_Config(void);

//configure TIM2 as PWM in Edge aligned mode
void TIM2_PWMConfig(void);



int main(void)
{
	RCC_ClockConfig();
	GPIO_Config();
	TIM2_PWMConfig();


	while(1)
	{


	}

	return 0;

}


void RCC_ClockConfig(void)
{
	//Reset CFGR register and disable PLL
	RCC -> CFGR = (uint32_t)(0x0000);
	RCC -> CR &= ~ RCC_CR_PLLON;
	RCC -> CFGR = (uint32_t)(0x0000);

	//Configure Flash states to one
	FLASH -> ACR &= ~(uint32_t)(0x07); //zero flash states and prefetch enable

	//Enable HSE clock
	RCC -> CR |= RCC_CR_HSEON;
	while(!(RCC -> CR & RCC_CR_HSERDY));

	RCC -> CFGR |= (uint32_t)(0x1); //switch to External oscillator
	while(!(RCC -> CFGR & RCC_CFGR_SWS_HSE));

}


void GPIO_Config(void)
{
	RCC -> APB2ENR |= 0x04; //enable GPIOA clock
	RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC -> APB2ENR |= RCC_APB2ENR_AFIOEN; //enable AFIO periph

	GPIOA -> CRL = 0x444444AA; //pin PA0, PA1 as Alternate function Push-pull 2 MHz
	GPIOB -> CRH = 0x4444AA44; //pin PB10 as Alternate function Push-pull 2 MHz

	//remap TIM2 CH3 to pin PB10
	AFIO -> MAPR |= (uint32_t)(0x2 << 8);

}


void TIM2_PWMConfig(void)
{
	RCC -> APB1ENR |= (uint32_t)(0x01); //enable TIM2 clock

	TIM2 -> CR1 |= (uint16_t)(0x80); //enable ARPE bit and Edge Aligned Mode
	TIM2 -> CCMR1 |= (uint16_t)(0x6868); //configure PWM mode 1 and Precharge bit for CH1, CH2 and CH3
	TIM2 -> CCMR2 |= (uint16_t)(0x6868);
	TIM2 -> CCER |= (uint16_t)(0x01) | (uint16_t)(1 << 4) | (uint16_t)(1 << 8) | (uint16_t)(1 << 12); //Polarity 0, CH1, CH2 and CH3
	TIM2 -> ARR = 7999; //load value to get 1 KHz of period
	TIM2 -> CCR1 = 2399; //load value to get 30% duty cycle
	TIM2 -> CCR2  = 3999; //load to get 50% duty cycle
	TIM2 -> CCR3 = 5999; //load to get 75%
	TIM2 -> CCR4 = 7991; //load to get 99.9%
	TIM2 -> EGR |= (uint16_t)(0x01); //UG bit set
	TIM2 -> CR1 |= (uint16_t)(0x01); // enable TIM2

}
