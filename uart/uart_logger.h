//
// Created by Alex on 25.07.2017.
//

#ifndef UART_LOGGER_H
#define UART_LOGGER_H
#include "stm32f4xx.h"

#if !defined(LOG_BUF_SIZE)
    #define LOG_BUF_SIZE 10
#endif

void initLogger();

typedef struct {
    char* pointers[LOG_BUF_SIZE];
    uint8_t sizes[LOG_BUF_SIZE];
    uint8_t first;
    uint8_t last;
    uint8_t size;
}LoggerQueue;

typedef struct {
    char* pointer;
    uint8_t size;
}Pair;


//void sendStringByDMA(char *str, uint8_t size);
void sendStringByDMAandWait(char *str, uint8_t size);
void sendStringByDMAAfterWait(char *str, uint8_t size);

//void sendStringByDMA();
void initUART(USART_TypeDef* UART, uint32_t speed);
void logByteUnsafe(char ch);


void addToLoggerQueue(char *pointer, uint8_t size);
void takeFromLoggerQueue(Pair *pair);
uint8_t isContainsElementLoggerQueue();

#if !defined(LOG_USART1) && !defined(LOG_USART2) && !defined(LOG_USART3)
    #error "Don't choose usart for logger"
#endif


#endif //UART_DRIVER_H
