#include "stm32f4xx.h"
#include "tim_driver.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

void TIMClockTimer(TIM_TypeDef * timer);
void TIMIrqEnable(TIM_TypeDef * timer);

void initTimerWithIRQ(TIM_TypeDef * timer, uint32_t milliSeconds){
    TIMClockTimer(timer);

    TIM_TimeBaseInitTypeDef TIM_Structure;
    TIM_TimeBaseStructInit(&TIM_Structure);

    TIM_Structure.TIM_ClockDivision = TIM_CKD_DIV2;
    TIM_Structure.TIM_Prescaler = 36000 - 1;
    TIM_Structure.TIM_Period = milliSeconds;
    TIM_TimeBaseInit(timer, &TIM_Structure);

    TIM_ITConfig(timer, TIM_IT_Update, ENABLE);

    TIMIrqEnable(timer);
}
void startTimer(TIM_TypeDef * timer){
    TIM_Cmd(timer, ENABLE);
}

void TIMClockTimer(TIM_TypeDef * timer);
void TIMIrqEnable(TIM_TypeDef * timer);

void initTimer(TIM_TypeDef * timer, uint32_t period, uint32_t prescaler, uint32_t division){
    TIMClockTimer(timer);

    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) prescaler;
    TIM_TimeBaseStructure.TIM_ClockDivision = (uint16_t) division;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(timer, &TIM_TimeBaseStructure);
}

void TIMClockTimer(TIM_TypeDef * timer){
    switch ((uint32_t)timer){
        case (uint32_t)TIM2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
            break;
        case (uint32_t)TIM3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
            break;
        case (uint32_t)TIM4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
            break;
        case (uint32_t)TIM5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
            break;
        default:
            break;
    }
}
void TIMIrqEnable(TIM_TypeDef * timer){
    switch ((uint32_t)timer){
        case (uint32_t)TIM2:
            NVIC_EnableIRQ(TIM2_IRQn);
            break;
        case (uint32_t)TIM3:
            NVIC_EnableIRQ(TIM3_IRQn);
            break;
        case (uint32_t)TIM4:
            NVIC_EnableIRQ(TIM4_IRQn);
            break;
        case (uint32_t)TIM5:
            NVIC_EnableIRQ(TIM5_IRQn);
            break;
        default:
            break;
    }
}