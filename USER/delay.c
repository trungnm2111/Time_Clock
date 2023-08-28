#include "delay.h"

unsigned int Timingdelay;

void TimerInit (void);
void SysTick_Handler (void);
void Delay_ms (uint16_t time);


void TimerInit (void)
{
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
}
void SysTick_Handler (void)
{
	Timingdelay --;
}
void Delay_ms (uint16_t time)
{
	Timingdelay = time;
	while(Timingdelay != 0);
}


































