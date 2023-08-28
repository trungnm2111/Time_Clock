
/*
  ******************************************************************************
  * @file		pwm.h                                                              *
  * @author	NGUYEN MINH TRUNG                                                        *
  * @date		00/00/0000                                                         *
  ******************************************************************************
*/

#ifndef _PWM_H_
#define _PWM_H_

/******************************************************************************/

#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

/******************************************************************************/

#define   TRIG_PIN					GPIO_Pin_8		// TRIG : PB8 
#define   TRIG_PORT					GPIOB
#define   INPUT_PIN					GPIO_Pin_6	// TRIG : PB6 
#define   INPUT_PORT				GPIOB
#define   TIM_USER					TIM4
#define   CLOCK_TIM_USER		RCC_APB1Periph_TIM4



/******************************************************************************/

void initPins(void) ;
void PWM_Init(void);
void DetectCount (void);
void Tim2Config(void);
/******************************************************************************/
#endif


