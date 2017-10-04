//
// Created by Alex on 15.08.2017.
//

#ifndef STMSAMPLE_GPIO_DRIVER_H
#define STMSAMPLE_GPIO_DRIVER_H

#ifdef STM32F429_439xx
    #define LED_PORT            GPIOG
    #define LD_ORANGE_Pin      GPIO_Pin_7
    #define LD_GREEN_Pin       GPIO_Pin_6
    #define LD_RED_Pin         GPIO_Pin_10
    #define LD_BLUE_Pin        GPIO_Pin_12
#else
    #define LED_PORT            GPIOD
    #define LD_ORANGE_Pin      GPIO_Pin_13
    #define LD_GREEN_Pin       GPIO_Pin_12
    #define LD_RED_Pin         GPIO_Pin_14
    #define LD_BLUE_Pin        GPIO_Pin_15
#endif

typedef enum {
//    LED for STM32F407 Discovery
    LD3_ORANGE = 0, LD4_GREEN = 1, LD5_RED = 2, LD6_BLUE = 3}LDx_Color;

void initDiscLed(LDx_Color led);
void testDscLedOn();
void initGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif //STMSAMPLE_GPIO_DRIVER_H
