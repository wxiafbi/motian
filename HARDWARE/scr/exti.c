#include "stm32f10x.h" //������Ҫ��ͷ�ļ�
#include "exti.h"
#include "key.h"
#include "delay.h"
#include "stmflash.h" //������flash������
#include "oled.h"
//#include "TF-LUNA.h"
//#include "main.h"
u8 flag = 1;
uint16_t test = Initial_check_value;
float temp_tof = 0;
unsigned int amp_valu = 0;
float High_liquid_level[5] = {0};
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	KEY_Init(); //	�����˿ڳ�ʼ��

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ�ܸ��ù���ʱ��

	//GPIO A5 �ж����Լ��жϳ�ʼ������   �½��ش���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line = EXTI_Line5; //KEY1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIO A6	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIO A7	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//����ͨ������5����9
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			 //ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void OLED_SHOW(void)
{
	float b;
	OLED_Clear();
	STMFLASH_Read(0x8010000, &test, 1);
	OLED_ShowCHinese(0, 0, 0);
	OLED_ShowCHinese(36, 0, 1); //��ʾ����
	OLED_ShowCHinese(0, 2, 2);
	OLED_ShowCHinese(36, 2, 3); //��ʾ����
	OLED_ShowCHinese(0, 4, 4);
	OLED_ShowCHinese(18, 4, 5);
	OLED_ShowCHinese(36, 4, 6); //��ʾУ��ֵ
	OLED_ShowCHinese(0, 6, 7);
	OLED_ShowCHinese(18, 6, 8);
	OLED_ShowCHinese(36, 6, 9); //��ʾҺλ��
	OLED_ShowString(54, 0, ":");
	OLED_ShowString(54, 2, ":");
	OLED_ShowString(54, 4, ":");
	OLED_ShowString(54, 6, ":");
	OLED_DecimalShow(72, 0, adcdata, 3, 2, 16);								  //��������ֵ����ʾ
	OLED_DecimalShow(72, 2, DIST, 3, 2, 16);						  //�������벢��ʾ
	OLED_DecimalShow(72, 4, Dynamic_check_value * Step_size * 1.0, 3, 2, 16); //����У��ֵ����ʾ
	//b=Dynamic_check_value*Step_size*1.0-	High_liquid_level[task_flag];
	b = Dynamic_check_value * Step_size * 1.0 - DIST;
	//b=( (float)( (int)( (b+0.005)*100 ) ) )/100;
	OLED_DecimalShow(72, 6, b, 3, 2, 16); //������������ݲ���ʾ
}
