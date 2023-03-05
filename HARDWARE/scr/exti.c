#include "stm32f10x.h" //包含需要的头文件
#include "exti.h"
#include "key.h"
#include "delay.h"
#include "stmflash.h" //包含了flash的内容
#include "oled.h"
//#include "TF-LUNA.h"
//#include "main.h"
u8 flag = 1;
uint16_t test = Initial_check_value;
float temp_tof = 0;
unsigned int amp_valu = 0;
float High_liquid_level[5] = {0};
//外部中断初始化函数
void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	KEY_Init(); //	按键端口初始化

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能复用功能时钟

	//GPIO A5 中断线以及中断初始化配置   下降沿触发
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line = EXTI_Line5; //KEY1
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIO A6	  中断线以及中断初始化配置 下降沿触发 //KEY2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIO A7	  中断线以及中断初始化配置  下降沿触发	//KEY3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//三个通道都是5——9
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			 //使能按键KEY1所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		 //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);								 //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void OLED_SHOW(void)
{
	float b;
	OLED_Clear();
	STMFLASH_Read(0x8010000, &test, 1);
	OLED_ShowCHinese(0, 0, 0);
	OLED_ShowCHinese(36, 0, 1); //显示电量
	OLED_ShowCHinese(0, 2, 2);
	OLED_ShowCHinese(36, 2, 3); //显示距离
	OLED_ShowCHinese(0, 4, 4);
	OLED_ShowCHinese(18, 4, 5);
	OLED_ShowCHinese(36, 4, 6); //显示校验值
	OLED_ShowCHinese(0, 6, 7);
	OLED_ShowCHinese(18, 6, 8);
	OLED_ShowCHinese(36, 6, 9); //显示液位高
	OLED_ShowString(54, 0, ":");
	OLED_ShowString(54, 2, ":");
	OLED_ShowString(54, 4, ":");
	OLED_ShowString(54, 6, ":");
	OLED_DecimalShow(72, 0, adcdata, 3, 2, 16);								  //读出电量值并显示
	OLED_DecimalShow(72, 2, DIST, 3, 2, 16);						  //读出距离并显示
	OLED_DecimalShow(72, 4, Dynamic_check_value * Step_size * 1.0, 3, 2, 16); //读出校验值并显示
	//b=Dynamic_check_value*Step_size*1.0-	High_liquid_level[task_flag];
	b = Dynamic_check_value * Step_size * 1.0 - DIST;
	//b=( (float)( (int)( (b+0.005)*100 ) ) )/100;
	OLED_DecimalShow(72, 6, b, 3, 2, 16); //读出处理后数据并显示
}
