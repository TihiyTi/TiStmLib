
//#include "stm32f4xx_conf.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"
#include "dac_driver.h"
#include "tim_driver.h"

//Arrays for using with DMA
uint16_t *arrayDac1;
uint16_t sizeDac1;
uint16_t *arrayDac2;
uint16_t sizeDac2;

void configDAC(uint32_t dac_channel);
void configDMAforDAC(uint32_t dac_channel, TIM_TypeDef * timer);

//todo need test in hardware
void initDAC(uint32_t dac_channel){
    GPIO_InitTypeDef GPIO_DAC_Structure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    /* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
    if(dac_channel == DAC_Channel_1){
        GPIO_DAC_Structure.GPIO_Pin = GPIO_Pin_4;
    }else{
        GPIO_DAC_Structure.GPIO_Pin = GPIO_Pin_5;
    }
    GPIO_DAC_Structure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_DAC_Structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_DAC_Structure);

    DAC_InitTypeDef DAC_InitStructure;
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
    DAC_Init(dac_channel, &DAC_InitStructure);

    /* Enable DAC Channel1 */
    DAC_Cmd(DAC_Channel_2, ENABLE);
}

void initDACwithDMA(uint32_t dac_channel, TIM_TypeDef * timer, uint16_t *array, uint16_t size){
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    arrayDac1 = array;
    arrayDac2 = array;
    sizeDac1 = size;
    sizeDac2 = size;

    GPIO_InitTypeDef GPIO_DAC_Structure;
    if(dac_channel == DAC_Channel_1){
        GPIO_DAC_Structure.GPIO_Pin = GPIO_Pin_4;
    }else{
        GPIO_DAC_Structure.GPIO_Pin = GPIO_Pin_5;
    }
    GPIO_DAC_Structure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_DAC_Structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_DAC_Structure);

    DAC_InitTypeDef DAC_InitStructure;
    switch ((uint32_t)timer){
        case (uint32_t)TIM2:
            DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
            break;
        case (uint32_t)TIM4:
            DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;
            break;
        case (uint32_t)TIM5:
            DAC_InitStructure.DAC_Trigger = DAC_Trigger_T5_TRGO;
            break;
        case (uint32_t)TIM6:
            DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
            break;
        case (uint32_t)TIM7:
            DAC_InitStructure.DAC_Trigger = DAC_Trigger_T7_TRGO;
            break;
        case (uint32_t)TIM8:
            DAC_InitStructure.DAC_Trigger = DAC_Trigger_T8_TRGO;
            break;
        default:
            break;
    }
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(dac_channel, &DAC_InitStructure);



    DMA_InitTypeDef DMA_InitStructure;
    if(dac_channel == DAC_Channel_1){
        DMA_DeInit(DMA1_Stream5);
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)arrayDac1;
        DMA_InitStructure.DMA_BufferSize = sizeDac1;
//        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&form1;
//        DMA_InitStructure.DMA_BufferSize = NUM_OF_SIGNAL_POINT;
    } else{
        DMA_DeInit(DMA1_Stream6);
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)arrayDac2;
        DMA_InitStructure.DMA_BufferSize = sizeDac2;
//        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&form1;
//        DMA_InitStructure.DMA_BufferSize = NUM_OF_SIGNAL_POINT;
    }
    DMA_InitStructure.DMA_Channel = DMA_Channel_7;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_ADDRESS;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

    if(dac_channel == DAC_Channel_1){
        DMA_Init(DMA1_Stream5, &DMA_InitStructure);
    } else{
        DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    }

    /* Enable DAC Channel1 */
    DAC_Cmd(dac_channel, ENABLE);

    DAC_DMACmd(dac_channel, ENABLE);

//    TIM5_Config(FREQ_1Hz);
//    timerConfigForDAC_DMA(timer);
}

void timerConfigForDAC_DMA(TIM_TypeDef *timer, uint32_t period, uint32_t prescaler, uint32_t division){
    initTimer(timer, period, prescaler, division);
    TIM_Cmd(timer, ENABLE);
    TIM_SelectOutputTrigger(timer, TIM_TRGOSource_Update);
}
