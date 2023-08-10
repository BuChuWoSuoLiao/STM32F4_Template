#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"

int main(void)
{
  u8 key;           //保存键值
	delay_init(168);  //初始化延时函数
	LED_Init();				//初始化LED端口 
	BEEP_Init();      //初始化蜂鸣器端口
	KEY_Init();       //初始化与按键连接的硬件接口
	
	while(1)
	{
		key=KEY_Scan(0);		//得到键值
	  if(key){						   
			switch(key){				 
				case KEY0_PRES:
					// 反转PF9电平
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)) {
						GPIO_ResetBits(GPIOF, GPIO_Pin_9);
					}else{
						GPIO_SetBits(GPIOF, GPIO_Pin_9);
					}
					break;
				case KEY1_PRES:
					// 反转PF8电平
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)) {
						GPIO_ResetBits(GPIOF, GPIO_Pin_8);
					}else{
						GPIO_SetBits(GPIOF, GPIO_Pin_8);
					}
					break;
				case KEY2_PRES:
					// 反转PF10电平
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)) {
						GPIO_ResetBits(GPIOF, GPIO_Pin_10);
					}else{
						GPIO_SetBits(GPIOF, GPIO_Pin_10);
					}
					break;
				case WKUP_PRES:
					// 将外设关闭
					GPIO_SetBits(GPIOF, GPIO_Pin_10);
					GPIO_SetBits(GPIOF, GPIO_Pin_9);
					GPIO_ResetBits(GPIOF, GPIO_Pin_8);
					break;
			}
		}else delay_ms(10); 
	}
}



