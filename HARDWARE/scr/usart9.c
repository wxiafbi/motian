#include "delay.h"
#include "usart3.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "usart9.h"
#include "stdlib.h"

//???????
uint8_t USART3_RX_BUF[USART3_MAX_RECV_LEN]; //????,??USART3_MAX_RECV_LEN???.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN];      //????,??USART3_MAX_SEND_LEN??

//????????2????????????10ms?????????????.
//??2?????????10ms,?????1?????.?????10ms?????
//????,?????????.
//????????
//[15]:0,???????;1,????????.
//[14:0]:????????
vu16 USART3_RX_STA = 0;
float TFMINI_Distance, result;
int32_t temp;
int dis_status, Signal_Strength,ampdata;

//???IO ??3
// pclk1:PCLK1????(Mhz)
// bound:???
void usart3_init(u32 bound)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // GPIOB??
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //??3????

    USART_DeInit(USART3);                        //????3
                                                 // USART3_TX   PB10
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10; // PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; //??????
    GPIO_Init(GPIOB, &GPIO_InitStructure);           //???PB10

    // USART3_RX	  PB11
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????
    GPIO_Init(GPIOB, &GPIO_InitStructure);                //???PB11

    USART_InitStructure.USART_BaudRate            = bound;                          //????????9600;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            //???8?????
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               //?????
    USART_InitStructure.USART_Parity              = USART_Parity_No;                //??????
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //????????
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //????

    USART_Init(USART3, &USART_InitStructure); //?????	3

    USART_Cmd(USART3, ENABLE); //????

    //??????
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //????

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      //?????3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;      //????3
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; // IRQ????
    NVIC_Init(&NVIC_InitStructure);                                //??????????VIC???

    USART3_RX_STA = 0; //??
}

//??3,printf ??
//???????????USART3_MAX_SEND_LEN??
void u3_printf(char *fmt, ...) //...??????(????????????,???????????),????????
{
    u16 i, j;
    va_list ap;                               //??????????????
    va_start(ap, fmt);                        //?????????????ap,?ap???????????
    vsprintf((char *)USART3_TX_BUF, fmt, ap); //???fmt?ap??????????????????,?(char*)USART3_TX_BUF???,????sprintf(),????????
    va_end(ap);
    i = strlen((const char *)USART3_TX_BUF); //?????????
    for (j = 0; j < i; j++)                  //??????
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
            ;                                     //????,??????
        USART_SendData(USART3, USART3_TX_BUF[j]); //????????????????
    }
}

void USART3_RX_Data()
{
    u8 len = 0;
    if (USART3_RX_STA & 0x8000) {
        len                = USART3_RX_STA & 0X7FFF; //????????????
        USART3_RX_BUF[len] = 0;                      //?????

        if (len > USART3_MAX_RECV_LEN - 2) {
            len                = USART3_MAX_RECV_LEN - 1;
            USART3_RX_BUF[len] = 0; //?????
        }

        USART3_RX_BUF[USART3_MAX_RECV_LEN - 1] = 0x01;
        //			u3_printf("%s\r\n",USART3_RX_BUF);
        USART3_RX_STA = 0;
    }
}

void USART_SendString(USART_TypeDef *USARTx, char *DataString)
{
    int i = 0;
    USART_ClearFlag(USARTx, USART_FLAG_TC); //??????????(?????????????)
    while (DataString[i] != '\0')           //??????
    {
        USART_SendData(USARTx, DataString[i]); //????????????
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == 0)
            ;                                   //????????
        USART_ClearFlag(USARTx, USART_FLAG_TC); //??????????
        i++;
    }
}

/**

@brief ??????
@param pUSARTx???? Data??
@retval ?
**/
void Usart_SendByte(USART_TypeDef *USARTx, uint8_t Data)
{
    USART_SendData(USARTx, Data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
}
/**

@brief ??????
@param pUSARTx???? data????? num????
@retval ?
*/
void Usart_SendArray(USART_TypeDef *USARTx, uint8_t *data, uint8_t num)
{
    uint8_t t;
    for (t = 0; t < num; t++) {
        Usart_SendByte(USARTx, data[t]);
    }
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        ;
}