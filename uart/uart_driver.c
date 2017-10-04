//
// Created by Alex on 28.07.2017.
//

#include <stm32f4xx.h>
#include <misc.h>
#include "stm32f4xx_conf.h"
#include "uart_driver.h"
#include "stm32f4xx_usart.h"

uint8_t testArray[] = {1,2,3,4,5,6,7,8,9,10};
int uart_wait = 0;
int dma_wait = 0;

void configUSART(USART_TypeDef* UART, uint32_t speed);
void configDMAforUSART(USART_TypeDef* UART);
void configIRQforUSART(USART_TypeDef* UART);
uint8_t sendUARTByDMAComplete(USART_TypeDef* UART);

void initUSART(USART_TypeDef* UART, uint32_t speed, DMA_Trancieve_State dma_state, IRQ_Recieve_State irq_state){
    configUSART(UART, speed);
    if(dma_state == DMA_ENABLE){
        configDMAforUSART(UART);
    }
    if(irq_state == IRQ_ENABLE){
        configIRQforUSART(UART);
    }
}

void configUSART(USART_TypeDef* UART, uint32_t speed){
    GPIO_InitTypeDef  GPIO;
    USART_InitTypeDef USART;

    GPIO_StructInit(&GPIO);

    GPIO.GPIO_OType = GPIO_OType_PP;
    GPIO.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO.GPIO_Mode = GPIO_Mode_AF;
    GPIO.GPIO_Speed = GPIO_Speed_50MHz;

    USART.USART_BaudRate = speed;
    USART.USART_WordLength = USART_WordLength_8b;
    USART.USART_StopBits = USART_StopBits_1;
    USART.USART_Parity = USART_Parity_No;
    USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    switch ((uint32_t)UART){
        case (uint32_t)USART1:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
            GPIO.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
            GPIO_Init(GPIOA, &GPIO);
            break;
        case (uint32_t)USART2:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
            GPIO.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
            GPIO_Init(GPIOA, &GPIO);
            break;
        case (uint32_t)USART3:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
            GPIO.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
            GPIO_Init(GPIOB, &GPIO);
            break;
        case (uint32_t)UART4:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
            GPIO.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
            GPIO_Init(GPIOC, &GPIO);
        default:break;
    }

    USART_Init(UART, &USART);
    USART_Cmd(UART, ENABLE);
}
void configDMAforUSART(USART_TypeDef* UART){
    DMA_InitTypeDef DMA_InitStruct_USART;
    DMA_StructInit(&DMA_InitStruct_USART);
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
            DMA_InitStruct_USART.DMA_Channel = DMA_Channel_4;
            DMA_InitStruct_USART.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
            break;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            break;
        case (uint32_t) USART3:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
            DMA_InitStruct_USART.DMA_Channel = DMA_Channel_4;
            DMA_InitStruct_USART.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
            break;
        case (uint32_t) UART4:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
            DMA_InitStruct_USART.DMA_Channel = DMA_Channel_4;
            DMA_InitStruct_USART.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
        default:
            break;
    }

    DMA_InitStruct_USART.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct_USART.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct_USART.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct_USART.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct_USART.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct_USART.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct_USART.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct_USART.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct_USART.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStruct_USART.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct_USART.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
}
void configIRQforUSART(USART_TypeDef* UART){
    NVIC_InitTypeDef nvic;
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            NVIC_EnableIRQ(USART1_IRQn);
            nvic.NVIC_IRQChannel = USART1_IRQn;
            break;
        case (uint32_t) USART2:
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
            NVIC_EnableIRQ(USART2_IRQn);
            nvic.NVIC_IRQChannel = USART2_IRQn;
            break;
        case (uint32_t) USART3:
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
            NVIC_EnableIRQ(USART3_IRQn);
            nvic.NVIC_IRQChannel = USART3_IRQn;
            break;
        case (uint32_t) UART4:
            USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
            NVIC_EnableIRQ(UART4_IRQn);
            nvic.NVIC_IRQChannel = UART4_IRQn;
            break;
        default:
            break;
    }
    nvic.NVIC_IRQChannelCmd = ENABLE;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Init(&nvic);
}

void sendUARTByDMA(USART_TypeDef* UART, uint8_t *array, uint8_t size){
    while(USART_GetFlagStatus(UART, USART_FLAG_TC) == RESET){
    }
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            //todo не реализовано для USART1
            break;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            break;
        case (uint32_t) USART3:
            DMA_Cmd(DMA1_Stream3, DISABLE);
            while(DMA_GetCmdStatus(DMA1_Stream3) == ENABLE){}
            DMA_DeInit(DMA1_Stream3);
            break;
        case (uint32_t) UART4:
            DMA_Cmd(DMA1_Stream4, DISABLE);
            while(DMA_GetCmdStatus(DMA1_Stream4) == ENABLE){}
            DMA_DeInit(DMA1_Stream4);
        default:
            break;
    }

    DMA_InitTypeDef DMA_InitStruct_USART;
    //todo проверить для всех ли UART channel == 4
    DMA_InitStruct_USART.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct_USART.DMA_PeripheralBaseAddr = (uint32_t)&(UART->DR);
    DMA_InitStruct_USART.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStruct_USART.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct_USART.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct_USART.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct_USART.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct_USART.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct_USART.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct_USART.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct_USART.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStruct_USART.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct_USART.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStruct_USART.DMA_Memory0BaseAddr =  (uint32_t)array;
    DMA_InitStruct_USART.DMA_BufferSize = size;

    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            //todo не реализовано для USART1
            break;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            break;
        case (uint32_t) USART3:
            DMA_Init(DMA1_Stream3, &DMA_InitStruct_USART);
            USART_DMACmd(UART, USART_DMAReq_Tx, ENABLE);
            DMA_Cmd(DMA1_Stream3, ENABLE);
            break;
        case (uint32_t) UART4:
            DMA_Init(DMA1_Stream4, &DMA_InitStruct_USART);
            USART_DMACmd(UART, USART_DMAReq_Tx, ENABLE);
            DMA_Cmd(DMA1_Stream4, ENABLE);
            break;
        default:
            break;
    }
    // Флаг TC очищается автоматически при записи регистра USART DR
    // НО если DMA записывает данные флаг TC не сбрасывается
    USART_ClearFlag(UART, USART_FLAG_TC);
}
uint8_t sendUARTByDMAComplete(USART_TypeDef* UART){
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            //todo не реализовано для USART1
            return 0;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            return 0;
        case (uint32_t) USART3:
            return (uint8_t)(USART_GetFlagStatus(UART, USART_FLAG_TC) == RESET)|(DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)==RESET);
        case (uint32_t) UART4:
            return (uint8_t)(USART_GetFlagStatus(UART, USART_FLAG_TC) == RESET)|(DMA_GetFlagStatus(DMA1_Stream4, DMA_FLAG_TCIF4)==RESET);
        default:
            return 0;
    }

}

void testUART(USART_TypeDef* UART){
    USART_SendData(UART, 0xAA);

    sendUARTByDMA(UART, testArray, 10);
    sendUARTByDMA(UART, testArray, 10);

    while (USART_GetFlagStatus(UART, USART_FLAG_TC) == RESET){
        uart_wait ++;
    }
    USART_SendData(UART, 0xAA);

}