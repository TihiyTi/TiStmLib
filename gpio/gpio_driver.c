//
// Created by Alex on 15.08.2017.
//

#include <stm32f4xx.h>
#include "gpio_driver.h"
/*
 * LD3 -  PD13
 * LD4 -  PD12
 * LD5 -  PD14
 * LD6 -  PD15
 */
void initDiscLed(LDx_Color led){
    switch (led){
        case LD3_ORANGE:
            initGPIO(LED_PORT, LD_ORANGE_Pin);
            break;
        case LD4_GREEN:
            initGPIO(LED_PORT, LD_GREEN_Pin);
            break;
        case LD5_RED:
            initGPIO(LED_PORT, LD_RED_Pin);
            break;
        case LD6_BLUE:
            initGPIO(LED_PORT, LD_BLUE_Pin);
            break;
        default:
            break;
    }
}
void testDscLedOn(){
#ifdef STM32F429_439xx
    GPIO_ResetBits(LED_PORT, LD_ORANGE_Pin | LD_GREEN_Pin| LD_RED_Pin| LD_BLUE_Pin);
#else
    GPIO_SetBits(LED_PORT, LD_ORANGE_Pin | LD_GREEN_Pin| LD_RED_Pin| LD_BLUE_Pin);
#endif
}
void initGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
    switch ((uint32_t)GPIOx){
        case (uint32_t)GPIOA:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            break;
        case (uint32_t)GPIOB:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            break;
        case (uint32_t)GPIOC:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
            break;
        case (uint32_t)GPIOD:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
            break;
        case (uint32_t)GPIOE:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
            break;
        case (uint32_t)GPIOF:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
            break;
        case (uint32_t)GPIOG:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
            break;
        default:
            break;
    }
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
};
