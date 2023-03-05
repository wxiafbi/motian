#ifndef __exti_H
#define __exti_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//外部中断 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/01  
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved	  
////////////////////////////////////////////////////////////////////////////////// 	  



#define Maximum_check_value     300.00     //最大校验值
#define Minimum_check_value     70.00      //最小校验值
#define Initial_check_value     120.00     //初始校验值
#define Dynamic_check_value     test
#define Step_size               0.1     //步长  0.01~10


extern u8 flag;
extern float DIST;             //定义一个变量，保存温度值
extern float tempdata;          //定义一个变量，保存温度值
extern float adcdata;           //定义一个变量，保存电压值
extern float TEST;
extern uint16_t test;
void EXTIX_Init(void);//IO初始化
void OLED_SHOW(void);		
void retrieve_data(void);
#endif

