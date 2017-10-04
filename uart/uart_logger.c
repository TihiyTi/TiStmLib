#include "uart_logger.h"
#include "stm32f4xx.h"
//#include "stm32f4xx_dma.h"
//#include "misc.h"

//#include "uart_driver.h"
//#include ""
//todo возможно можно вынести в локальную переменную
DMA_InitTypeDef DMA_InitStruct_USART;
USART_TypeDef* logUart;

#if defined(LOG_BUF)
    LoggerQueue log;
#endif

void initLoggerQueue();

void initLogger(){
    //todo вынести настройку UART в отдельный файл

    GPIO_InitTypeDef  GPIO;
    USART_InitTypeDef USART;

    GPIO_StructInit(&GPIO);

    GPIO.GPIO_OType = GPIO_OType_PP;
    GPIO.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO.GPIO_Mode = GPIO_Mode_AF;
    GPIO.GPIO_Speed = GPIO_Speed_50MHz;

    USART.USART_BaudRate = LOG_SPEED;
    USART.USART_WordLength = USART_WordLength_8b;
    USART.USART_StopBits = USART_StopBits_1;
    USART.USART_Parity = USART_Parity_No;
    USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

#if defined(LOG_USART1)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO);
    logUart = USART1;
#elif defined(LOG_USART2)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    GPIO.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO);
    logUart = USART2;
#elif defined(LOG_USART3)
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
    GPIO.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO);
    logUart = USART3;
#endif

    USART_Init(logUart, &USART);
    USART_Cmd(logUart, ENABLE);

//#if  defined(LOG_DMA)
//        NVIC_InitTypeDef nvic;
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    #if defined(LOG_USART1)
//        nvic.NVIC_IRQChannel = USART1_IRQn;
//    #elif defined(LOG_USART2)
//        nvic.NVIC_IRQChannel = USART2_IRQn;
//    #elif defined(LOG_USART3)
//        nvic.NVIC_IRQChannel = USART3_IRQn;
//    #endif
//        nvic.NVIC_IRQChannelCmd = ENABLE;
//        nvic.NVIC_IRQChannelPreemptionPriority = 0;
//        nvic.NVIC_IRQChannelSubPriority = 0;
//        NVIC_Init(&nvic);
//#endif

#if defined(LOG_DMA)
        DMA_StructInit(&DMA_InitStruct_USART);
    #if defined(LOG_USART1)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
        DMA_InitStruct_USART.DMA_Channel = DMA_Channel_4;
        DMA_InitStruct_USART.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    #elif defined(LOG_USART2)
        //todo не реализовано для USART2
    #elif defined(LOG_USART3)
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        DMA_InitStruct_USART.DMA_Channel = DMA_Channel_4;
        DMA_InitStruct_USART.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
    #endif

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
#endif

#if defined(LOG_BUF)
    initLoggerQueue(&log);
#endif

}


//todo realize only for usart 3
void sendStringByDMA(char *str, uint8_t size){
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){
    }
    DMA_Cmd(DMA1_Stream3, DISABLE);
    while(DMA_GetCmdStatus(DMA1_Stream3) == ENABLE){}
    DMA_DeInit(DMA1_Stream3);

    DMA_InitStruct_USART.DMA_Memory0BaseAddr =  (uint32_t)str;
    DMA_InitStruct_USART.DMA_BufferSize = size;
    DMA_Init(DMA1_Stream3, &DMA_InitStruct_USART);
    USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
    DMA_Cmd(DMA1_Stream3, ENABLE);
}
void sendStringByDMAandWait(char *str, uint8_t size){
    sendStringByDMA(str, size);
    while(!DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)){}
}
void sendStringByDMAAfterWait(char *str, uint8_t size){
    if(DMA_GetCmdStatus(DMA1_Stream3) == ENABLE){
        while(!DMA_GetFlagStatus(DMA1_Stream3, DMA_FLAG_TCIF3)){}
    }
    sendStringByDMA(str, size);
}



//DEPRECATED
void initUART(USART_TypeDef* UART, uint32_t speed){
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
            RCC_AHB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            //todo unsupported yet
        default:break;
    }

    USART_Init(UART, &USART);
    USART_Cmd(UART, ENABLE);
    logUart = UART;
}
void logByteUnsafe(char ch){
    USART_SendData(logUart, (uint16_t) ch);
}

//todo may be can extract to work with queue to external library
void addToLoggerQueue(char *pointer, uint8_t size){
    if(log.size < LOG_BUF_SIZE){
        log.size++;
        log.last = (uint8_t) ((log.first + log.size - 1) % LOG_BUF_SIZE);
        log.pointers[log.last] = pointer;
        log.sizes[log.last] = size;
    }else{
        //todo increment error counter
    }
}
void takeFromLoggerQueue(Pair *pair){
    pair->pointer = log.pointers[log.first];
    pair->size = log.sizes[log.first];
    if(log.first == LOG_BUF_SIZE - 1){
        log.first = 0;
    }else{
        log.first ++;
    }
    log.size --;
}
uint8_t isContainsElementLoggerQueue(){
    if(log.size > 0){
        return 1;
    }
    return 0;
}
void initLoggerQueue(){
    log.first = 0;
    log.last = 0;
    log.size = 0;
}

#if defined(LOG_USART1)

#endif