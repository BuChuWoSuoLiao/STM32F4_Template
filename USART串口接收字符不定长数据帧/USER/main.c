#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

int main(void)
{ 
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	while(1)
	{
		
		if(USART_RX_STA == 1) {
			// 重置状态
			printf(USART_RX_BUF);
			// 清除缓冲区
			memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
			USART_RX_STA = 0;
		}
	}
}


