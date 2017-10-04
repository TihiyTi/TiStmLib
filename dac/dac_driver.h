//
// Created by Aleksey on 12.08.2017.
//

#ifndef STMSAMPLE_DAC_DRIVER_H
#define STMSAMPLE_DAC_DRIVER_H

#define DAC_DHR12R2_ADDRESS    0x40007414

typedef enum {DMA_DISABLE = 0, DMA_ENABLE = 1}DMA_DAC_State;

#define FREQ_1Hz (uint8_t)1
#define FREQ_2Hz (uint8_t)2
#define FREQ_4Hz (uint8_t)3
#define FREQ_12Hz (uint8_t)4


void initDAC(uint32_t dac_channel);
void initDACwithDMA(uint32_t dac_channel, TIM_TypeDef * timer, uint16_t *array, uint16_t size);
void timerConfigForDAC_DMA(TIM_TypeDef *timer, uint32_t period, uint32_t prescaler, uint32_t division);

void magnitInit();
void DAC_DMA_Pin_Clock();
void DAC_Ch2_Pulse();
void TIM5_Config(uint8_t freq);

#endif //STMSAMPLE_DAC_DRIVER_H
