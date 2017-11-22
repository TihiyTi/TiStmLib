//
// Created by Alex on 03.08.2017.
//

#include <queue.h>
#include "nextion_core.h"
#include "uart_buffer.h"

#define NEXTION_WORD 7

QueueByte nextionRXQueue;

uint8_t nextionRXarray[32];

int nextionProtocolCrash = 0;

USART_TypeDef * nextionUsart = 0;

void NextionHandler(void) __attribute__((used));

void nextionInit(USART_TypeDef *setUsart){
    nextionUsart = setUsart;
    initBuffer(nextionUsart, 9600, &nextionRXQueue, nextionRXarray, 32);
}

void nextionProtocolParse(){
    uint8_t data[7];
    while (isContainRX(nextionUsart) >= NEXTION_WORD){
        data[0] = takeFromRX(nextionUsart);
        if(data[0] == 0x65){
            data[1] = takeFromRX(nextionUsart);
            data[2] = takeFromRX(nextionUsart);
            data[3] = takeFromRX(nextionUsart);
            data[4] = takeFromRX(nextionUsart);
            data[5] = takeFromRX(nextionUsart);
            data[6] = takeFromRX(nextionUsart);
            if((data[1] == 0x00) && (data[3] == 0x01) &&
                    (data[4] == 0xFF)&& (data[5] == 0xFF)&& (data[6] == 0xFF)){
                switch(data[2])
                {
                    case 0x01: //STOP
                        NextionCommand_1();
                        break;
                    case 0x02: //PLAY
                        NextionCommand_2();
                        break;
                    case 0x03: //Full amplitude
                        NextionCommand_3();
                        break;
                    case 0x04: //Half amplitude
                        NextionCommand_4();
                        break;
                    case 0x05: //P1
                        NextionCommand_5();
                        break;
                    case 0x06: //P2
                        NextionCommand_6();
                        break;
                    case 0x07: //1Hz
                        NextionCommand_7();
                        break;
                    case 0x08: //2.5Hz
                        NextionCommand_8();
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
