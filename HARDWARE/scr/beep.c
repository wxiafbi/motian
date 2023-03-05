#include "delay.h"      //������Ҫ��ͷ�ļ�
#include "beep.h"      //������Ҫ��ͷ�ļ�

void beep_init(void){
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(BEEP_BUS, ENABLE);	 //ʹ��PC�˿�ʱ��
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO;				 //LED0-->BEEP �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(BEEP, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
}



void beep_ctrl(u8 on){
	if (on)
		GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP �����
	else
		GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP �����
}

void beep_on_500ms(){
			GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP �����
			Delay_Ms(500);
			GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP �����
			//delay_ms(500);
}


void beep_on_200ms(){
			GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP �����
			Delay_Ms(200);
			GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP �����
			//delay_ms(500);
}

void beep_on_100ms(){
			GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP �����
			Delay_Ms(100);
			GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP �����
			//delay_ms(500);
}


