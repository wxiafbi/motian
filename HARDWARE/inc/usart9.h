#ifndef __USART3_H
#define __USART3_H
#include "sys.h"

#define USART3_MAX_RECV_LEN 60       //?????????
#define USART3_MAX_SEND_LEN 600      //?????????
#define USART3_RX_EN        1        // 0,???;1,??.
#define L1MOD_USART         (USART3) // L1模组串口号

extern uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; //????,??USART3_MAX_RECV_LEN??
extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];      //????,??USART3_MAX_SEND_LEN??
extern vu16 USART3_RX_STA;                         //??????
extern float TFMINI_Distance, result;
extern int32_t temp;
extern int dis_status, Signal_Strength,ampdata;

void usart3_init(u32 bound); //??2???
void u3_printf(char *fmt, ...);
void USART3_RX_Data(void);
void USART_SendString(USART_TypeDef *USARTx, char *DataString);
void Usart_SendByte(USART_TypeDef *USARTx, uint8_t Data);
void Usart_SendArray(USART_TypeDef *USARTx, uint8_t *data, uint8_t num);
#endif
