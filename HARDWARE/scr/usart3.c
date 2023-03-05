/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ�ִ���3���ܵ�Դ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "usart3.h"     //������Ҫ��ͷ�ļ�
#include "timer4.h"     //������Ҫ��ͷ�ļ�
#include "mqtt.h"       //������Ҫ��ͷ�ļ�

#if  USART2_RX_ENABLE                   //���ʹ�ܽ��չ���
char Usart2_RxCompleted = 0;            //����һ������ 0����ʾ����δ��� 1����ʾ������� 
unsigned int Usart2_RxCounter = 0;      //����һ����������¼����3�ܹ������˶����ֽڵ�����
char Usart2_RxBuff[USART2_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��3���յ�������   	
#endif

char DMA_flag = 0;                      // 0 DMA���Ϳ���   1 DMA������

/*-------------------------------------------------*/
/*����������ʼ������3���͹���                      */
/*��  ����bound��������                            */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart2_Init(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
#if USART2_RX_ENABLE                         //���ʹ�ܽ��չ���
	NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
#endif

#if USART2_RX_ENABLE                                 //���ʹ�ܽ��չ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
#endif	
      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //ʹ�ܴ���3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOBʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;             //׼������PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���3�ķ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PB10
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             //׼������PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���3�Ľ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PB11
	
	USART_InitStructure.USART_BaudRate = bound;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
#if USART2_RX_ENABLE               												   //���ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
#else                                                                              //�����ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //ֻ��ģʽ
#endif        
    USART_Init(USART2, &USART_InitStructure);                                      //���ô���3	

#if USART2_RX_ENABLE  	         					        //���ʹ�ܽ���ģʽ
	USART_ClearFlag(USART2, USART_FLAG_RXNE);	            //������ձ�־λ
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //���������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //���ô���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //���ô���3�ж�
#endif  
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);            //ʹ�ܴ���3��DMA����
	USART_Cmd(USART2, ENABLE);                              //ʹ�ܴ���3
}
/*-------------------------------------------------*/
/*����������ʼ������3 ����DMA�Ϳ����ж�              */
/*��  ����bound��������                             */
/*����ֵ����                                        */
/*-------------------------------------------------*/
void Usart2_IDELInit(unsigned int bound)
{  	 	
    GPIO_InitTypeDef GPIO_InitStructure;     //����һ������GPIO���ܵı���
	USART_InitTypeDef USART_InitStructure;   //����һ�����ô��ڹ��ܵı���
#if USART2_RX_ENABLE                         //���ʹ�ܽ��չ���
	NVIC_InitTypeDef NVIC_InitStructure;     //���ʹ�ܽ��չ��ܣ�����һ�������жϵı���
#endif

#if USART2_RX_ENABLE                                 //���ʹ�ܽ��չ���
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3
#endif	
    USART_DeInit(USART2);                                  //����3 ���мĴ��� �ָ�Ĭ��ֵ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);  //ʹ�ܴ���3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOBʱ��
	USART_DeInit(USART2);                                  //����3�Ĵ�����������ΪĬ��ֵ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;             //׼������PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //IO����50M
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	       //����������������ڴ���3�ķ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PB10
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;             //׼������PB11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������룬���ڴ���3�Ľ���
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //����PB11
	
	USART_InitStructure.USART_BaudRate = bound;                                    //����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //8������λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
#if USART2_RX_ENABLE               												   //���ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	               //�շ�ģʽ
#else                                                                              //�����ʹ�ܽ���ģʽ
	USART_InitStructure.USART_Mode = USART_Mode_Tx ;	                           //ֻ��ģʽ
#endif        
    USART_Init(USART2, &USART_InitStructure);                                      //���ô���3	

#if USART2_RX_ENABLE  	         					        //���ʹ�ܽ���ģʽ
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);          //���������ж�
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       //���ô���3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //���ô���3�ж�
#endif  
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);            //��������3 DMA����
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);            //��������3 DMA����
	Usart2DMA_init();                                       //��ʼ��DMA
	USART_Cmd(USART2, ENABLE);                              //ʹ�ܴ���3
	
}
/*-------------------------------------------------*/
/*������������3 printf����                         */
/*��  ����char* fmt,...  ��ʽ������ַ����Ͳ���    */
/*����ֵ����                                       */
/*-------------------------------------------------*/

__align(8) char Usart2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart2_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart2_TxBuff);		
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = Usart2_TxBuff[i];
		while((USART2->SR&0X40)==0);	
	}	
}

/*-------------------------------------------------*/
/*������������3���ͻ������е�����                  */
/*��  ����data������                               */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void u2_TxData(unsigned char *data)
{
	DMA_flag = 1;                                                    //DMA��־=1  Ҫ��ʼ������
	DMA1_Channel7->CNDTR = (unsigned int)(data[0]*256+data[1]);      //�����������ݸ���   
	DMA1_Channel7->CMAR =  (unsigned int)(&data[2]);                 //�������ڴ��ַ
	DMA_Cmd(DMA1_Channel7,ENABLE);                                   //����DMA
}
/*-------------------------------------------------*/
/*������������3 DMA��ʼ��                           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Usart2DMA_init(void) 
{   
	DMA_InitTypeDef    DMA_Initstructure;    
	NVIC_InitTypeDef   NVIC_InitStructure;   
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);                               //����DMA1 ʱ��	
	//������ͨ��3 ����3�Ľ���
	DMA_Initstructure.DMA_PeripheralBaseAddr =  (unsigned int)(&USART2->DR);        //�����ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     = (unsigned int)Usart2_RxBuff;         //�ڴ��ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;                              //�����赽�ڴ� 
	DMA_Initstructure.DMA_BufferSize = USART2_RXBUFF_SIZE;                          //��С����Ϊ����3�Ļ�������С
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                //���費��
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;                          //�ڴ��ַ�Ĵ�������
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;         //�������ݿ��Ϊ8λ
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                 //�ڴ����ݿ��Ϊ8λ
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;                                   //����������ģʽ
	DMA_Initstructure.DMA_Priority = DMA_Priority_High;                             //ӵ�и����ȼ�
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;                                    //û������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel6,&DMA_Initstructure);	                                    //����ͨ��3
	DMA_Cmd(DMA1_Channel6,ENABLE);                                                  //ʹ��ͨ��3
	
	//������ͨ��2 ����3�ķ���
	DMA_Initstructure.DMA_PeripheralBaseAddr =(unsigned int)(&USART2->DR);          //�����ַ
	DMA_Initstructure.DMA_MemoryBaseAddr     =(unsigned int)(&MQTT_TxDataOutPtr[2]);//�ڴ��ַ
	DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;                              //���ڴ浽���� 
	DMA_Initstructure.DMA_BufferSize = TBUFF_UNIT;                                  //��С����ΪTBUFF_UNIT
	DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                //���費��
	DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;                          //�ڴ��ַ�Ĵ�������
	DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;         //�������ݿ��Ϊ8λ
	DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                 //�ڴ����ݿ��Ϊ8λ
	DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;                                   //����������ģʽ
	DMA_Initstructure.DMA_Priority = DMA_Priority_Medium;                           //ӵ���е����ȼ�
	DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;                                    //û������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel7,&DMA_Initstructure);	                                    //����ͨ��2
	DMA_Cmd(DMA1_Channel7,DISABLE);                                                 //�Ȳ�Ҫʹ��ͨ��2	
	
	DMA_ClearITPendingBit(DMA1_IT_TC7);                                             //���DMA1ͨ��2 ��������жϱ�־
	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);                                   //����DMA1ͨ��2 ��������ж�
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;                        //����DMA�ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 					              //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;							                  //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                                 //���ж� 
	NVIC_Init(&NVIC_InitStructure);                                                 //�����ж�
}
