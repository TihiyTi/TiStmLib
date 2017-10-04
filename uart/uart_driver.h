//
// Created by Alex on 28.07.2017.
//

#ifndef STMSAMPLE_UART_DRIVER_H
#define STMSAMPLE_UART_DRIVER_H

typedef enum {DMA_DISABLE = 0, DMA_ENABLE = 1}DMA_Trancieve_State;
typedef enum {IRQ_DISABLE = 0, IRQ_ENABLE = 1}IRQ_Recieve_State;

void initUSART(USART_TypeDef* UART, uint32_t speed, DMA_Trancieve_State dma_state, IRQ_Recieve_State irq_state);
void testUART(USART_TypeDef* UART);
void sendUARTByDMA(USART_TypeDef* UART, uint8_t *array, uint8_t size);

#endif //STMSAMPLE_UART_DRIVER_H
