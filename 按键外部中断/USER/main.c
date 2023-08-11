#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "exit.h"

int main(void)
{ 
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	EXTIX_Init();				// 初始化中断
	printf("Hello Stm32!!!");
	while(1)
	{
		
	}
}


