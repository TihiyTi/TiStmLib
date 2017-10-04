//
// Created by Aleksey on 12.08.2017.
//

#ifndef STMSAMPLE_TIM_DRIVER_H
#define STMSAMPLE_TIM_DRIVER_H

//#include <stm32f4xx.h>

void initTimerWithIRQ(TIM_TypeDef * timer, uint32_t microSeconds);
void startTimer(TIM_TypeDef * timer);

void initTimer(TIM_TypeDef * timer, uint32_t period, uint32_t prescaler, uint32_t division);

#endif //STMSAMPLE_TIM_DRIVER_H
