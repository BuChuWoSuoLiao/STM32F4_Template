#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"

int main(void)
{
  u8 key;           //�����ֵ
	delay_init(168);  //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿� 
	BEEP_Init();      //��ʼ���������˿�
	KEY_Init();       //��ʼ���밴�����ӵ�Ӳ���ӿ�
	
	while(1)
	{
		key=KEY_Scan(0);		//�õ���ֵ
	  if(key){						   
			switch(key){				 
				case KEY0_PRES:
					// ��תPF9��ƽ
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)) {
						GPIO_ResetBits(GPIOF, GPIO_Pin_9);
					}else{
						GPIO_SetBits(GPIOF, GPIO_Pin_9);
					}
					break;
				case KEY1_PRES:
					// ��תPF8��ƽ
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_8)) {
						GPIO_ResetBits(GPIOF, GPIO_Pin_8);
					}else{
						GPIO_SetBits(GPIOF, GPIO_Pin_8);
					}
					break;
				case KEY2_PRES:
					// ��תPF10��ƽ
					if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)) {
						GPIO_ResetBits(GPIOF, GPIO_Pin_10);
					}else{
						GPIO_SetBits(GPIOF, GPIO_Pin_10);
					}
					break;
				case WKUP_PRES:
					// ������ر�
					GPIO_SetBits(GPIOF, GPIO_Pin_10);
					GPIO_SetBits(GPIOF, GPIO_Pin_9);
					GPIO_ResetBits(GPIOF, GPIO_Pin_8);
					break;
			}
		}else delay_ms(10); 
	}
}



