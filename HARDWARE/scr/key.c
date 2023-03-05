#include "stm32f10x.h"
#include "key.h"
#include "delay.h"
#include "sys.h"							    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTC,PORTE时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA6设置成输入，默认上拉  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(TOUCH==1))
	{
		Delay_Ms(10);//去抖动 
		key_up=0;
		if(TOUCH==1)return KEY_UP;
	}else if(TOUCH==0)key_up=1; 	    
 	return 0;// 无按键按下
}

void KEY_CTRL(){
		u8 t,i;
		t=KEY_Scan(1);		//得到键值  
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
