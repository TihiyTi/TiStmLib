//
// Created by Alex on 14.08.2017.
//

#include "rcc_driver.h"
#include "stm32f4xx_rcc.h"

/**
 * @brief  Config clock from source HSE = 8MHz
 * target frequency 72MHz
 */
void rccExternalInit(){
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);


    RCC_HCLKConfig(RCC_SYSCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);

    RCC_PLLConfig(RCC_PLLSource_HSE, 8 , 288, 4, 4);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);
}

void rccInternalInit(){
//    unsupported yet
}