#include "delay.h"      //包含需要的头文件
#include "beep.h"      //包含需要的头文件

void beep_init(void){
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(BEEP_BUS, ENABLE);	 //使能PC端口时钟
		GPIO_InitStructure.GPIO_Pin = BEEP_GPIO;				 //LED0-->BEEP 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(BEEP, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
}



void beep_ctrl(u8 on){
	if (on)
		GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP 输出高
	else
		GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP 输出低
}

void beep_on_500ms(){
			GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP 输出高
			Delay_Ms(500);
			GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP 输出低
			//delay_ms(500);
}


void beep_on_200ms(){
			GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP 输出高
			Delay_Ms(200);
			GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP 输出低
			//delay_ms(500);
}

void beep_on_100ms(){
			GPIO_SetBits(BEEP,BEEP_GPIO);						 //BEEP 输出高
			Delay_Ms(100);
			GPIO_ResetBits(BEEP,BEEP_GPIO);						 //BEEP 输出低
			//delay_ms(500);
}


