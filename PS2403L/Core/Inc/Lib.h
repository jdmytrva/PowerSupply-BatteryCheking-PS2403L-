#ifndef _LIB_H_
#define _LIB_H_
#include "stm32f1xx.h"
char * itoa(int32_t n);
char * itoa_koma(int32_t n, uint8_t koma);
char * itoaP(int32_t n,char *s1);
char * itoa_komaP(int32_t n,char *s1, uint8_t koma);
int32_t Module32 (int32_t value);
int16_t Module16 (int16_t value);
int8_t Module8 (int8_t value);
uint8_t strlen1(char string[]);
char * Merge2Strings(char * str1in, char * str2in, char * strout);
char *ClockStringWithSec(uint32_t time, char *OutStr);
char *ClockStringNoSec(uint32_t time, char *OutStr);
char * Merge3Strings(char * str1in, char * str2in,char * str3in, char * strout);

int32_t middle_of_3Imax1(int32_t value);
int32_t middle_of_3Imax2(int32_t value);
int32_t middle_of_3Umax1(int32_t value);
int32_t middle_of_3Umax2(int32_t value);
int32_t middle_of_3Umax3(int32_t value);

int32_t RunningAverageI1(int32_t newValue);
int32_t RunningAverageI2(int32_t newValue);
int32_t RunningAverageU1(int32_t newValue);
int32_t RunningAverageU2(int32_t newValue);
int32_t RunningAverageU3(int32_t newValue);

#endif
