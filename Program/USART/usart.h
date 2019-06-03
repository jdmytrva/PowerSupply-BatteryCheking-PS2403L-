#include "stm32f10x.h"
#ifndef _USART_H_
#define _USART_H_

#define SIZE_BUF 200 //Rx buffer size
#define TX_SIZE_BUF 200 //Rx buffer size
#define USART_RESP 200 // Response from at command buffer size
#define USART_RESP_AMOUNT 3 // Responses stack size

void FlushBuf(void); //Clear rx buf
void USART_PutChar(uint8_t sym); //Put uint8_t to tx buffer



void USART1Init(void);
void Transmit(volatile uint8_t *data);
int StringMerge (volatile uint8_t * st1, volatile uint8_t*st2, volatile uint8_t res[]);
uint8_t strlen(uint8_t string[]);
void TransmitChar(volatile uint8_t data);
void TransmitString(volatile uint8_t *data);
void Print_to_USART1_d(volatile int32_t value,volatile uint8_t *string,uint8_t koma);

#endif
