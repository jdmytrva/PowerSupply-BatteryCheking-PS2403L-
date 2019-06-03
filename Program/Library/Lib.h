#include "stm32f10x.h"
#ifndef _LIB_H_
#define _LIB_H_
uint8_t * itoa(int32_t n);
uint8_t * itoa_koma(int32_t n, uint8_t koma);
int32_t Module32 (int32_t value);
int16_t Module16 (int16_t value);
int8_t Module8 (int8_t value);
uint8_t strlen1(uint8_t string[]);

#endif
