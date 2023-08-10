#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"

int main(void)
{
	
	delay_init(168);
	BEEP_Init();
	
  while(1){
		GPIO_ResetBits(GPIOF,GPIO_Pin_8); //BEEP�������ͣ� ��ͬBEEP=0;
		delay_ms(300);                    //��ʱ300ms
		GPIO_SetBits(GPIOF,GPIO_Pin_8);   //BEEP�������ߣ� ��ͬBEEP=1;
		delay_ms(300); 	
	}
}