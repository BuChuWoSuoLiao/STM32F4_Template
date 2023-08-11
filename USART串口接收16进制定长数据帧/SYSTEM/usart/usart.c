#include "sys.h"
#include "usart.h"	 
#include "string.h"
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
 

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //如果使能了接收

//接收缓冲,最大USART_REC_LEN个字节.
char USART_RX_BUF[USART_REC_LEN];     
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	
uint8_t USART_RX_Index = 0;	// 接收字节索引

/************************************
1. 初始化IO 串口
2. bound:	特率
************************************/
void uart_init(u32 bound){
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif
}

/************************************
1. 串口发送单个字符
2. USARTx:	串口号
3. Byte	 :	发送的数据
************************************/
void USART_SendByte(USART_TypeDef* USARTx, uint8_t Byte)
{
	USART_SendData(USARTx,Byte);
	// 将数据data&(uint16_t0x01FF)，然后直接赋值给DR寄存器，
	// 数据通向TDR发送寄存器，TDR再传递给发送移位寄存器，最后一位一位的把数据移出到TX引脚完成发送
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);												
}


/************************************
1. 串口发送数组
2. USARTx	:	串口号
3. *Array	:	发送的数据
4. Length	: 数据长度
************************************/
void USART_SendArray(USART_TypeDef* USARTx, uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0 ;i<Length;i++)
	{
		USART_SendByte(USARTx, Array[i]);
	}
}


/************************************
1. 串口发送字符串
2. USARTx	:	串口号
3. *String	:	发送的数据
************************************/
void USART_SendString(USART_TypeDef* USARTx, char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		USART_SendByte(USARTx, String[i]);
	}

}

/*次方函数*/
uint32_t USART_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result *=X;
	}
	return Result;
}


/************************************
1. 串口发送数组
2. USARTx	:	串口号
3. *String	:	发送的数据
4. Lenght : 数据长度
************************************/
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t Number,uint8_t Lenght)
{
	uint8_t i;
	for(i=0;i<Lenght;i++)
	{
		USART_SendByte(USARTx, Number/USART_Pow(10,Lenght-i-1)%10+'0');
	}
}


/************************************
1. 串口1中断服务程序
************************************/
void USART1_IRQHandler(void)                	
{
//	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	// 接收到的一字节数据
	uint8_t recv_dat;
	// 静态变量，表示当前接受状态
	static uint8_t recv_state = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		// 将接收到的数据放入 recv_dat 中
		recv_dat = USART_ReceiveData(USART1);
		
		switch(recv_state) {
			case 0:	// 接收到帧头
				if(recv_dat == 0xFE){
					recv_state = 1;
					USART_RX_Index = 0;	// 重置数据缓冲区索引
				}else{
					recv_state = 0;	
				}
				break;
			case 1:	// 接受数据
				USART_RX_BUF[USART_RX_Index] = recv_dat;
				USART_RX_Index++;
				if(USART_RX_Index >= 4) {
					recv_state = 2;
				}
				break;
			case 2:	// 接收到帧尾
				if(recv_dat == 0xEF) {
					USART_RX_STA = 1;	// 设置标志为1，表示数据接受完成
					recv_state = 0;
				}
				break;
		}
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

 



