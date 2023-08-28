#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "uart.h"
#include "delay.h"

#define   TRIG_PIN					GPIO_Pin_8		// TRIG : PB8 
#define   TRIG_PORT					GPIOB
#define   INPUT_PIN					GPIO_Pin_6	// TRIG : PB6 
#define   INPUT_PORT				GPIOB

#define   TIM_USER					TIM4
#define   CLOCK_TIM_USER		RCC_APB1Periph_TIM4

uint8_t  flag_end = 0, state = 0;
uint16_t stt_old = 1, stt_new =1;
uint32_t count = 0;


void initPins(void);
void PWM_Init(void);
void DetectCount (void);

int main()
{
	PWM_Init();
	initPins();
	uart_Init();
	GPIO_SetBits(GPIOC , GPIO_Pin_13);
	TimerInit ();
	state = 0;
	flag_end = 1;
	while(1)
	{
		stt_old = stt_new;
		stt_new = GPIO_ReadInputDataBit(INPUT_PORT, INPUT_PIN);
	
		if(stt_old == 1 && stt_new == 0)
		{
			if(flag_end == 1)
			{
				if(state == 0)
				{
					state = 1;
					TIM_SetCompare3 (TIM_USER, 5000);
					GPIO_SetBits(GPIOC , GPIO_Pin_13);
				}
				else if(state == 1)
				{
					state = 0;
					GPIO_ResetBits(GPIOC , GPIO_Pin_13);
					TIM_SetCompare3(TIM_USER, 0);
					GPIO_ResetBits(GPIOC , GPIO_Pin_13);
				}
				flag_end = 0;
				count = 0;
			}
		}
	
		if(flag_end == 0)
		{
			
			if(count >= 299)
			{
				flag_end = 1;
			}
			else
			{
				count++;
			}
		}
		Delay_ms(10);
	}
}

void initPins(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	 
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(TRIG_PORT , &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = INPUT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(INPUT_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void PWM_Init(void) 
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(CLOCK_TIM_USER, ENABLE);
	//////////////////////////////////////////////////////
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720-1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 10000-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM_USER, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM_USER, ENABLE);
	
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0; 
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM_USER, &TIM_OCInitStruct);
	TIM_CtrlPWMOutputs(TIM_USER, ENABLE);
	TIM_SetCompare3(TIM_USER, 0);
	TIM_Cmd(TIM2, DISABLE);
}
