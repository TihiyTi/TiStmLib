//
// Created by Alex on 03.08.2017.
//

#ifndef STMSAMPLE_NEXTION_MANAGER_H
#define STMSAMPLE_NEXTION_MANAGER_H

void NextionCommand_1(void) __attribute__((weak));
void NextionCommand_2(void) __attribute__((weak));
void NextionCommand_3(void) __attribute__((weak));
void NextionCommand_4(void) __attribute__((weak));
void NextionCommand_5(void) __attribute__((weak));
void NextionCommand_6(void) __attribute__((weak));
void NextionCommand_7(void) __attribute__((weak));
void NextionCommand_8(void) __attribute__((weak));

void nextionInit(USART_TypeDef *usart);
void nextionProtocolParse();

#endif //STMSAMPLE_NEXTION_MANAGER_H
