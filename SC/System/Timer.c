#include "stm32f10x.h"                  // Device header

/********** 定时公式 **********/
/*
	TimeOut = ((Prescaler + 1) * (Period + 1) ) / TimeClockFren;
	
	解释下公式参数意义：
	TimeOut：定时器溢出时间（单位为us），多少触发（进入）一次TIM中断。
	Prescaler：分频TIM时钟的预分频器值。
	Period：计数重载值，TIM计数当超过这个值，则重新计数。
	TimeClockFren：定时器的输入时钟频率（单位MHZ），也就是当前使用的TIM所用的CLOK的时钟频率,也就是系统时钟频率72 MHz。
  
	TimeOut = (7199+1)*(9999+1)/72 MHz
					= 72000000 / 72000000 Hz
					= 1 s
*/

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能TIM2时钟
	
	TIM_InternalClockConfig(TIM2);	//选择使用内部定时器TIM2
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//1分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数器向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;									//周期		 ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;								//预分频器 PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;							//重复计数器
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);															//清除标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);												//使能中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);										//初始化中断
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}


