#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

#ifndef _INIT_
#define _INIT_


void Initialization(void);

void Init_Out();
void Init_button(void);
void init_timer7();
void init_timer16();
#endif
