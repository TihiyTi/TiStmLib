#include <stm32f4xx.h>
#include <queue.h>
#include "uart_driver.h"
#include "uart_buffer.h"

// 16 byte in RAM
QueueByte* queueByteUSART1_RX;
uint8_t * bufUSART1_RX;
BufferDoubleByte* bufferByteUSART1_TX;
uint8_t * bufUSART1_TX1;
uint8_t * bufUSART1_TX2;

// 16 byte in RAM
QueueByte* queueByteUSART3_RX;
uint8_t * bufUSART3_RX;
BufferDoubleByte* bufferByteUSART3_TX;
uint8_t * bufUSART3_TX1;
uint8_t * bufUSART3_TX2;

// 16 byte in RAM
QueueByte* queueByteUSART4;
uint8_t * bufUSART4;


//Log data
//todo вынести в структуру отображающую статус модуля UART буфферов
uint8_t dma_wait_UART3 = 0;

//#define USART3_addToBuffer USART3_IRQHandler

void sendBuffer(USART_TypeDef* UART);
void USART3_addToBuffer() __attribute__((used));

void initBuffer(USART_TypeDef* UART, uint32_t speed,
                QueueByte* queueByte, uint8_t * buf, uint8_t size){
    initUSART(UART, speed, DMA_ENABLE, IRQ_ENABLE);
    initQueueByte(queueByte, size);
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            queueByteUSART1_RX = queueByte;
            bufUSART1_RX = buf;
            queueByteUSART1_RX->buf = bufUSART1_RX;
            break;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            break;
        case (uint32_t) USART3:
            queueByteUSART3_RX = queueByte;
            bufUSART3_RX = buf;
            queueByteUSART3_RX->buf = bufUSART3_RX;
            break;
        case (uint32_t) UART4:
            queueByteUSART4 = queueByte;
            bufUSART4 = buf;
            queueByteUSART4->buf = bufUSART4;
            break;
        default:
            break;
    }
}

void initBufferTX(USART_TypeDef* UART,
                  BufferDoubleByte* bufferDoubleByte, uint8_t * buf1, uint8_t * buf2, uint8_t size){
    initBufferDoubleByte(bufferDoubleByte, size);
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            bufferByteUSART1_TX = bufferDoubleByte;
            bufUSART1_TX1 = buf1;
            bufUSART1_TX2 = buf2;
            bufferByteUSART1_TX->buf1= bufUSART1_TX1;
            bufferByteUSART1_TX->buf2= bufUSART1_TX2;
            break;
//        case (uint32_t) USART2:
//            //todo не реализовано для USART2
//            break;
        case (uint32_t) USART3:
            bufferByteUSART3_TX = bufferDoubleByte;
            bufUSART3_TX1 = buf1;
            bufUSART3_TX2 = buf2;
            bufferByteUSART3_TX->buf1 = bufUSART3_TX1;
            bufferByteUSART3_TX->buf2 = bufUSART3_TX2;
            break;
//        case (uint32_t) UART4:
//            queueByteUSART4 = queueByte;
//            bufUSART4 = buf1;
//            queueByteUSART4->buf1 = bufUSART4;
//            break;
        default:
            break;
    }
}

#if defined(BUFFER_UART1)
void USART1_IRQHandler(){
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		uint8_t rxByte = (uint8_t) USART_ReceiveData(USART1);
        addToQueueByte(queueByteUSART1_RX, rxByte);
	}
	if(USART_GetITStatus(USART1, USART_IT_ORE)!= RESET){
		USART_ClearITPendingBit(USART1, USART_IT_ORE);
//		error_uart1_rx ++;
	}
}
#endif

#if defined(BUFFER_UART2)
void USART2_IRQHandler(){
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);

		rxByte = (uint8_t) USART_ReceiveData(USART2);
		addToRX1Buffer(rxByte);
	}
	if(USART_GetITStatus(USART2, USART_IT_ORE)!= RESET){
		USART_ClearITPendingBit(USART2, USART_IT_ORE);
		error_uart1_rx ++;
	}
}

#endif

//void USART3_IRQHandler(){
void USART3_addToBuffer(){
    __disable_irq();
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        uint8_t rxByte = (uint8_t) USART_ReceiveData(USART3);
        addToQueueByte(queueByteUSART3_RX, rxByte);
	}
	if(USART_GetITStatus(USART3, USART_IT_ORE)!= RESET){
		USART_ClearITPendingBit(USART3, USART_IT_ORE);
//		error_uart1_rx ++;
	}
    __enable_irq();
}

void USART4_addToBuffer(){
//void UART4_IRQHandler(){
    __disable_irq();
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);
        uint8_t rxByte = (uint8_t) USART_ReceiveData(UART4);
        addToQueueByte(queueByteUSART4, rxByte);
    }
    if(USART_GetITStatus(UART4, USART_IT_ORE)!= RESET){
        USART_ClearITPendingBit(UART4, USART_IT_ORE);
    }
    __enable_irq();
}


uint8_t takeFromRX(USART_TypeDef* UART){
    uint8_t result;
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
            result = takeQueueByte(queueByteUSART1_RX);
            USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
            break;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            result = 0;
            break;
        case (uint32_t) USART3:
            //Block add element to queue to DISABLE irq from UART
            // Safety take element from queue
            USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
            result = takeQueueByte(queueByteUSART3_RX);
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
            break;
        case (uint32_t) UART4:
            //Block add element to queue to DISABLE irq from UART
            // Safety take element from queue
            USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
            result = takeQueueByte(queueByteUSART4);
            USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

void addToTXBuffer(USART_TypeDef* UART, uint8_t addingByte){
    uint8_t state;
    switch ((uint32_t)UART) {
        case (uint32_t) USART1:
            break;
        case (uint32_t) USART2:
            //todo не реализовано для USART2
            break;
        case (uint32_t) USART3:
            state = addToBufferDoubleByte(bufferByteUSART3_TX, addingByte);
            if(state == 1){
                sendBuffer(UART);
            }
            break;
        case (uint32_t) UART4:
            break;
        default:
            break;
    }
}

void sendBuffer(USART_TypeDef* UART){
    switch ((uint32_t)UART) {
//        case (uint32_t) USART1:
//            break;
//        case (uint32_t) USART2:
//            todo не реализовано для USART2
//            break;
        case (uint32_t) USART3:
            if(bufferByteUSART3_TX->activeBuffer == 1){
                sendUARTByDMA(UART, bufferByteUSART3_TX->buf1, bufferByteUSART3_TX->head);
            }if(bufferByteUSART3_TX->activeBuffer == 2){
                sendUARTByDMA(UART, bufferByteUSART3_TX->buf2, bufferByteUSART3_TX->head);
            }
            toggleActiveBufferDoubleByte(bufferByteUSART3_TX);
            break;
//        case (uint32_t) UART4:
//            break;
        default:
            break;
    }

}

uint8_t isContainRX(USART_TypeDef* UART){
    if(UART == USART1){
        return queueByteUSART1_RX -> size;
    }else if(UART == USART2){
        //todo unsupported yet
        return 0;
    }else if(UART == USART3){
        return queueByteUSART3_RX -> size;
    }else if(UART == UART4){
        return queueByteUSART4 -> size;
    }else{
        return 0;
    }
}