#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "Serial.h"

enum STATE{
	WAIT=0, CONED=1
} state = WAIT;

uint8_t timeUp;
uint8_t revArry[] = {0xFE, 0x5A, 0x49, 0x61, 0x6E, 0xEF};
uint8_t heartBeat[] = {0xFE, 0x5A, 0x61, 0x6E, 0x65, 0x72, 0x6F, 0x67, 0x6C, 0xEF};

uint8_t GetCMD(void);

int main(void)
{
	Timer_Init();
	Serial_Init();
	
	while (1)
	{	
		if(Serial_GetRxFlag()==1) {
			switch (GetCMD()){
				case 0:
					break;
				case 1:			//Start
					Serial_SendArray(revArry, 6); 
				    state = CONED;
				    timeUp = 0;
					break;
				case 2:			//Stop
					state = WAIT;
					timeUp = 1;
					break;
			}
		}
		
		if(state == CONED){
			if(Serial_GetRxFlag()==1 && GetCMD()==1){			//如果中途断开，检测指令
				timeUp = 1;
				state = WAIT;
				continue;
			} else if(timeUp==1) {
				Serial_SendArray(heartBeat, 10);
				timeUp = 0;
			}
		}
	}
}

void TIM2_IRQHandler(void){
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(timeUp == 0){
			timeUp = 1;
		}
	}
}

uint8_t GetCMD(void){
	if(Serial_RxPacket[0]==0x5A && Serial_RxPacket[1]==0x49 && Serial_RxPacket[2]==0x61 && Serial_RxPacket[3]==0x6E){			//Start
		return 1;
	} else if(Serial_RxPacket[0]==0x6E && Serial_RxPacket[1]==0x61 && Serial_RxPacket[2]==0x49 && Serial_RxPacket[3]==0x5A){	//Stop
		return 2;
	}
//	 else if(Serial_RxPacket[0]==0x44 && Serial_RxPacket[1]==0x69 && Serial_RxPacket[2]==0x73 && Serial_RxPacket[3]==0x63){	//Disconnect
//		return 3;
//	}
	return 0;																													//Wrong
}


