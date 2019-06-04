#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include "hd44780_driver/hd44780_driver.h"
#include "usart.h"
#include "ADC.h"

#ifndef _INIT_
#define _INIT_


void Initialization(void);

void Init_Out(void);
void Init_button(void);
void init_timer7();
void init_timer16();
#endif
