/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*              ����4Gģ�鹦�ܵ�ͷ�ļ�             */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __CAT1_H
#define __CAT1_H

#define RESET_IO(x)       GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)x)  //PB2���Ƹ�λIO
#define LINKA_STA         GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)        //PB0,��ȡ��ƽ״̬,�����ж�����A�Ƿ���
#define LINKB_STA         GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)        //PB1,��ȡ��ƽ״̬,�����ж�����B�Ƿ���

#define CAT1_printf       u2_printf           
#define CAT1_RxCounter    Usart2_RxCounter    
#define CAT1_RX_BUF       Usart2_RxBuff       
#define CAT1_RXBUFF_SIZE  USART2_RXBUFF_SIZE  

void CAT1_GIPO_Init(void);
char CAT1_Reset(int);
char CAT1_ExitTrans(int);
char CAT1_SendCmd(char *, int);
char CAT1_CSQ(int);
char CAT1_SYSINFO(int);

char CAT1_Connect_IoTServer(int);
	
#endif


