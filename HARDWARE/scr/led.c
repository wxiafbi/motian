/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ʵ��LED���ܵ�Դ�ļ�                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�
#include "delay.h"

/*-------------------------------------------------*/
/*����������ʼ��LED����                            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //����һ������GPIO�ı���
		
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //ʹ��GPIOA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                //׼������ PA12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                     //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		                              //���������ʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		                              //����PA8 11 12
	
	LED_AllOff();   //����LEDϨ��
}

/*-------------------------------------------------*/
/*����������������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	LED3_ON;
}

/*-------------------------------------------------*/
/*��������Ϩ������LED                              */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	LED3_ON;
	Delay_Ms(5);
	LED3_OFF;
	Delay_Ms(5);
}



