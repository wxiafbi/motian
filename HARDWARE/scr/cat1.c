/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ����4Gģ�鹦�ܵ�Դ�ļ�              */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "main.h"         //������Ҫ��ͷ�ļ�
#include "cat1.h"	      //������Ҫ��ͷ�ļ�
#include "delay.h"	      //������Ҫ��ͷ�ļ�
#include "usart1.h"	      //������Ҫ��ͷ�ļ�
#include "usart3.h"	      //������Ҫ��ͷ�ļ�
#include "led.h"          //������Ҫ��ͷ�ļ�
#include "key.h"          //������Ҫ��ͷ�ļ�
#include "mqtt.h"         //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������ʼ��4Gģ���IO                         */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void CAT1_GIPO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                     //����һ������GPIO�ı���
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��GPIOA�˿�ʱ��			
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��GPIOB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                //׼������PB2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO����50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //���������ʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //����PB2
	RESET_IO(0);                                             //��λIO���͵�ƽ
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;  //׼������PB0 1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   		 //�������룬Ĭ��״̬�Ǹߵ�ƽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);                   //����PB0 1
}
/*-------------------------------------------------*/
/*����������λ4Gģ��                               */
/*��  ����timeout�� ��ʱʱ�䣨1s�ı�����           */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_Reset(int timeout)
{	
	RESET_IO(1);                  //��λIO���ߵ�ƽ
	Delay_Ms(500);                //��ʱ
	RESET_IO(0);                  //��λIO���͵�ƽ
	
	while(timeout--){                         //�ȴ���ʱʱ�䵽0
		Delay_Ms(1000);                       //��ʱ1s
		if(strstr(CAT1_RX_BUF,"WH-GM5"))      //������յ�WH-GM5��ʾ�ɹ�
			break;       					  //��������whileѭ��
		u1_printf("%d ",timeout);             //����������ڵĳ�ʱʱ��
	}	
	u1_printf("\r\n");                        //���������Ϣ
	if(timeout<=0)return 1;                   //��ʱ���󣬷���1
	return 0;                                 //��ȷ����0	
}	
/*-------------------------------------------------*/
/*��������4Gģ���˳�͸��                           */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_ExitTrans(int timeout)
{	
	CAT1_RxCounter=0;                         //������������������                        
	memset(CAT1_RX_BUF,0,CAT1_RXBUFF_SIZE);   //��ս��ջ����� 
	CAT1_printf("+++");                       //����ָ��		
	while(timeout--){                         //�ȴ���ʱʱ�䵽0
		Delay_Ms(100);                        //��ʱ100ms
		if(strstr(CAT1_RX_BUF,"a"))           //������յ�a��ʾ�ɹ�
			break;       					  //��������whileѭ��
		u1_printf("%d ",timeout);             //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                        //���������Ϣ
	if(timeout<=0)return 1;                   //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�a������1
	else{
		CAT1_RxCounter=0;                         //������������������                        
		memset(CAT1_RX_BUF,0,CAT1_RXBUFF_SIZE);   //��ս��ջ����� 
		CAT1_printf("a");                         //����ָ��			
		while(timeout--){                         //�ȴ���ʱʱ�䵽0
			Delay_Ms(100);                        //��ʱ100ms
			if(strstr(CAT1_RX_BUF,"+ok"))         //������յ�+ok��ʾ�ɹ�
				break;       					  //��������whileѭ��
			u1_printf("%d ",timeout);             //����������ڵĳ�ʱʱ��
		}
		u1_printf("\r\n");                        //���������Ϣ
		if(timeout<=0)return 2;                   //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+ok������1	
	}
    return 	0;                                    //��ȷ ����0
}	
/*-------------------------------------------------*/
/*��������4Gģ�鷢������ָ��                       */
/*��  ����cmd��ָ��                                */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_SendCmd(char *cmd, int timeout)
{
	CAT1_RxCounter=0;                           //CAT1������������������                        
	memset(CAT1_RX_BUF,0,CAT1_RXBUFF_SIZE);     //���CAT1���ջ����� 
	CAT1_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		Delay_Ms(100);                          //��ʱ100ms
		if(strstr(CAT1_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						//��������whileѭ��
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				//��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}
/*-------------------------------------------------*/
/*��������4Gģ���źż��                           */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_CSQ(int timeout)
{
	char temp[20];
	char csq[20];
	
	CAT1_RxCounter=0;                           //CAT1������������������                        
	memset(CAT1_RX_BUF,0,CAT1_RXBUFF_SIZE);     //���CAT1���ջ����� 
	CAT1_printf("AT+CSQ\r\n");                  //����ָ��
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		Delay_Ms(100);                          //��ʱ100ms
		if(strstr(CAT1_RX_BUF,","))            //������յ�,��ʾָ��ɹ�
			break;       						//��������whileѭ��
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�,����1
	else {
		sscanf(CAT1_RX_BUF,"%[^:]:%[^,],%[^,],%[^\r]",temp,csq,temp,temp);//�������
		u1_printf("�ź�ǿ��:%s\r\n",csq);                                 //������ʾ��Ϣ
	}
	return 0;		                            //��ȷ ����0        				
}
/*-------------------------------------------------*/
/*��������4Gģ���ѯ������Ϣ                       */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ����                                       */
/*-------------------------------------------------*/
char CAT1_SYSINFO(int timeout)
{
	char temp[20];
	char info[20];
	
	CAT1_RxCounter=0;                           //CAT1������������������                        
	memset(CAT1_RX_BUF,0,CAT1_RXBUFF_SIZE);     //���CAT1���ջ����� 
	CAT1_printf("AT+SYSINFO\r\n");              //����ָ��
	while(timeout--){                           //�ȴ���ʱʱ�䵽0
		Delay_Ms(100);                          //��ʱ100ms
		if(strstr(CAT1_RX_BUF,","))            //������յ�,��ʾָ��ɹ�
			break;       						//��������whileѭ��
		u1_printf("%d ",timeout);               //����������ڵĳ�ʱʱ��
	}
	u1_printf("\r\n");                          //���������Ϣ
	if(timeout<=0)return 1;                     //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�,����1
	else {
		sscanf(CAT1_RX_BUF,"%[^:]:%[^,],%[^\r]",temp,temp,info);          //�������
		u1_printf("��ǰ������ʽ:%s\r\n",info);                            //������ʾ��Ϣ
	}
	return 0;		                            //��ȷ ����0        				
}
/*-------------------------------------------------*/
/*��������4Gģ�����ӷ�����                         */
/*��  ����timeout�� ��ʱʱ�䣨100ms�ı�����        */
/*����ֵ��0����ȷ   ����������                     */
/*-------------------------------------------------*/
char CAT1_Connect_IoTServer(int timeout)
{			
	char temp[256];
	
	u1_printf("׼����λģ��\r\n");                     //������ʾ����
	if(CAT1_Reset(20)){                                //��λ��1s��ʱ��λ���ܼ�20s��ʱʱ��
		u1_printf("��λʧ�ܣ�׼������\r\n");           //���ط�0ֵ������if��������ʾ����
		return 1;                                      //����1
	}else u1_printf("��λ�ɹ�\r\n");                   //������ʾ����
	
	u1_printf("׼���˳�͸��\r\n");                     //������ʾ����
	if(CAT1_ExitTrans(50)){                            //�˳�͸����100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("�˳�͸��ʧ�ܣ�׼������\r\n");       //���ط�0ֵ������if��������ʾ����
		return 2;                                      //����2
	}else u1_printf("�˳�͸���ɹ�\r\n");               //������ʾ����
			
	u1_printf("׼������NETģʽ\r\n");                  //������ʾ����
	if(CAT1_SendCmd("AT+WKMOD=NET",50)){               //����NETģʽ��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("����NETģʽʧ�ܣ�׼������\r\n");    //���ط�0ֵ������if��������ʾ����
		return 5;                                      //����5
	}else u1_printf("����NETģʽ�ɹ�\r\n");            //������ʾ����
	
	u1_printf("׼��ʹ������A\r\n");                    //������ʾ����
	if(CAT1_SendCmd("AT+SOCKAEN=ON",50)){              //ʹ������A��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("ʹ������Aʧ�ܣ�׼������\r\n");      //���ط�0ֵ������if��������ʾ����
		return 6;                                      //����6
	}else u1_printf("ʹ������A�ɹ�\r\n");              //������ʾ����	
	
	u1_printf("׼���ر�����B\r\n");                    //������ʾ����
	if(CAT1_SendCmd("AT+SOCKBEN=OFF",50)){             //�ر�����B��100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("�ر�����Bʧ�ܣ�׼������\r\n");      //���ط�0ֵ������if��������ʾ����
		return 7;                                      //����7
	}else u1_printf("�ر�����B�ɹ�\r\n");              //������ʾ����
	
	u1_printf("׼����������A������\r\n");                  //������ʾ����
	if(CAT1_SendCmd("AT+SOCKASL=LONG",50)){                //��������A�����ӣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("��������A������ʧ�ܣ�׼������\r\n");    //���ط�0ֵ������if��������ʾ����
		return 8;                                          //����8
	}else u1_printf("��������A�����ӳɹ�\r\n");            //������ʾ����	
	 
	u1_printf("׼���ر��Դ���������\r\n");                 //������ʾ����
	if(CAT1_SendCmd("AT+HEARTEN=OFF",50)){                 //�ر��Դ��������ܣ�100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("�ر��Դ���������ʧ�ܣ�׼������\r\n");   //���ط�0ֵ������if��������ʾ����
		return 9;                                          //����9
	}else u1_printf("�ر��Դ��������ܳɹ�\r\n");           //������ʾ����		
	
	u1_printf("׼����������A���ӷ�����\r\n");                //������ʾ����
	sprintf(temp,"AT+SOCKA=TCP,%s,%d",ServerIP,ServerPort);  //��������
	if(CAT1_SendCmd(temp,50)){                               //��������A���ӷ�������100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("��������A���ӷ�����ʧ�ܣ�׼������\r\n");  //���ط�0ֵ������if��������ʾ����
		return 10;                                           //����10
	}else u1_printf("��������A���ӷ������ɹ�\r\n");          //������ʾ����				
	
	u1_printf("�������ã�Ȼ������ģ��\r\n");                 //������ʾ����
	if(CAT1_SendCmd("AT+S",50)){                             //�������ã�Ȼ������ģ�飬100ms��ʱ��λ���ܼ�5s��ʱʱ��
		u1_printf("��������ʧ�ܣ�׼������\r\n");             //���ط�0ֵ������if��������ʾ����
		return 11;                                           //����11
	}else u1_printf("�������óɹ�\r\n");                     //������ʾ����
	
	u1_printf("�ȴ����������ӷ�����\r\n");  //������ʾ����
	while(timeout--){                       //�ȴ���ʱʱ�䵽0
		Delay_Ms(1000);                     //��ʱ1s
		if(LINKA_STA==0)                    //��LINKA_STA PC1�ǵ͵�ƽ�ˣ���ʾ���ӳɹ�
			break;                          //����whileѭ��  
		u1_printf("%d ",timeout);           //����������ڵĳ�ʱʱ��  
	}	
	u1_printf("\r\n");                      //���������Ϣ
	if(timeout<=0)return 12;                //��ʱ���󣬷���12

	Delay_Ms(500);                          //��ʱ
	return 0;                               //��ȷ����0	
}
