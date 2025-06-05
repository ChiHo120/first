#ifndef _HEADFILE_H
#define _HEADFILE_H


#include "stm32g4xx.h"                  // Device header

#include "main.h"
#include "gpio.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "tran.h"
#include "adc.h"
#include "fun.h"
#include "key.h"
#include "lcd.h"
#include "tim.h"
#include "usart.h"
#include "i2c_hal.h"


extern uint8_t B3_pressed;
extern uint8_t B3_time;
extern uint8_t led1_flag;
extern uint8_t led2_flag;
extern uint8_t uart_flag;
#endif

