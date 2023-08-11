#include "led.h"
#include "stm32f4xx.h"

// 初始化led
void LED_Init(void){
		// 定义gpio初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	// GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	// 定义io口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						// 普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;				// 100mhz速度
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;							// 上拉
	// 初始化gpio
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	// 初始化gpio电平
	GPIO_ResetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
}


