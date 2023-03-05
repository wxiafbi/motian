/*-----------------------------------------------------*/
/*              ��γ����STM32ϵ�п�����                */
/*-----------------------------------------------------*/
/*                     ����ṹ                        */
/*-----------------------------------------------------*/
/*USER     �����������main��������������������      */
/*HARDWARE ��������������ֹ����������������          */
/*CORE     ������STM32�ĺ��ĳ��򣬹ٷ��ṩ�����ǲ��޸� */
/*STLIB    ���ٷ��ṩ�Ŀ��ļ������ǲ��޸�              */
/*HMAC     �����ּ����㷨                              */
/*-----------------------------------------------------*/
/*                                                     */
/*           ����main��������ں���Դ�ļ�              */
/*                                                     */
/*-----------------------------------------------------*/

#include "stm32f10x.h" //������Ҫ��ͷ�ļ�
#include "main.h"      //������Ҫ��ͷ�ļ�
#include "delay.h"     //������Ҫ��ͷ�ļ�
#include "usart1.h"    //������Ҫ��ͷ�ļ�
#include "usart3.h"    //������Ҫ��ͷ�ļ�
#include "timer2.h"    //������Ҫ��ͷ�ļ�
#include "timer3.h"    //������Ҫ��ͷ�ļ�
#include "timer4.h"    //������Ҫ��ͷ�ļ�
#include "cat1.h"      //������Ҫ��ͷ�ļ�
#include "led.h"       //������Ҫ��ͷ�ļ�
#include "mqtt.h"      //������Ҫ��ͷ�ļ�
#include "key.h"       //������Ҫ��ͷ�ļ�
#include "aht10.h"     //������Ҫ��ͷ�ļ�
#include "iic.h"       //������Ҫ��ͷ�ļ�
#include "adc.h"       //������Ҫ��ͷ�ļ�
#include "iwdg.h"      //������Ҫ��ͷ�ļ�
#include "24c02.h"     //������Ҫ��ͷ�ļ�
#include "usart9.h"    //������Ҫ��ͷ�ļ�
#include "exti.h"
#include "beep.h"
#include "stmflash.h"
#include "oled.h"
#include "cmd.h"

extern int finaldata1;
extern char fina_data1[5];
unsigned int x;
unsigned int SystemTimer = 0;                        // ����ȫ�ּ�ʱ�ı���           ��λ��
unsigned int TEHUTimer   = 0;                        // ������ʪ�Ȳɼ��ļ�ʱ�ı���   ��λ��
unsigned int ADCTimer    = 0;                        // ����ADC�ɼ��ļ�ʱ�ı���      ��λ��
u8 DataString[8]         = {0x80, 0x06, 0x02, 0x78}; // ���ڲ�ѯ����
u8 TX_flag               = 0;                        // ���ͱ�־λ����
float MAX = 0.0, TEMP = 0.0, DIST = 0.0;
float Disce[150] = {0};
float LUNA       = 0;
float tempdata; // ���ڱ��洫���������¶�ֵ�������������¶�ֵ��������20��

float adcdata; // ���ڱ���3��ADCͨ��������

int main(void)
{
    // int i; // ����forѭ��

    // IWDG_Init(6, 625); // �������Ź� ʱ��4s
    Delay_Init();      // ��ʱ���ܳ�ʼ��
    Usart1_Init(9600); // ����1���ܳ�ʼ����������9600
    u1_printf("��һ��");
    EXTIX_Init(); // �ⲿ�жϳ�ʼ��
    usart3_init(38400);
    beep_init();

    LED_Init(); // LED��ʼ��
    //	IIC_Init();                     //��ʼ��IIC�ӿ�
    //	AHT10_Init();                   //��ʼ��AHT10
    //	KEY_Init();                     //������ʼ��
    Adc_Init();  // ADC��ʼ��
    OLED_Init(); // OLED��Ļ��ʼ��
    // IoT_Parameter_Init();                                                    // ��ʼ��������IoTƽ̨MQTT�������Ĳ���
    // CAT1_GIPO_Init();                                                        // ��ʼ��4Gģ��Ŀ���IO
    // STMFLASH_Read(0x8010000, &test, 1);                                      // ��ȡFLASH�����У��ֵ
    adcdata = 2 * (float)(Get_Adc_Average(ADC_Channel_4, 8)) * (3.3 / 4096); // ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ
    while (1)                                                                // ��ѭ��
    {
        
    }
}
/*-------------------------------------------------*/
/*���������жϿ���1״̬����������������            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED1_State(void)
{
    char temp[TBUFF_UNIT]; // ����һ����ʱ������

    memset(temp, 0, TBUFF_UNIT);
    if (LED1_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta1\":0,\"Switch1\":0},\"version\":\"1.0.0\"}"); // ���LED1�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta1\":1,\"Switch1\":1},\"version\":\"1.0.0\"}"); // ���LED1�ǵ͵�ƽ��˵���ǵ���״̬����Ҫ�ظ���״̬��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // ������ݣ�������������
}
/*-------------------------------------------------*/
/*���������жϿ���2״̬����������������            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED2_State(void)
{
    char temp[TBUFF_UNIT]; // ����һ����ʱ������

    memset(temp, 0, TBUFF_UNIT);
    if (LED2_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta2\":0,\"Switch2\":0},\"version\":\"1.0.0\"}"); // ���LED2�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta2\":1,\"Switch2\":1},\"version\":\"1.0.0\"}"); // ���LED2�ǵ͵�ƽ��˵���ǵ���״̬����Ҫ�ظ���״̬��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // ������ݣ�������������
}
/*-------------------------------------------------*/
/*���������жϿ���3״̬����������������            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED3_State(void)
{
    char temp[TBUFF_UNIT]; // ����һ����ʱ������

    memset(temp, 0, TBUFF_UNIT);
    if (LED3_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta3\":0,\"Switch3\":0},\"version\":\"1.0.0\"}"); // ���LED3�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta3\":1,\"Switch3\":1},\"version\":\"1.0.0\"}"); // ���LED3�ǵ͵�ƽ��˵���ǵ���״̬����Ҫ�ظ���״̬��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // ������ݣ�������������
}
/*-------------------------------------------------*/
/*���������жϿ���4״̬����������������            */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED4_State(void)
{
    char temp[TBUFF_UNIT]; // ����һ����ʱ������

    memset(temp, 0, TBUFF_UNIT);
    if (LED4_IN_STA)
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta4\":0,\"Switch4\":0},\"version\":\"1.0.0\"}"); // ���LED4�Ǹߵ�ƽ��˵����Ϩ��״̬����Ҫ�ظ��ر�״̬��������
    else
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta4\":1,\"Switch4\":1},\"version\":\"1.0.0\"}"); // ���LED4�ǵ͵�ƽ��˵���ǵ���״̬����Ҫ�ظ���״̬��������
    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                      // ������ݣ�������������
}
/*-------------------------------------------------*/
/*���������ж�4������״̬����������������          */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void LED_State(void)
{
    char temp[TBUFF_UNIT];       // ����һ����ʱ������
    char sta1, sta2, sta3, sta4; // ����4�������������led��״̬

    if (LED1_IN_STA)
        sta1 = 0; // ����Ǹߵ�ƽ���ǹر�״̬��״ֵ̬=0
    else
        sta1 = 1; // ����ǵ͵�ƽ���Ǵ�״̬��״ֵ̬=1
    if (LED2_IN_STA)
        sta2 = 0; // ����Ǹߵ�ƽ���ǹر�״̬��״ֵ̬=0
    else
        sta2 = 1; // ����ǵ͵�ƽ���Ǵ�״̬��״ֵ̬=1
    if (LED3_IN_STA)
        sta3 = 0; // ����Ǹߵ�ƽ���ǹر�״̬��״ֵ̬=0
    else
        sta3 = 1; // ����ǵ͵�ƽ���Ǵ�״̬��״ֵ̬=1
    if (LED4_IN_STA)
        sta4 = 0; // ����Ǹߵ�ƽ���ǹر�״̬��״ֵ̬=0
    else
        sta4 = 1; // ����ǵ͵�ƽ���Ǵ�״̬��״ֵ̬=1

    memset(temp, 0, TBUFF_UNIT);
    sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"SwitchSta1\":%d,\"SwitchSta2\":%d,\"SwitchSta3\":%d,\"SwitchSta4\":%d,\"Switch1\":%d,\"Switch2\":%d,\"Switch3\":%d,\"Switch4\":%d},\"version\":\"1.0.0\"}", sta1, sta2, sta3, sta4, sta1, sta2, sta3, sta4); // ��Ҫ�ظ�״̬��������

    MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp)); // ������ݣ�������������
}
/*-------------------------------------------------*/
/*����������ʱ�ϴ��������ݵ�����                   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void Data_State(void)
{
    int u = 0;
    char temp[256];                        // ������Ч���ػ�����
    if ((SystemTimer - TEHUTimer) >= 19) { // ȫ��ʱ�� �� ��ʪ�ȼ�ʱʱ�����ٲ�25s������if
        TEHUTimer = SystemTimer;           // �ѵ�ǰ��ȫ��ʱ�䣬��¼����ʪ�ȼ�ʱ����	                           //��ȡ��ʪ��ֵ
        while (u == 0) {
            Usart_SendArray(USART3, DataString, 4);
            result = finaldata1 / 10.0;
            DIST   = result;
            u1_printf("����ֵ��%.2f\r\n", DIST);
            tempdata = dis_status;
            u1_printf("�¶�ֵ��%.2f\r\n", tempdata);
            ampdata = Signal_Strength;
            u1_printf("����ǿ�ȣ�%d\r\n", ampdata);
            u += 1;
            if (ampdata < 0 || ampdata == 65535 || DIST > 550.00 || tempdata == 255 || DIST <= 0) {
                u1_printf("����\r\n");
                u = 0;
            }
        }
        if (TX_flag == 0) {
            memset(temp, 0, 256);                                                                                                                                                                                    // ��ջ�����
            u1_printf("Һλ�ߣ�%.2f\r\n", Dynamic_check_value * Step_size * 1.0 - DIST);                                                                                                                             // �����������
            sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Distance\":%.2f,\"amp\":%d},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0 - DIST, ampdata); // ��������
            MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                       // ������ݵ����ͻ�����
        }
        if (TX_flag == 1) {
            memset(temp, 0, 256);                                                                                                                                                                                      // ��ջ�����
            u1_printf("Һλ��1��%.2f\r\n", Dynamic_check_value * Step_size * 1.0 - DIST);                                                                                                                              // �����������
            sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Distance1\":%3.2f,\"amp\":%d},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0 - DIST, ampdata); // ��������
            MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                         // ������ݵ����ͻ�����
        }
        if (TX_flag == 2) {
            memset(temp, 0, 256);                                                                                                                                                                                      // ��ջ�����
            u1_printf("Һλ��2��%.2f\r\n", Dynamic_check_value * Step_size * 1.0 - DIST);                                                                                                                              // �����������
            sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"Distance2\":%3.2f,\"amp\":%d},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0 - DIST, ampdata); // ��������
            MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                         // ������ݵ����ͻ�����
        }
        TX_flag++;
        if (TX_flag == 3) {
            TX_flag = 0;
        }
        if (!flag) {
            OLED_SHOW();
        }
    }

    if ((SystemTimer - ADCTimer) >= 39) {                                        // ȫ��ʱ�� �� ��ʪ�ȼ�ʱʱ�����ٲ�65s������if
        ADCTimer = SystemTimer;                                                  // �ѵ�ǰ��ȫ��ʱ�䣬��¼����ʪ�ȼ�ʱ����
        memset(temp, 0, 256);                                                    // ��ջ�����
        adcdata = 2 * (float)(Get_Adc_Average(ADC_Channel_4, 8)) * (3.3 / 4096); // ��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ
        u1_printf("valu��%.2f\r\n", Dynamic_check_value * Step_size * 1.0);      // �����������
        u1_printf("ADC��%.2f\r\n", adcdata);
        sprintf(temp, "{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"valu\":%3.2f,\"temp\":%.2f,\"electricity\":%.2f},\"version\":\"1.0.0\"}", Dynamic_check_value * Step_size * 1.0, tempdata, adcdata); // ��������
        MQTT_PublishQs0(P_TOPIC_NAME, temp, strlen(temp));                                                                                                                                                                               // ������ݵ����ͻ�����
    }
}
