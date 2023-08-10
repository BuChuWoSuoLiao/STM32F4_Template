#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"

int main(void)
{
	
	delay_init(168);
	BEEP_Init();
	
  while(1){
		GPIO_ResetBits(GPIOF,GPIO_Pin_8); //BEEP引脚拉低， 等同BEEP=0;
		delay_ms(300);                    //延时300ms
		GPIO_SetBits(GPIOF,GPIO_Pin_8);   //BEEP引脚拉高， 等同BEEP=1;
		delay_ms(300); 	
	}
}