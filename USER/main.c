/*-----------------------------------------------------*/
/*              超纬电子STM32系列开发板                */
/*-----------------------------------------------------*/
/*                     程序结构                        */
/*-----------------------------------------------------*/
/*USER     ：包含程序的main函数，是整个程序的入口      */
/*HARDWARE ：包含开发板各种功能外设的驱动程序          */
/*CORE     ：包含STM32的核心程序，官方提供，我们不修改 */
/*STLIB    ：官方提供的库文件，我们不修改              */
/*HMAC     ：各种加密算法                              */
/*-----------------------------------------------------*/
/*                                                     */
/*           程序main函数，入口函数源文件              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h" //包含需要的头文件
#include "main.h"      //包含需要的头文件
#include "delay.h"     //包含需要的头文件
#include "usart1.h"    //包含需要的头文件
#include "usart3.h"    //包含需要的头文件
#include "timer2.h"    //包含需要的头文件
#include "timer3.h"    //包含需要的头文件
#include "timer4.h"    //包含需要的头文件
#include "cat1.h"      //包含需要的头文件
#include "led.h"       //包含需要的头文件
#include "mqtt.h"      //包含需要的头文件
#include "key.h"       //包含需要的头文件
#include "aht10.h"     //包含需要的头文件
#include "iic.h"       //包含需要的头文件
#include "adc.h"       //包含需要的头文件
#include "iwdg.h"      //包含需要的头文件
#include "24c02.h"     //包含需要的头文件
#include "usart9.h"    //包含需要的头文件
#include "exti.h"
#include "beep.h"
#include "stmflash.h"
#include "oled.h"
#include "cmd.h"

extern int finaldata1;
extern char fina_data1[5];
unsigned int x;
unsigned int SystemTimer = 0;                        // 用于全局计时的变量           单位秒
unsigned int TEHUTimer   = 0;                        // 用于温湿度采集的计时的变量   单位秒
unsigned int ADCTimer    = 0;                        // 用于ADC采集的计时的变量      单位秒
u8 DataString[8]         = {0x80, 0x06, 0x02, 0x78}; // 串口查询命令
u8 TX_flag               = 0;                        // 发送标志位置零
float MAX = 0.0, TEMP = 0.0, DIST = 0.0;
float Disce[150] = {0};
float LUNA       = 0;
float tempdata; // 用于保存传感器表面温度值，传感器表面温度值高于气温20度

float adcdata; // 用于保存3个ADC通道的数据

int main(void)
{
    // int i; // 用于for循环

    // IWDG_Init(6, 625); // 独立看门狗 时间4s
    Delay_Init();      // 延时功能初始化
    Usart1_Init(9600); // 串口1功能初始化，波特率9600
    u1_printf("第一步");
    EXTIX_Init(); // 外部中断初始化
    usart3_init(38400);
    beep_init();

    LED_Init(); // LED初始化
    //	IIC_Init();                     //初始化IIC接口
    //	AHT10_Init();                   //初始化AHT10
    //	KEY_Init();                     //按键初始化
    Adc_Init();  // ADC初始化
    OLED_Init(); // OLED屏幕初始化
    // IoT_Parameter_Init();                                                    // 初始化连接云IoT平台MQTT服务器的参数
    // CAT1_GIPO_Init();                                                        // 初始化4G模块的控制IO
    // STMFLASH_Read(0x8010000, &test, 1);                                      // 读取FLASH储存的校验值
    adcdata = 2 * (float)(Get_Adc_Average(ADC_Channel_4, 8)) * (3.3 / 4096); // 获取计算后的带小数的实际电压值
    while (1)                                                                // 主循环
    {
        
    }
}
/*-------------------------------------------------*/
/*函数名：判断开关1状态，并发布给服务器            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED1_State(void)
{
    char temp[TBUFF_UNIT]; // 定义一个临时缓冲区

    memset(temp, 0, TBUFF_UNIT);
    if (LED1_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta1\":0,\"Switch1\":0},\"version\":\"1.0.0\"}"); // 如果LED1是高电平，说明是熄灭状态，需要回复关闭状态给服务器
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta1\":1,\"Switch1\":1},\"version\":\"1.0.0\"}"); // 如果LED1是低电平，说明是点亮状态，需要回复打开状态给服务器
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // 添加数据，发布给服务器
}
/*-------------------------------------------------*/
/*函数名：判断开关2状态，并发布给服务器            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED2_State(void)
{
    char temp[TBUFF_UNIT]; // 定义一个临时缓冲区

    memset(temp, 0, TBUFF_UNIT);
    if (LED2_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta2\":0,\"Switch2\":0},\"version\":\"1.0.0\"}"); // 如果LED2是高电平，说明是熄灭状态，需要回复关闭状态给服务器
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta2\":1,\"Switch2\":1},\"version\":\"1.0.0\"}"); // 如果LED2是低电平，说明是点亮状态，需要回复打开状态给服务器
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // 添加数据，发布给服务器
}
/*-------------------------------------------------*/
/*函数名：判断开关3状态，并发布给服务器            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED3_State(void)
{
    char temp[TBUFF_UNIT]; // 定义一个临时缓冲区

    memset(temp, 0, TBUFF_UNIT);
    if (LED3_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta3\":0,\"Switch3\":0},\"version\":\"1.0.0\"}"); // 如果LED3是高电平，说明是熄灭状态，需要回复关闭状态给服务器
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta3\":1,\"Switch3\":1},\"version\":\"1.0.0\"}"); // 如果LED3是低电平，说明是点亮状态，需要回复打开状态给服务器
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // 添加数据，发布给服务器
}
/*-------------------------------------------------*/
/*函数名：判断开关4状态，并发布给服务器            */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED4_State(void)
{
    char temp[TBUFF_UNIT]; // 定义一个临时缓冲区

    memset(temp, 0, TBUFF_UNIT);
    if (LED4_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta4\":0,\"Switch4\":0},\"version\":\"1.0.0\"}"); // 如果LED4是高电平，说明是熄灭状态，需要回复关闭状态给服务器
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta4\":1,\"Switch4\":1},\"version\":\"1.0.0\"}"); // 如果LED4是低电平，说明是点亮状态，需要回复打开状态给服务器
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // 添加数据，发布给服务器
}
/*-------------------------------------------------*/
/*函数名：判断4个开关状态，并发布给服务器          */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_State(void)
{
    char temp[TBUFF_UNIT];       // 定义一个临时缓冲区
    char sta1, sta2, sta3, sta4; // 定义4个变量保存各个led的状态

    if (LED1_IN_STA)
        sta1 = 0; // 如果是高电平，是关闭状态，状态值=0
    else
        sta1 = 1; // 如果是低电平，是打开状态，状态值=1
    if (LED2_IN_STA)
        sta2 = 0; // 如果是高电平，是关闭状态，状态值=0
    else
        sta2 = 1; // 如果是低电平，是打开状态，状态值=1
    if (LED3_IN_STA)
        sta3 = 0; // 如果是高电平，是关闭状态，状态值=0
    else
        sta3 = 1; // 如果是低电平，是打开状态，状态值=1
    if (LED4_IN_STA)
        sta4 = 0; // 如果是高电平，是关闭状态，状态值=0
    else
        sta4 = 1; // 如果是低电平，是打开状态，状态值=1

    memset(temp, 0, TBUFF_UNIT);
    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta1\":%d,\"SwitchSta2\":%d,\"SwitchSta3\":%d,\"SwitchSta4\":%d,\"Switch1\":%d,\"Switch2\":%d,\"Switch3\":%d,\"Switch4\":%d},\"version\":\"1.0.0\"}", sta1, sta2, sta3, sta4, sta1, sta2, sta3, sta4); // 需要回复状态给服务器

    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); // 添加数据，发布给服务器
}
/*-------------------------------------------------*/
/*函数名：定时上传各种数据的任务                   */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void Data_State(void)
{
    int u = 0;
    char temp[256];                        // 报文有效负载缓冲区
    if ((SystemTimer - TEHUTimer) >= 19) { // 全局时间 和 温湿度计时时间至少差25s，进入if
        TEHUTimer = SystemTimer;           // 把当前的全局时间，记录到温湿度计时变量	                           //读取温湿度值
        while (u == 0) {
            Usart_SendArray(USART3, DataString, 4);
            result = finaldata1 / 10.0;
            DIST   = result;
            u1_printf("距离值：%.2f\r\n", DIST);
            tempdata = dis_status;
            u1_printf("温度值：%.2f\r\n", tempdata);
            ampdata = Signal_Strength;
            u1_printf("反射强度：%d\r\n", ampdata);
            u += 1;
            if (ampdata < 0 || ampdata == 65535 || DIST > 550.00 || tempdata == 255 || DIST <= 0) {
                u1_printf("错误\r\n");
                u = 0;
            }
        }
        if (TX_flag == 0) {
            memset(temp, 0, 256);                                                                                                                                                                                    // 清空缓冲区
            u1_printf("液位高：%.2f\r\n", Dynamic_check_value * Step_size * 1.0 - DIST);                                                                                                                             // 串口输出数据
            sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Distance\":%.2f,\"amp\":%d},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0 - DIST, ampdata); // 构建数据
            MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                       // 添加数据到发送缓冲区
        }
        if (TX_flag == 1) {
            memset(temp, 0, 256);                                                                                                                                                                                      // 清空缓冲区
            u1_printf("液位高1：%.2f\r\n", Dynamic_check_value * Step_size * 1.0 - DIST);                                                                                                                              // 串口输出数据
            sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Distance1\":%3.2f,\"amp\":%d},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0 - DIST, ampdata); // 构建数据
            MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                         // 添加数据到发送缓冲区
        }
        if (TX_flag == 2) {
            memset(temp, 0, 256);                                                                                                                                                                                      // 清空缓冲区
            u1_printf("液位高2：%.2f\r\n", Dynamic_check_value * Step_size * 1.0 - DIST);                                                                                                                              // 串口输出数据
            sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Distance2\":%3.2f,\"amp\":%d},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0 - DIST, ampdata); // 构建数据
            MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                         // 添加数据到发送缓冲区
        }
        TX_flag++;
        if (TX_flag == 3) {
            TX_flag = 0;
        }
        if (!flag) {
            OLED_SHOW();
        }
    }

    if ((SystemTimer - ADCTimer) >= 39) {                                        // 全局时间 和 温湿度计时时间至少差65s，进入if
        ADCTimer = SystemTimer;                                                  // 把当前的全局时间，记录到温湿度计时变量
        memset(temp, 0, 256);                                                    // 清空缓冲区
        adcdata = 2 * (float)(Get_Adc_Average(ADC_Channel_4, 8)) * (3.3 / 4096); // 获取计算后的带小数的实际电压值
        u1_printf("valu：%.2f\r\n", Dynamic_check_value * Step_size * 1.0);      // 串口输出数据
        u1_printf("ADC：%.2f\r\n", adcdata);
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"valu\":%3.2f,\"temp\":%.2f,\"electricity\":%.2f},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0, tempdata, adcdata); // 构建数据
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                                               // 添加数据到发送缓冲区
    }
}
