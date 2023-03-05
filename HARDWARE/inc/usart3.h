/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���3���ܵ�ͷ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __USART2_H
#define __USART2_H

#include "stdio.h"      //������Ҫ��ͷ�ļ�
#include "stdarg.h"	    //������Ҫ��ͷ�ļ� 
#include "string.h"     //������Ҫ��ͷ�ļ�

#define USART2_RX_ENABLE     1      //�Ƿ������չ���  1������  0���ر�
#define USART2_TXBUFF_SIZE   1024   //���崮��3 ���ͻ�������С 1024�ֽ�

#if  USART2_RX_ENABLE                          //���ʹ�ܽ��չ���
#define USART2_RXBUFF_SIZE   1024              //���崮��3 ���ջ�������С 1024�ֽ�
extern char Usart2_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart2_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���
#endif
extern char DMA_flag;

void Usart2_Init(unsigned int);
void Usart2_IDELInit(unsigned int);
void u2_printf(char*,...) ;          
void u2_TxData(unsigned char *);
void Usart2DMA_init(void);

#endif

