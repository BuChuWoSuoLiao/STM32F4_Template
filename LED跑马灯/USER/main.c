#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "sys.h"

int main(void)
{
	delay_init(168);
	// 初始化led
	LED_Init();

	while(1) {
		GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0对应引脚GPIOF.9拉低，亮  等同LED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_10);   //LED1对应引脚GPIOF.10拉高，灭 等同LED1=1;
		delay_ms(500);  		   //延时300ms
		GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0对应引脚GPIOF.0拉高，灭  等同LED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_10); //LED1对应引脚GPIOF.10拉低，亮 等同LED1=0;
		delay_ms(500); 
	}
	
}

