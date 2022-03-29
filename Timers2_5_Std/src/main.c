//This project is an example of how to configure Timer 2 as Running Free Mode
#include "stm32f10x.h"

void RCC_Initialize(void);

void GPIO_Initialize(void);

void TIM2_Initialize(void);

int main(void)
{
	RCC_Initialize();
	GPIO_Initialize();
	TIM2_Initialize();

	while(1)
	{
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
		while(!(TIM2 -> SR & TIM_SR_UIF));
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
		while(!(TIM2 -> SR & TIM_SR_UIF));
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	}

	return 0;

}

void RCC_Initialize(void)
{
	RCC_DeInit(); //reset CFGR and CR registers

	RCC_HSEConfig(RCC_HSE_ON); //turn on HSE external clock
	while(RCC_WaitForHSEStartUp() != SUCCESS);

	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); //select HSE clock as main CPU clock
	while(RCC_GetSYSCLKSource() != 0x04); //wait until HSE stabilize and it'll be selected as main clock

	RCC_HSICmd(DISABLE); //switch off HSI clock

}

void GPIO_Initialize(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //enable GPIOA clock

	GPIO_InitTypeDef GPIOA_StructureInit;

	GPIOA_StructureInit.GPIO_Pin = GPIO_Pin_5;
	GPIOA_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOA_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIOA_StructureInit);

}

void TIM2_Initialize(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //enable TIM2 clock

	TIM_DeInit(TIM2);

	TIM_TimeBaseInitTypeDef TIM2_StructureInit;

	TIM2_StructureInit.TIM_CounterMode = TIM_CounterMode_Up;
	TIM2_StructureInit.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM2_StructureInit.TIM_Prescaler = 16; //set prescaler value 16
	TIM2_StructureInit.TIM_Period = 49999; //load into TIMx_ARR

	TIM_TimeBaseInit(TIM2, &TIM2_StructureInit);

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //clear UIF flag

	TIM_Cmd(TIM2, ENABLE); //turn on Timer 2

}
