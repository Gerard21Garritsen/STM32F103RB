//This program is an example of how to configure PWM
#include "stm32f10x.h"

//configure oscillator to use HSE clock (8 MHz) to feed CPU and the rest of peripherals
void RCC_Initialize(void);

//configure Pin PA0 as digital output Push-pull Alternate Function
void GPIO_Initialize(void);

//configure PWM to generate a signal with 100 Hz and duty cycle of 30%
void PWM_Initialize(void);

int main(void)
{
	RCC_Initialize();
	GPIO_Initialize();
	PWM_Initialize();

	while(1)
	{


	}

	return 0;

}

void RCC_Initialize(void)
{
	RCC_DeInit(); //reset CR and CFGR registers

	//enable external crystal oscillator
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_WaitForHSEStartUp() != SUCCESS);

	//switch to HSE clock as main clock for CPU and the rest of peripherals
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
	while(RCC_GetSYSCLKSource() != 0x04);

	//switch off HSI clock
	RCC_HSICmd(DISABLE);

}

void GPIO_Initialize(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIOA_StructureInit;

	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_0;
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOA_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

}

void PWM_Initialize(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //enable Timer 2 clock

	TIM_DeInit(TIM2); //reset Timer 2 registers

	//create TIM2 structure to get 100 Hz of frequency
	TIM_TimeBaseInitTypeDef TIM2_StructureInit;

	TIM2_StructureInit.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM2_StructureInit.TIM_CounterMode = TIM_CounterMode_Up;
	TIM2_StructureInit.TIM_Prescaler = (uint16_t)(0x01); //precaler 1:2
	TIM2_StructureInit.TIM_Period = 39999; //load TIMx_ARR with 39,999

	TIM_TimeBaseInit(TIM2, &TIM2_StructureInit);

	/*create PWM structure using TIM2 as time base to get
	 * 100 Hz and 30% of Duty Cycle
	 */
	TIM_OCInitTypeDef PWM1_StructureInit;

	PWM1_StructureInit.TIM_Pulse = 11999;
	PWM1_StructureInit.TIM_OCMode = TIM_OCMode_PWM1;
	PWM1_StructureInit.TIM_OCPolarity = TIM_OCPolarity_High;
	PWM1_StructureInit.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OC1Init(TIM2, &PWM1_StructureInit); //initialize structure with CH1 (PA0)

	//initialize PWM module
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	//run TIM2
	TIM_Cmd(TIM2, ENABLE);

}
