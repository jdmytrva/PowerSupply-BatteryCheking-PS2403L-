#include "onewire.h"
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_conf.h>
//#include "misc.h"

volatile uint32_t OneWire_Timer=0;
volatile uint32_t OneWire_Timer1=0;


void OneWireInitGPIOs(void)
{
	GPIO_InitTypeDef InitGpio;
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	InitGpio.GPIO_Pin = ONEWIRE_SEND_PIN;
	InitGpio.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_OUT;
	InitGpio.GPIO_Speed = GPIO_Speed_2MHz;
	//InitGpio.GPIO_OType = GPIO_OType_PP;
	//InitGpio.GPIO_PuPd = GPIO_PuPd_NOPULL;


	GPIO_Init(GPIOA, &InitGpio);
	InitGpio.GPIO_Pin = ONEWIRE_RECEIVE_PIN;
	InitGpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &InitGpio);
	//GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}


volatile uint16_t timek = 12;
void OneWireDelay(volatile uint32_t us)
{
	Delay_us(us*timek);
}


void Delay_us(volatile uint32_t value)
{
  TIM2->ARR = value;
  TIM2->CNT = 0;
  TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;
  while((TIM2->SR & TIM_SR_UIF)==0){}
  TIM2->SR &= ~TIM_SR_UIF;
}
void InitTimer2ForDelay(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC     = F_CPU/1000000-1;//7;//8035-1;
	TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;
}


void OneWireLow()
{
	GPIO_ResetBits(GPIOA,ONEWIRE_SEND_PIN);

	//OneWireL_Print_to_USART1("OneWire LOW");
}
void OneWireHigh()
{
	GPIO_SetBits(GPIOA, ONEWIRE_SEND_PIN);

	//OneWireL_Print_to_USART1("OneWire HIGH");
}
uint8_t OneWireLevel()
{

	//return GPIO_ReadInputDataBit(GPIOA, ONEWIRE_RECEIVE_PIN);
	if ((GPIOA->IDR & ONEWIRE_RECEIVE_PIN)==0) return 0;
	else return 1;

}

void OneWireL_Print_to_USART1(char * str)
{
	//Print_to_USART1(str);
}
void OneWireL_Print_to_USART1_d(volatile uint32_t value,volatile char *string,char koma)
{
	//Print_to_USART1_d(value,string,koma);
}
void OneWire_Print_to_USART1(char * str)
{
	//Print_to_USART1(str);
}
void OneWire_Print_to_USART1_d(volatile uint32_t value,volatile char *string,char koma)
{
	Print_to_USART1_d(value,string,koma);
}
uint8_t OneWire_Master_reset()
{
	//OneWireL_Print_to_USART1("OneWire RESET Procedure +++++++++++++++++++++++++++++++++++++++++");

	OneWireDelay(1);
	OneWireLow();
	OneWireDelay(600);// min 480mks, max 960 was720
	OneWireHigh();
	OneWireDelay(3);
	OneWire_Timer = 0;
	while(OneWire_Timer<=60)
	{
		//OneWireL_Print_to_USART1("OneWire Waiting for PRESENCE impulse 60 mks");

		if(!OneWireLevel())
		{
			//OneWireL_Print_to_USART1_d(OneWire_Timer,"OneWire Presence started in  : ",0);
			OneWire_Timer1 = 0;
			while(OneWire_Timer1<=285)
			{
				//OneWireL_Print_to_USART1("OneWire Waiting for end of PRESENCE max 285 mks");
				OneWireDelay(1);
				if(OneWireLevel())
				{
					//OneWireL_Print_to_USART1_d(OneWire_Timer1,"OneWire Presence finished in : ",0);
					return 1;
				}
			}
			//OneWireL_Print_to_USART1("OneWire PRESENCE impulse is not finished in 285 mks");
			return 2;
		}
		OneWireDelay(1);
	}
	//OneWireL_Print_to_USART1("OneWire PRESENCE impulse is not started in 60 mks");
	return 0;
}




void OneWire_Master_send_bit(uint8_t bit)
{
	OneWireDelay(3);
	OneWireLow();
	if (bit)
	{
		OneWireDelay(7);
		OneWireHigh();
		OneWireDelay(90);
	}else
	{
		OneWireDelay(90);
		OneWireHigh();
		OneWireDelay(7);
	}
}

void OneWire_Master_send(uint8_t b)
{
	//OneWireL_Print_to_USART1("OneWire Master send byte");
	uint8_t p;
	for (p = 8; p; p--)
	{
		OneWire_Master_send_bit(b & 1);
		b >>= 1;
	}
}


uint8_t OneWire_Master_ReadBit()
{
	OneWireDelay(10);
	OneWireLow();
	OneWireDelay(10);
	OneWireHigh();
	OneWireDelay(25);
	uint8_t r = OneWireLevel();
	OneWireDelay(100);


	return r;
}


uint8_t OneWire_Master_read()
{
  uint8_t r = 0;
  uint8_t p;
  for (p = 8; p; p--) {
    r >>= 1;
    if (OneWire_Master_ReadBit())
      r |= 0x80;
  }
  return r;
}

uint16_t OneWire_Master_read2byte()
{
	uint16_t f = 0;
	uint8_t *c = (uint8_t*) &f;

	c[0] = OneWire_Master_read();
	//OneWireDelay(10);
	c[1] = OneWire_Master_read();
	return f;
}


//0x55 MATCH ROM
/*
***********************************************************************
* Расчет циклического избыточного кода CRC для массива сообщения
* Используется полином x^8 + x^2 + x + 1 (CRC-8-Dallas/Maxim)
* Задается указатель на начало рассчитываемых байтов в массиве,
* количество байтов, участвующих в расчете (ровно)
* начальный код CRC (если считается не с начала массива),
* Для ускорения расчета используется таблица
***********************************************************************/
char OneWire_calcCRC(char *Ptr, char Num, char CRC1)
{
   char CrcTable[256] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, // 00
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d, // 08
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, // 10
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, // 18
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, // 20
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD, // 28
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,  // 30
    0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,  // 38
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,  // 40
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,  // 48
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,  // 50
    0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,  // 58
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,  // 60
    0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,  // 68
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,  // 70
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,  // 78
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,  // 80
    0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,  // 88
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,  // 90
    0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,  // 98
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,  // a0
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,  // a8
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,  // b0
    0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,  // b8
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,  // c0
    0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,  // c8
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,  // d0
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, // d8
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, // e0
    0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83, // e8
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, // f0
    0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3 // f8
  };
  do  CRC1 = CrcTable[CRC1 ^ *Ptr++]; while (--Num);
  return CRC1;
}
void init_timer6()
{

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);


  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);

  base_timer.TIM_Prescaler = F_CPU/1000000-1;//80;
    base_timer.TIM_Period = timek;
  TIM_TimeBaseInit(TIM6, &base_timer);


  TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM6, ENABLE);


  NVIC_EnableIRQ(TIM6_DAC_IRQn);
}


void TIM6_DAC_IRQHandler()
{

  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {

    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

	OneWire_Timer++;
	OneWire_Timer1++;




  }
}


uint32_t getTimer()
{
	return OneWire_Timer;
}
