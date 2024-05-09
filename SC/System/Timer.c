#include "stm32f10x.h"                  // Device header

/********** ��ʱ��ʽ **********/
/*
	TimeOut = ((Prescaler + 1) * (Period + 1) ) / TimeClockFren;
	
	�����¹�ʽ�������壺
	TimeOut����ʱ�����ʱ�䣨��λΪus�������ٴ��������룩һ��TIM�жϡ�
	Prescaler����ƵTIMʱ�ӵ�Ԥ��Ƶ��ֵ��
	Period����������ֵ��TIM�������������ֵ�������¼�����
	TimeClockFren����ʱ��������ʱ��Ƶ�ʣ���λMHZ����Ҳ���ǵ�ǰʹ�õ�TIM���õ�CLOK��ʱ��Ƶ��,Ҳ����ϵͳʱ��Ƶ��72 MHz��
  
	TimeOut = (7199+1)*(9999+1)/72 MHz
					= 72000000 / 72000000 Hz
					= 1 s
*/

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
	
	TIM_InternalClockConfig(TIM2);	//ѡ��ʹ���ڲ���ʱ��TIM2
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//1��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���������ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;									//����		 ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;								//Ԥ��Ƶ�� PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;							//�ظ�������
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);															//�����־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);												//ʹ���ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);										//��ʼ���ж�
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}


