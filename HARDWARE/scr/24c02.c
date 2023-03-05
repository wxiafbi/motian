/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*             ʵ��24c02���ܵ�Դ�ļ�               */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //������Ҫ��ͷ�ļ�
#include "24c02.h"      //������Ҫ��ͷ�ļ�
#include "delay.h" 	    //������Ҫ��ͷ�ļ�			 
#include "iic.h"        //������Ҫ��ͷ�ļ�
#include "led.h"        //������Ҫ��ͷ�ļ�

/*-------------------------------------------------*/
/*����������24c02ָ���ĵ�ַ��һ�ֽ�����            */
/*��  ����ReadAddr:��Ҫ�����ݵĵ�ַ                */
/*����ֵ������������                               */
/*-------------------------------------------------*/
unsigned char AT24C02_ReadOneByte(unsigned char ReadAddr)
{				  
	unsigned char receive;         //����һ���������ڱ�����յ�������
		
	receive = 0;                   //���receive��׼����������
	IIC_Start();                   //IIC��ʼ�ź�
	IIC_Send_Byte(AT24C02_WADDR);  //����24c02������ַ+д����	   	
	IIC_Wait_Ack();                //�ȴ�Ӧ��
    IIC_Send_Byte(ReadAddr);       //������Ҫ�����ݵĵ�ַ 
	IIC_Wait_Ack();	               //�ȴ�Ӧ�� 
	IIC_Start();  	 	           //IIC��ʼ�ź�
	IIC_Send_Byte(AT24C02_RADDR);  //����24c02������ַ+������				   
	IIC_Wait_Ack();	               //�ȴ�Ӧ��
    receive=IIC_Read_Byte(0);	   //��һ�ֽ����ݣ�������receive������   
    IIC_Stop();                    //ֹͣ�ź�
	Delay_Ms(5);                   //�ӳ�
	return receive;                //���ض�ȡ������
}

/*-------------------------------------------------*/
/*��������ָ����ַд��һ������                     */
/*��  ����WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ           */
/*��  ����DataToWrite:Ҫд�������                 */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_WriteOneByte(unsigned char WriteAddr,unsigned char DataToWrite)
{				   	  	    																 
    IIC_Start();                    //��ʼ�ź�
	IIC_Send_Byte(AT24C02_WADDR);   //����������ַ+д����	 
	IIC_Wait_Ack();	                //�ȴ�Ӧ��
    IIC_Send_Byte(WriteAddr);       //����д�����ݵ�Ŀ�ĵ�ַ
	IIC_Wait_Ack(); 	 	        //�ȴ�Ӧ��									  		   
	IIC_Send_Byte(DataToWrite);     //����Ҫд�������							   
	IIC_Wait_Ack();  		        //�ȴ�Ӧ��	   
    IIC_Stop();                     //ֹͣ�ź�  
	Delay_Ms(5);                    //�ӳ�
}

/*-------------------------------------------------*/
/*��������ָ����ַд��һҳ���ݣ�8�ֽڣ�            */
/*��  ����WriteAddr  :д�����ݵ���ʼ��ַ           */
/*��  ����DataToWrite:Ҫд������ݣ�8���ֽڣ�      */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_WritePage(unsigned char WriteAddr,unsigned char *DataToWrite)
{				   	  	    																 
	unsigned char i;                   //����һ����������forѭ��   
 
	IIC_Start();                       //��ʼ�ź�
	IIC_Send_Byte(AT24C02_WADDR);      //����������ַ+д����	 
	IIC_Wait_Ack();	                   //�ȴ�Ӧ��
    IIC_Send_Byte(WriteAddr);          //����д�����ݵ�Ŀ�ĵ�ַ
	IIC_Wait_Ack(); 	 	           //�ȴ�Ӧ��	
			Delay_Us(20);
	for(i=0;i<8;i++){                  //24c02һҳ8���ֽڣ�ѭ��д��
		IIC_Send_Byte(DataToWrite[i]); //����Ҫд�������							   
		IIC_Wait_Ack();  		       //�ȴ�Ӧ��

	}	
    IIC_Stop();                        //ֹͣ�ź� 
	Delay_Ms(5);                       //�ӳ�	
}
/*-------------------------------------------------*/
/*����������ָ���ĵ�ַ��һҳ����(8�ֽ�)            */
/*��  ����ReadAddr:��Ҫ�����ݵĵ�ַ                */
/*��  ����Redata:������յ�������                  */
/*����ֵ������������                               */
/*-------------------------------------------------*/
void AT24C02_ReadPage(unsigned char ReadAddr, unsigned char *Redata)
{				  		
	unsigned char i;               //����һ����������forѭ��   
	
	IIC_Start();                   //IIC��ʼ�ź�
	IIC_Send_Byte(AT24C02_WADDR);  //����������ַ+д����	   	
	IIC_Wait_Ack();                //�ȴ�Ӧ��
    IIC_Send_Byte(ReadAddr);       //������Ҫ�����ݵĵ�ַ 
	IIC_Wait_Ack();	               //�ȴ�Ӧ�� 
	IIC_Start();  	 	           //IIC��ʼ�ź�
	IIC_Send_Byte(AT24C02_RADDR);  //����������ַ+������				   
	IIC_Wait_Ack();	               //�ȴ�Ӧ��
	for(i=0;i<7;i++){              //24c02һҳ8���ֽڣ�ѭ����ȡǰ7���ֽ�	
		Redata[i]=IIC_Read_Byte(1);//��һ�ֽ����ݣ�����ACK�ź� 
    }	
	Redata[7]=IIC_Read_Byte(0);    //�����һ�ֽ����ݣ�������ACK�ź� 
    IIC_Stop();                    //ֹͣ�ź�
	Delay_Ms(5);                   //�ӳ�
}
/*-------------------------------------------------*/
/*�����������濪��LED��״̬                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_SaveLEDSta(void)
{
	char sta;                                    //����1����������4��led��״̬
	
	sta = 0;
	if(!LED1_IN_STA) sta |= 0x01;                //����ǵ͵�ƽ���Ǵ�״̬������if��sta����λ0��λ
	if(!LED2_IN_STA) sta |= 0x02;                //����ǵ͵�ƽ���Ǵ�״̬������if��sta����λ1��λ
	if(!LED3_IN_STA) sta |= 0x04;                //����ǵ͵�ƽ���Ǵ�״̬������if��sta����λ2��λ
	if(!LED4_IN_STA) sta |= 0x08;                //����ǵ͵�ƽ���Ǵ�״̬������if��sta����λ3��λ
	
	AT24C02_WriteOneByte(0,0x55);                //24C02 ��ַ0 д��0x55
	AT24C02_WriteOneByte(1,sta);                 //24C02 ��ַ1 д��4��led��״̬
}
/*-------------------------------------------------*/
/*���������ظ�����LED��״̬                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void AT24C02_ResumeLEDSta(void)
{
	char key,sta;
	
	key = AT24C02_ReadOneByte(0);       //24C02 ��ַ0 ��������
	if(key == 0x55){                    //���24C02��ַ0 ��������0x55������if
		sta = AT24C02_ReadOneByte(1);   //24C02 ��ַ1 ����4��led��״̬		
		if(sta&0x01) LED1_ON;           //���sta����λ0��λ����LED1
		if(sta&0x02) LED2_ON;           //���sta����λ1��λ����LED2
		if(sta&0x04) LED3_ON;           //���sta����λ2��λ����LED3
		if(sta&0x08) LED4_ON;           //���sta����λ3��λ����LED4
	}else{                              //��֮24C02��ַ0 �����Ĳ���0x55������else
		Delay_Ms(500);                  //�ӳ�
		NVIC_SystemReset();	            //����
	}
}







