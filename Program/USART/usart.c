#include "usart.h"
//#include <system_stm32f0xx.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_conf.h>
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include <stm32f10x_conf.h>
//#include "stm32f10x_dma.h"
#include "misc.h"
#include "Library/Lib.h"
#define ASYNCHRONOUS_PRINT_TO_USART 0
void USART1Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Enable GPIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* Enable USART clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* Connect PXx to USARTx_Tx */
    //GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

    /* Connect PXx to USARTx_Rx */
    //GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    /* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   // GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(USART1, &USART_InitStructure);
    /* NVIC configuration */

    //NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
      NVIC_InitStructure.NVIC_IRQChannel =USART1_IRQn;   //NVIC_IRQChannel  = USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 34;//NVIC_IRQChannelPriority = 34;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      /* Enable USART */
      USART_Cmd(USART1, ENABLE);
      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


//кольцевой (циклический) буфер
volatile uint8_t RXBuf[SIZE_BUF]; //main buffer
volatile uint8_t RXtail = 0;      //"указатель" хвоста буфера
volatile uint8_t RXhead = 0;   //"указатель" головы буфера
volatile uint8_t RXcount = 0;  //счетчик символов

volatile uint8_t TXBuf[TX_SIZE_BUF];
volatile uint8_t TXtail = 0;
volatile uint8_t TXhead = 0;
volatile uint8_t TXcount = 0;

//положить символ в буфер
void PutChar(uint8_t sym)
{
  if (RXcount < SIZE_BUF){   //если в буфере еще есть место
      RXBuf[RXtail] = sym;   //помещаем в него символ
      RXcount++;             //инкрементируем счетчик символов
      RXtail++;              //и индекс хвоста буфера
      if (RXtail == SIZE_BUF) RXtail = 0;
    }
}


void USART_PutChar(uint8_t sym)
{
  if((USART1->SR & USART_SR_TC) && (TXcount == 0)){
	  USART1->DR = sym;
  }
  else {
	USART1->CR1 &= ~USART_CR1_TXEIE;//!<PE Interrupt Disable
    if (TXcount < TX_SIZE_BUF){                    //если в буфере еще есть место
      TXBuf[TXtail] = sym;             //помещаем в него символ
      TXcount++;                                     //инкрементируем счетчик символов
      TXtail++;                                    //и индекс хвоста буфера
      if (TXtail == TX_SIZE_BUF) TXtail = 0;
    }
	USART1->CR1 |= USART_CR1_TXEIE;/*!<PE Interrupt enable */
	if((USART1->SR & USART_SR_TXE)!=0){// If Transmit Data Register Empty
		USART1_IRQHandler(); //Call usart interruption to send data
	}
  }
}

void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_RXNE){/*!<Read Data Register Not Empty */
		PutChar(USART1->DR);
	}
	if((USART1->SR & USART_SR_TXE)!=0){// If Transmit Data Register Empty
		 if (TXcount > 0){                       //если буфер не пустой
		    USART1->DR = TXBuf[TXhead];  //записываем в DR символ из буфера
		    TXcount--;                           //уменьшаем счетчик символов
		    TXhead++;                         //инкрементируем индекс головы буфера
		    if (TXhead == TX_SIZE_BUF) TXhead = 0;
		  }
		 else USART1->CR1 &= ~USART_CR1_TXEIE;//!<PE Interrupt Disable
	}
	if((USART1->SR & USART_SR_TC) != 0){//!<Transmission Complete
		USART1->SR &= ~USART_SR_TC; //Clear flag --^
	}
}


void Print_to_USART1_d(volatile int32_t value,volatile char *string,uint8_t koma)
{
	char *str;

	if (koma == 0)
		str = itoa(value);
	else
		str = itoa_koma(value,koma);
	TransmitString(string);
	TransmitString(str);
	TransmitChar('\n');

}
void Print_to_USART1(volatile char *data)
{
	#if ASYNCHRONOUS_PRINT_TO_USART
		while(*data)
		{

			USART_PutChar(*data);
			data++;
		}

			USART_PutChar('\n');
	#else
		while(*data)
		{
		  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} //Проверка завершения передачи предыдущих данных
		  USART_SendData(USART1, *data);
		  data++;
		}
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){} //Проверка завершения передачи предыдущих данных
		  USART_SendData(USART1, '\n');
	#endif


}

void TransmitChar(volatile char data)
{
#if ASYNCHRONOUS_PRINT_TO_USART
	USART_PutChar(data);
#else
	while(!(USART1->SR & USART_SR_TC)); //Проверка завершения передачи предыдущих данных
	USART1->DR = data; //Передача данных
#endif
}

void TransmitString(volatile char *data)
{
#if ASYNCHRONOUS_PRINT_TO_USART
	  while(*data)
	  {
		  USART_PutChar(*data);
		  data++;
	  }
#else
  while(*data)
  {
	  while(!(USART1->SR & USART_SR_TC)); //Проверка завершения передачи предыдущих данных
	  USART1->DR = *data; //Передача данных
	  data++;
  }
#endif
}

