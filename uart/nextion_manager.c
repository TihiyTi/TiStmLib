//
// Created by Alex on 03.08.2017.
//

#include <queue.h>
#include "nextion_manager.h"
#include "uart_buffer.h"
#include "uart_driver.h"

#define NEXTION_WORD 7

QueueByte nextionRXQueue;
uint8_t nextionRXarray[32];

int nextionProtocolCrash = 0;

void nextionInit(){
//    initQueueByte(&nextionRXQueue, 32);
    initBuffer(UART4, 9600, &nextionRXQueue, nextionRXarray, 32);
}

void nextionProtocolParse(){
    uint8_t data[7];
    while (isContainRX(UART4) >= NEXTION_WORD){
        data[0] = takeFromRX(UART4);
        if(data[0] == 0x65){
            data[1] = takeFromRX(UART4);
            data[2] = takeFromRX(UART4);
            data[3] = takeFromRX(UART4);
            data[4] = takeFromRX(UART4);
            data[5] = takeFromRX(UART4);
            data[6] = takeFromRX(UART4);
            if((data[1] == 0x00) && (data[3] == 0x01) &&
                    (data[4] == 0xFF)&& (data[5] == 0xFF)&& (data[6] == 0xFF)){
                switch(data[2])
                {
                    case 0x01: //STOP
                        USART_SendData(UART4, data[2]);
//                        sendUARTByDMA(UART4, (uint8_t *) "Stop", 4);
//                        stopMagnit();
                        //do smth
                        break;
                    case 0x02: //PLAY
                        USART_SendData(UART4, data[2]);
//                        startMagnit();
                        //do smth
                        break;
                    case 0x03: //Full amplitude
                        USART_SendData(UART4, data[2]);
//                        setAmplitude(255);
                        break;
                    case 0x04: //Half amplitude
                        USART_SendData(UART4, data[2]);
//                        setAmplitude(256/2);
                        //do smth
                        break;
                    case 0x05: //P1
                        USART_SendData(UART4, data[2]);
                        //do smth
                        break;
                    case 0x06: //P2
                        USART_SendData(UART4, data[2]);
                        //do smth
                        break;
                    case 0x07: //1Hz
                        USART_SendData(UART4, data[2]);
//                        setFrequency(FREQ_1Hz);
                        //do smth
                        break;
                    case 0x08: //2.5Hz
                        USART_SendData(UART4, data[2]);
//                        setFrequency(FREQ_2Hz);
                        break;
                    default:break;
                }
            }else{
                nextionProtocolCrash++;
            }
        }else{
            nextionProtocolCrash++;
        }
    }
}
