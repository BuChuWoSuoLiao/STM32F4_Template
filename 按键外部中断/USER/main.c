#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "exit.h"

int main(void)
{ 
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	EXTIX_Init();				// ��ʼ���ж�
	printf("Hello Stm32!!!");
	while(1)
	{
		
	}
}


