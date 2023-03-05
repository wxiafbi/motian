/*-------------------------------------------------*/
/*            ��γ����STM32ϵ�п�����              */
/*-------------------------------------------------*/
/*                                                 */
/*          ʵ�ָ����жϷ�������Դ�ļ�           */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"    //������Ҫ��ͷ�ļ�
#include "stm32f10x_it.h" //������Ҫ��ͷ�ļ�
#include "main.h"         //������Ҫ��ͷ�ļ�
#include "delay.h"        //������Ҫ��ͷ�ļ�
#include "usart1.h"       //������Ҫ��ͷ�ļ�
#include "usart3.h"       //������Ҫ��ͷ�ļ�
#include "timer4.h"       //������Ҫ��ͷ�ļ�
#include "cat1.h"         //������Ҫ��ͷ�ļ�
#include "timer3.h"       //������Ҫ��ͷ�ļ�
#include "led.h"          //������Ҫ��ͷ�ļ�
#include "mqtt.h"         //������Ҫ��ͷ�ļ�
#include "usart9.h"       //������Ҫ��ͷ�ļ�
#include "exti.h"
#include "stmflash.h"
#include "oled.h"
#include "beep.h"

char fina_data1[5];
char fina_data2[5];
int finaldata1;
int finaldata2;

/*-------------------------------------------------*/
/*������������3�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART2_IRQHandler(void)
{
    if ((USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)) { // ���USART_IT_RXNE��־��λ����ʾ�����ݵ��� ����if��֧
        if (USART2->DR) {                                      // ����ָ������״̬ʱ������ֵ�ű��浽������
            Usart2_RxBuff[Usart2_RxCounter] = USART2->DR;      // ���浽������
            Usart2_RxCounter++;                                // ÿ����1���ֽڵ����ݣ�Usart3_RxCounter��1����ʾ���յ���������+1
        }
    }
    if ((USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)) {                         // ��USART_IT_IDLE��־��λ����ʾ�����ж� ����if��֧
        Usart2_RxCounter = USART2->SR;                                                 // ���USART_IT_IDLE��־  ����1
        Usart2_RxCounter = USART2->DR;                                                 // ���USART_IT_IDLE��־  ����2
        DMA_Cmd(DMA1_Channel6, DISABLE);                                               // �ر�DMA
        Usart2_RxCounter = USART2_RXBUFF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6); // ��ȡ���ڽ��յ�������
        memcpy(&MQTT_RxDataInPtr[2], Usart2_RxBuff, Usart2_RxCounter);                 // �������ݵ����ջ�����
        MQTT_RxDataInPtr[0] = Usart2_RxCounter / 256;                                  // ��¼���ݳ��ȸ��ֽ�
        MQTT_RxDataInPtr[1] = Usart2_RxCounter % 256;                                  // ��¼���ݳ��ȵ��ֽ�
        MQTT_RxDataInPtr += RBUFF_UNIT;                                                // ָ������
        if (MQTT_RxDataInPtr == MQTT_RxDataEndPtr)                                     // ���ָ�뵽������β����
            MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                                      // ָ���λ����������ͷ
        Usart2_RxCounter     = 0;                                                      // ����3������������������
        DMA1_Channel6->CNDTR = USART2_RXBUFF_SIZE;                                     // �������ý������ݸ���
        DMA_Cmd(DMA1_Channel6, ENABLE);                                                // ����DMA
        TIM_SetCounter(TIM3, 0);                                                       // �����ʱ��3 ����ֵ
    }
}
/*-------------------------------------------------*/
/*������������2�����жϺ���                        */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void USART3_IRQHandler(void)
{
    static u8 seri_count = 0;
    char check_flag, end_flag, j, k = 0;
    u8 x;

    static u8 uflag = 0;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //?????
    {

        if (USART_ReceiveData(USART3) == 0x44) {
            uflag = 1;
        }
        if (uflag) {
            USART3_RX_BUF[seri_count++] = USART_ReceiveData(USART3);
            if (USART_ReceiveData(USART3) == 0x0A) {
                beep_on_200ms();
                for (size_t i = 0; i < seri_count; i++) {
                    /* code */
                    u1_printf("%x ", USART3_RX_BUF[i]);
                    switch (USART3_RX_BUF[i]) {
                        case 0x6D:
                            check_flag = i;
                            u1_printf("0x6D�ڵ�%dλ", check_flag);
                            break;
                        case 0x23:
                            end_flag = i;
                            u1_printf("0x0D�ڵ�%dλ", end_flag);
                            break;
                        default:
                            break;
                    }
                }
                for (size_t i = 2; i < check_flag; i++) {
                    if (USART3_RX_BUF[i] == 0x2E) {
                        continue;
                    }
                    fina_data1[j++] = USART3_RX_BUF[i];
                }
                for (size_t a = check_flag + 1; a < end_flag; a++) {
                    if (USART3_RX_BUF[a] == 0x2C) {
                        continue;
                    }
                    fina_data2[k++] = USART3_RX_BUF[a];
                }
                sscanf(fina_data1, "%d", &finaldata1); // �ַ���תint
                sscanf(fina_data2, "%d", &finaldata2); // �ַ���תint
                Delay_Ms(10);
                u1_printf("����ֵ=%dmm,�ع���=%d\r\n", finaldata1, finaldata2); // print�ô���2������1�����ͼ���ģ��ͨѶ
                DIST = finaldata1 * 1.0 / 10;
                for (x = 0; x < j; x++) {
                    fina_data1[x] = 0;
                    fina_data2[x] = 0;
                }
                uflag      = 0;
                seri_count = 0;
                j          = 0;
                k          = 0;
            }
        }
    }
}
/*-------------------------------------------------*/
/*��������DMA1ͨ��2�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DMA1_Channel7_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC7) != RESET) { // ���DMA1_IT_TC2��λ����ʾDMA1ͨ��7��������жϣ�����if
        DMA_ClearITPendingBit(DMA1_IT_TC7);      // ���DMA1ͨ��2 ��������жϱ�־
        DMA_Cmd(DMA1_Channel7, DISABLE);         // �ر�DMA1ͨ��2
        DMA_flag = 0;                            // ��־λ��� ����״̬
    }
}
/*-------------------------------------------------*/
/*����������ʱ��3�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) { // ���TIM_IT_Update��λ����ʾTIM3����жϣ�����if
        switch (Ping_flag) {                             // �ж�Ping_flag��״̬
            case 0:                                      // ���Ping_flag����0����ʾ����״̬������Ping����
                MQTT_PingREQ();                          // ���Ping���ĵ����ͻ�����
                break;
            case 1:               // ���Ping_flag����1��˵����һ�η��͵���ping���ģ�û���յ��������ظ�������1û�б����Ϊ0�������������쳣������Ҫ��������pingģʽ
                TIM3_ENABLE_2S(); // ���ǽ���ʱ��6����Ϊ2s��ʱ,���ٷ���Ping����
                MQTT_PingREQ();   // ���Ping���ĵ����ͻ�����
                break;
            case 2:             // ���Ping_flag����2��˵����û���յ��������ظ�
            case 3:             // ���Ping_flag����3��˵����û���յ��������ظ�
            case 4:             // ���Ping_flag����4��˵����û���յ��������ظ�
                MQTT_PingREQ(); // ���Ping���ĵ����ͻ�����
                break;
            case 5:                     // ���Ping_flag����5��˵�����Ƿ����˶��ping�����޻ظ���Ӧ�������������⣬������������
                Connect_flag = 0;       // ����״̬��0����ʾ�Ͽ���û���Ϸ�����
                TIM_Cmd(TIM3, DISABLE); // ��TIM3
                break;
        }
        Ping_flag++;                                // Ping_flag����1����ʾ�ַ�����һ��ping���ڴ��������Ļظ�
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ���TIM3����жϱ�־
    }
}
/*-------------------------------------------------*/
/*�������� EXTI9_5�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void EXTI9_5_IRQHandler(void)
{
    u8 laser_on[5]      = {'i', 'L', 'D', ':', '1'};
    u8 laser_off[5]     = {'i', 'L', 'D', ':', '0'};
    u8 laser_measure[3] = {'i', 'S', 'M'};

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) {
        Delay_Ms(10); // ����
        beep_on_100ms();
        STMFLASH_Read(0x8010000, &Dynamic_check_value, 1);
        Dynamic_check_value -= 1;
        if (Dynamic_check_value > Maximum_check_value / Step_size)
            Dynamic_check_value = Minimum_check_value / Step_size;
        if (Dynamic_check_value < Minimum_check_value / Step_size)
            Dynamic_check_value = Maximum_check_value / Step_size;
        STMFLASH_Write(0x8010000, &Dynamic_check_value, 1);
        // TEST=Dynamic_check_value*Step_size*1.0;
        Usart_SendArray(USART3, laser_on, 5);
        OLED_SHOW();
        u1_printf("�����ⲿ�ж�1\r\n");
    }
    EXTI_ClearITPendingBit(EXTI_Line5); // �ж���

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0) {
        Delay_Ms(10); // ����
        beep_on_100ms();
        STMFLASH_Read(0x8010000, &Dynamic_check_value, 1);
        Dynamic_check_value += 1;
        if (Dynamic_check_value > Maximum_check_value / Step_size)
            Dynamic_check_value = Minimum_check_value / Step_size;
        if (Dynamic_check_value < Minimum_check_value / Step_size)
            Dynamic_check_value = Maximum_check_value / Step_size;
        STMFLASH_Write(0x8010000, &Dynamic_check_value, 1);
        // TEST=Dynamic_check_value*Step_size*1.0;
        Usart_SendArray(USART3, laser_off, 5);
        OLED_SHOW();
        u1_printf("�����ⲿ�ж�2\r\n");
    }
    EXTI_ClearITPendingBit(EXTI_Line6); // �ж���

    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0) {
        Delay_Ms(10); // ����
        beep_on_100ms();
        // flag = !flag;
        Usart_SendArray(USART3, laser_measure, 3);
        // if (flag) {
        //     OLED_Display_Off(); // Turn_off_OLED();
        // } else {
        //     OLED_Display_On(); // OLED_Init();
        // }
        // if (!flag) {
        //     OLED_SHOW();
        // }
        // if (!flag && result == 0) {
        //     OLED_Clear();
        //     OLED_ShowCHinese(0, 3, 10);
        //     OLED_ShowCHinese(18, 3, 11);
        //     OLED_ShowCHinese(36, 3, 12);
        //     OLED_ShowCHinese(54, 3, 13);
        //     OLED_ShowCHinese(72, 3, 14);
        //     OLED_ShowCHinese(90, 3, 15);
        //     OLED_ShowString(108, 3, "!");
        // }
        OLED_SHOW();
        u1_printf("�����ⲿ�ж�3\r\n");
    }

    EXTI_ClearITPendingBit(EXTI_Line7); // �ж���
}
/*-------------------------------------------------*/
/*����������ʱ��2�жϷ�����                      */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) { // ���TIM_IT_Update��λ����ʾTIM2����жϣ�����if
        SystemTimer++;                                   // ȫ�ּ�ʱ�ı���+1,��ʾ��ȥ��1����
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);      // ���TIM2����жϱ�־
    }
}
/*-------------------------------------------------*/
/*�����������������жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void NMI_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������Ӳ������������жϴ�����             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void HardFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*���������ڴ�����жϴ�����                     */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void MemManage_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������Ԥȡָʧ�ܣ��洢������ʧ���жϴ�����   */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void BusFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������δ�����ָ���Ƿ�״̬������           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void UsageFault_Handler(void)
{
}

/*-------------------------------------------------*/
/*�����������жϣ�SWI ָ����õĴ�����           */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SVC_Handler(void)
{
}

/*-------------------------------------------------*/
/*�����������Լ����������                       */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void DebugMon_Handler(void)
{
}

/*-------------------------------------------------*/
/*���������ɹ����ϵͳ��������                 */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void PendSV_Handler(void)
{
}

/*-------------------------------------------------*/
/*��������SysTicϵͳ��શ�ʱ��������             */
/*��  ������                                       */
/*����ֵ����                                       */
/*-------------------------------------------------*/
void SysTick_Handler(void)
{
}
