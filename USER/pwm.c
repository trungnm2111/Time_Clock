#include "pwm.h" 

uint16_t  check_time = 0;
uint8_t stt = 0 , stt_old = 0, stt_new =0;
void Tim2Config(void);
void TimerInit (void);
void initPins(void);
void PWM_Init(void);
void initPins(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	 // PB8 (trigle) channel 3
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(TRIG_PORT , &GPIO_InitStructure);
	// PB6 (input)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = INPUT_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(INPUT_PORT , &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC , &GPIO_InitStructure);
}

void PWM_Init(void) 
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(CLOCK_TIM_USER, ENABLE);
	//////////////////////////////////////////////////////
//	TIM_DeInit(TIM_USER);
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
//	TIM_ITConfig (TIM4 , TIM_FLAG_Update, ENABLE);
//	TIM_ClearFlag(TIM_USER, TIM_FLAG_Update);
	TIM_CtrlPWMOutputs(TIM_USER, ENABLE);
//	NVIC_EnableIRQ (TIM4_IRQn);

}

void delay(unsigned int time){
	unsigned int i, j;
	for(i=0 ;i< time ;i++){
		for(j=0 ; j<0x2aff ;j++);
	}
}

void DetectCount (void)
{
	stt_old = stt_new;
	stt_new = GPIO_ReadInputDataBit(INPUT_PORT, INPUT_PIN);
	if(stt_old == 0 && stt_new == 1)
	{
		stt ++; 
	}
	if( stt == 1)
	{
		GPIO_ResetBits(GPIOC , GPIO_Pin_13);
		TIM_SetCompare3 (TIM_USER, 5000);
		TIM_Cmd (TIM2, ENABLE);
	}
	if( stt == 0 )
	{
		GPIO_SetBits(GPIOC , GPIO_Pin_13);
		TIM_SetCompare3 (TIM_USER, 0);
		check_time = 0;
		stt = 0;
	}

	delay(1);
}	

void Tim2Config(void)
{
	TIM_TimeBaseInitTypeDef tim_innit_structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	tim_innit_structure.TIM_CounterMode = TIM_CounterMode_Up;
	tim_innit_structure.TIM_Period = 5000 -1;
	tim_innit_structure.TIM_Prescaler = 7200-1;
	tim_innit_structure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &tim_innit_structure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig (TIM2 , TIM_FLAG_Update, ENABLE);
	NVIC_EnableIRQ (TIM2_IRQn);
	
}
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET)
	{
//		if(stt == 1)
//		{
			check_time = 1;
//		}
//		else
//		{
//			check_time = 0;	
//		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	}
}
