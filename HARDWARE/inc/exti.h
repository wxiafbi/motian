#ifndef __exti_H
#define __exti_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//�ⲿ�ж� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/01  
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  



#define Maximum_check_value     300.00     //���У��ֵ
#define Minimum_check_value     70.00      //��СУ��ֵ
#define Initial_check_value     120.00     //��ʼУ��ֵ
#define Dynamic_check_value     test
#define Step_size               0.1     //����  0.01~10


extern u8 flag;
extern float DIST;             //����һ�������������¶�ֵ
extern float tempdata;          //����һ�������������¶�ֵ
extern float adcdata;           //����һ�������������ѹֵ
extern float TEST;
extern uint16_t test;
void EXTIX_Init(void);//IO��ʼ��
void OLED_SHOW(void);		
void retrieve_data(void);
#endif

