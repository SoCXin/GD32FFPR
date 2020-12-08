#ifndef UART_H
#define UART_H

#include <stdio.h>
#include "gd32f30x.h"

#define USART_REC_LEN               300

#define COMn                        2U

#define EVAL_COM0                   USART0
#define EVAL_COM0_CLK               RCU_USART0
#define EVAL_COM0_TX_PIN            GPIO_PIN_9
#define EVAL_COM0_RX_PIN            GPIO_PIN_10
#define EVAL_COM0_GPIO_PORT         GPIOA
#define EVAL_COM0_GPIO_CLK          RCU_GPIOA

#define EVAL_COM1                   USART1
#define EVAL_COM1_CLK               RCU_USART1
#define EVAL_COM1_TX_PIN            GPIO_PIN_2
#define EVAL_COM1_RX_PIN            GPIO_PIN_3
#define EVAL_COM1_GPIO_PORT         GPIOA
#define EVAL_COM1_GPIO_CLK          RCU_GPIOA

extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;

void gd_eval_com_init(uint32_t com);

#endif
