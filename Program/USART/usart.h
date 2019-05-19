#include "stm32f10x.h"

#define SIZE_BUF 200 //Rx buffer size
#define TX_SIZE_BUF 200 //Rx buffer size
#define USART_RESP 200 // Response from at command buffer size
#define USART_RESP_AMOUNT 3 // Responses stack size

void FlushBuf(void); //Clear rx buf
void USART_PutChar(unsigned char sym); //Put char to tx buffer


void itoa(int32_t n, uint8_t s1[]);
void USART1Init(void);
void Transmit(volatile char *data);
void itoa_koma(int32_t n, uint8_t s1[],uint8_t koma);
int StringMerge (volatile char * st1, volatile char*st2, volatile char res[]);
uint8_t strlen(uint8_t string[]);
void TransmitChar(volatile char data);
void TransmitString(volatile char *data);
