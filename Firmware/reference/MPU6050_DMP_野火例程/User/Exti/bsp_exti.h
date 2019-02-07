#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


/*�ⲿ�ж�EXIT��غ궨��*/
#define             EXTI_GPIO_CLK                        (RCC_APB2Periph_GPIOB  |RCC_APB2Periph_AFIO)     
#define             EXTI_GPIO_PORT                       GPIOB   
#define             EXTI_GPIO_PIN                        GPIO_Pin_5
#define             EXTI_SOURCE_PORT                     GPIO_PortSourceGPIOB
#define             EXTI_SOURCE_PIN                      GPIO_PinSource5
#define             EXTI_LINE                            EXTI_Line5
#define             EXTI_IRQ                             EXTI9_5_IRQn
#define             EXTI_INT_FUNCTION                    EXTI9_5_IRQHandler


#define ENABLE_INV_INTERRUPTS  EnableInvInterrupt()
#define DISABLE_INV_INTERRUPTS DisableInvInterrupt()


void EXTI_Pxy_Config(void);
void EnableInvInterrupt(void);
void DisableInvInterrupt(void);


#endif /* __EXTI_H */

