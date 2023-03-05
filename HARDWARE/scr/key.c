#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "sys.h"							    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTC,PORTEʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA6���ó����룬Ĭ������  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(TOUCH==1))
	{
		Delay_Ms(10);//ȥ���� 
		key_up=0;
		if(TOUCH==1)return KEY_UP;
	}else if(TOUCH==0)key_up=1; 	    
 	return 0;// �ް�������
}

void KEY_CTRL(){
		u8 t,i;
		t=KEY_Scan(1);		//�õ���ֵ  
		if (t == KEY_UP)
		{		
				if (i){
					i = 0;
				}	else {
					i = 1;
				}
				Delay_Ms(500);
		}
}
