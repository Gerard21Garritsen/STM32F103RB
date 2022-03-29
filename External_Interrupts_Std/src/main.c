//This project is an example of how to configure External Interrupts
#include "stm32f10x.h"
#include "stm32f1xx_nucleo.h"

//Functions created
void GPIO_Initialize(void); //initialize pin PA5 and PC13

void RCC_Initialize(void); //intialize clock to use HSE clock directly (8 MHz)

void Interrupt_Initialize(void); //initialize and configure external interrupt on pin PC13

void Interrupt2_Initialize(void); //initialize and configure external interrupt on pin PC4

void Delay_ms(uint16_t time); //function to make delays by software

void EXTI15_10_IRQHandler(void); //Interrupt routine function for EXTI15 - EXTI10

void EXTI4_IRQHandler(void); //Interrupt routine function for EXTI4

int main(void)
{
	RCC_Initialize(); //called to clock function initialize
	GPIO_Initialize(); //called to GPIO pins initialize
	Interrupt_Initialize(); //called to interrupt on pin PC13 initialize
	Interrupt2_Initialize(); //called to interrupt on pin PC4 initialize

	while(1)
	{


	}

	return 0;
}

void GPIO_Initialize(void)
{
	//enable APB2 clock for PORTA and PORTC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

	//Initialize GPIO structure and configure pins PA5 and PC13
	GPIO_InitTypeDef GPIO_StructureInit;

	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_5;
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_StructureInit.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA, &GPIO_StructureInit);

	GPIO_StructureInit.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_4; //pin PC13 and PC4 as inputs
	GPIO_StructureInit.GPIO_Mode = GPIO_Mode_IPU; //input with PullUp resistor

	GPIO_Init(GPIOC, &GPIO_StructureInit);

}

void RCC_Initialize(void)
{
	//Configuration to use External crystal oscillator as main clock (8 MHz)
	//Turn on PLL and reset CGFR registers
	RCC_PLLCmd(DISABLE);
	RCC_DeInit();

	//Enable HSE clock
	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_WaitForHSEStartUp() != SUCCESS); //wait upon HSE clock is stable

	//Switch to HSE as main clock
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

}

void Interrupt_Initialize(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //enable EXTI clock
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13); //define which GPIO PORT and pin

	//define which group of priorities we want
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //it means we have 2 bits for priority and 2 bits for subpriority

	//create our EXTI structure
	EXTI_InitTypeDef EXTI_StructureInit;

	EXTI_StructureInit.EXTI_Mode = EXTI_Mode_Interrupt; //define we are working with Interrupts
	EXTI_StructureInit.EXTI_Trigger = EXTI_Trigger_Falling; //define trigger edge as falling edge
	EXTI_StructureInit.EXTI_Line = EXTI_Line13; //define line 13 because Pin 13
	EXTI_StructureInit.EXTI_LineCmd = ENABLE; //enable EXTI configuration

	EXTI_Init(&EXTI_StructureInit);

	//create our NVIC structure
	NVIC_InitTypeDef NVIC_StructureInit;

	NVIC_StructureInit.NVIC_IRQChannel = EXTI15_10_IRQn; //define that because our EXTI is EXTI13 (PC13)
	NVIC_StructureInit.NVIC_IRQChannelPreemptionPriority = 0; //set priority with the highest level
	NVIC_StructureInit.NVIC_IRQChannelSubPriority = 0; //set subpriority with the highest level
	NVIC_StructureInit.NVIC_IRQChannelCmd = ENABLE; //enable interrupt

	NVIC_Init(&NVIC_StructureInit);

}

void Interrupt2_Initialize(void)
{
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4); //define which PORT and Pin is an EXTI Line source

	EXTI_InitTypeDef EXTI_StructureInit;

	EXTI_StructureInit.EXTI_Line = EXTI_Line4;
	EXTI_StructureInit.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_StructureInit.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_StructureInit.EXTI_LineCmd = ENABLE;

	EXTI_Init(&EXTI_StructureInit);

	NVIC_InitTypeDef NVIC_StructureInit;

	NVIC_StructureInit.NVIC_IRQChannel = EXTI4_IRQn; //define EXTI4 because we're using Pin 4
	NVIC_StructureInit.NVIC_IRQChannelPreemptionPriority = 1; //set priority with "1"
	NVIC_StructureInit.NVIC_IRQChannelSubPriority = 0; //set subpriority with the highest level
	NVIC_StructureInit.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_StructureInit);

}

void Delay_ms(uint16_t time)
{
	for(uint32_t i = 0; i < time * 4000; i++)
		asm("NOP");

}

void EXTI15_10_IRQHandler(void)
{
	//check if interrupt comes from line expected
	if(EXTI_GetFlagStatus(EXTI_Line13) == SET)
	{
		for(uint8_t i = 0; i < 4; i++) //blink led onboard 4 times for 100 ms
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
			Delay_ms(100);
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
			Delay_ms(100);

		}
	}

	EXTI_ClearFlag(EXTI_Line13); //clear flag

}

void EXTI4_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line4) == SET)
	{
		for(uint8_t i = 0; i < 6; i++) //blink led onboard 6 times for 50 ms
		{
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_SET);
			Delay_ms(50);
			GPIO_WriteBit(GPIOA, GPIO_Pin_5, Bit_RESET);
			Delay_ms(50);
		}
	}

	EXTI_ClearFlag(EXTI_Line4); //clear flag

}
