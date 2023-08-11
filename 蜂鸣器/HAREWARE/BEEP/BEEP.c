#include "beep.h"
#include "stm32f4xx.h"

/////////////////////////////////////
// ��ʼ��������
// 1.��ʼ��ʱ��
// 2.��ʼ��io
// 	��1������io��
// 	��2��io����ģʽ
//	��3��io�������ģʽ
// 	��4����������
// 	��5���ٶ�
/////////////////////////////////////
void BEEP_Init(void){
	// ��ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitType;
	
	// ��ʼ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	// ��������
	GPIO_InitType.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitType.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitType.GPIO_OType = GPIO_OType_PP;
	GPIO_InitType.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitType.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitType);
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}

