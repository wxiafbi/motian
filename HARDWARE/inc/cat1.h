/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              操作4G模块功能的头文件             */
/*                                                 */
/*-------------------------------------------------*/

#ifndef __CAT1_H
#define __CAT1_H

#define RESET_IO(x)       GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)x)  //PB2控制复位IO
#define LINKA_STA         GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)        //PB0,读取电平状态,可以判断连接A是否建立
#define LINKB_STA         GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)        //PB1,读取电平状态,可以判断连接B是否建立

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


