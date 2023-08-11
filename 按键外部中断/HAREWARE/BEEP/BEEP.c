#include "beep.h"
#include "stm32f4xx.h"

void BEEP_Init(void){
	// 初始化结构体
	GPIO_InitTypeDef GPIO_InitType;
	
	// 初始化时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	// 设置引脚
	GPIO_InitType.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitType.GPIO_OType = GPIO_OType_PP;
	GPIO_InitType.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitType);
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}

