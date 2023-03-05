#include "stm32f10x.h"

#define BEEP_BUS RCC_APB2Periph_GPIOB
#define BEEP GPIOB
#define BEEP_GPIO GPIO_Pin_12

void beep_init(void);
void beep_ctrl(u8 on);
void beep_on_500ms(void);
void beep_on_200ms(void);
void beep_on_100ms(void);

