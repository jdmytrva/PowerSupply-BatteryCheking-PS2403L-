#include "stm32f10x.h"
#include "Lib.h"
#ifndef _USART_H_
#define _USART_H_

#define SIZE_BUF 200 //Rx buffer size
#define TX_SIZE_BUF 200 //Rx buffer size
#define USART_RESP 200 // Response from at command buffer size
#define USART_RESP_AMOUNT 3 // Responses stack size

void FlushBuf(void); //Clear rx buf
void USART_PutChar(uint8_t sym); //Put uint8_t to tx buffer



void USART1Init(void);
void Transmit(volatile char *data);
int StringMerge (volatile uint8_t * st1, volatile uint8_t*st2, volatile uint8_t res[]);
void TransmitChar(volatile char data);
void TransmitString(volatile char *data);
void Print_to_USART1_d(volatile int32_t value,volatile char *string,uint8_t koma);
void Print_to_USART1(volatile char *data);

#endif
