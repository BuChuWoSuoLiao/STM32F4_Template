#include "sys.h"
#include "usart.h"	 
#include "string.h"
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
 

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   //���ʹ���˽���

//���ջ���,���USART_REC_LEN���ֽ�.
char USART_RX_BUF[USART_REC_LEN];     
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
uint8_t USART_RX_Index = 0;	// �����ֽ�����

/************************************
1. ��ʼ��IO ����
2. bound:	����
************************************/
void uart_init(u32 bound){
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	
#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
#endif
}

/************************************
1. ���ڷ��͵����ַ�
2. USARTx:	���ں�
3. Byte	 :	���͵�����
************************************/
void USART_SendByte(USART_TypeDef* USARTx, uint8_t Byte)
{
	USART_SendData(USARTx,Byte);
	// ������data&(uint16_t0x01FF)��Ȼ��ֱ�Ӹ�ֵ��DR�Ĵ�����
	// ����ͨ��TDR���ͼĴ�����TDR�ٴ��ݸ�������λ�Ĵ��������һλһλ�İ������Ƴ���TX������ɷ���
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);												
}


/************************************
1. ���ڷ�������
2. USARTx	:	���ں�
3. *Array	:	���͵�����
4. Length	: ���ݳ���
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
1. ���ڷ����ַ���
2. USARTx	:	���ں�
3. *String	:	���͵�����
************************************/
void USART_SendString(USART_TypeDef* USARTx, char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		USART_SendByte(USARTx, String[i]);
	}

}

/*�η�����*/
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
1. ���ڷ�������
2. USARTx	:	���ں�
3. *String	:	���͵�����
4. Lenght : ���ݳ���
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
1. ����1�жϷ������
************************************/
void USART1_IRQHandler(void)                	
{
//	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	// ���յ���һ�ֽ�����
	uint8_t recv_dat;
	// ��̬��������ʾ��ǰ����״̬
	static uint8_t recv_state = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		// �����յ������ݷ��� recv_dat ��
		recv_dat = USART_ReceiveData(USART1);
		
		switch(recv_state) {
			case 0:	// ���յ�֡ͷ
				if(recv_dat == 0xFE){
					recv_state = 1;
					USART_RX_Index = 0;	// �������ݻ���������
				}else{
					recv_state = 0;	
				}
				break;
			case 1:	// ��������
				USART_RX_BUF[USART_RX_Index] = recv_dat;
				USART_RX_Index++;
				if(USART_RX_Index >= 4) {
					recv_state = 2;
				}
				break;
			case 2:	// ���յ�֡β
				if(recv_dat == 0xEF) {
					USART_RX_STA = 1;	// ���ñ�־Ϊ1����ʾ���ݽ������
					recv_state = 0;
				}
				break;
		}
  } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif	

 



