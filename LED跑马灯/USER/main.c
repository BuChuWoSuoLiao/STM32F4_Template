#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "sys.h"

int main(void)
{
	delay_init(168);
	// ��ʼ��led
	LED_Init();

	while(1) {
		GPIO_ResetBits(GPIOF,GPIO_Pin_9);  //LED0��Ӧ����GPIOF.9���ͣ���  ��ͬLED0=0;
		GPIO_SetBits(GPIOF,GPIO_Pin_10);   //LED1��Ӧ����GPIOF.10���ߣ��� ��ͬLED1=1;
		delay_ms(500);  		   //��ʱ300ms
		GPIO_SetBits(GPIOF,GPIO_Pin_9);	   //LED0��Ӧ����GPIOF.0���ߣ���  ��ͬLED0=1;
		GPIO_ResetBits(GPIOF,GPIO_Pin_10); //LED1��Ӧ����GPIOF.10���ͣ��� ��ͬLED1=0;
		delay_ms(500); 
	}
	
}

