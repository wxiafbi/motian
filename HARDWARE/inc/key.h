#ifndef __KEY_H
#define __KEY_H	 


#define TOUCH  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ����(touch) 
#define key1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
#define key2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);
#define key3 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);

#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
