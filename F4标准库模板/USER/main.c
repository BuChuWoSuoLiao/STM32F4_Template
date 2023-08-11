#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

int main(void)
{ 
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);		//��ʱ��ʼ�� 
	uart_init(115200);	//���ڳ�ʼ��������Ϊ115200
	while(1)
	{
		
		if(USART_RX_STA == 1) {
			// ����״̬
			printf(USART_RX_BUF);
			// ���������
			memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
			USART_RX_STA = 0;
		}
	}
}


