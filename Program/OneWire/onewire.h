#include "stm32f10x.h"
//#include <system_stm32f0xx.h>
#define OLD_BOARD 0

#define F_CPU 24000000UL
// old
volatile uint32_t OneWire_Timer;
volatile uint32_t OneWire_Timer1;

#define ONEWIRE_SEND_PIN GPIO_Pin_4
#define ONEWIRE_RECEIVE_PIN GPIO_Pin_3
//#define TEMPERATURE_PIN GPIO_Pin_3

void OneWireInitGPIOs(void);
char OneWire_calcCRC(char *Ptr, char Num, char CRC1);
void OneWireLow();
void OneWireHigh();
uint8_t OneWireLevel();
void OneWireL_Print_to_USART1(char * str);
void OneWireL_Print_to_USART1_d(volatile uint32_t value,volatile char *string,char koma);
void OneWire_Print_to_USART1(char * str);
void OneWire_Print_to_USART1_d(volatile uint32_t value,volatile char *string,char koma);
uint8_t OneWire_Master_reset();
uint8_t OneWire_Slave_presence();
uint8_t OneWire_Slave_WaitingReset();
void OneWire_Master_send_bit(uint8_t bit);
void OneWire_Slave_send_bit(uint8_t bit);
void OneWire_Master_send(uint8_t b);
void OneWire_Slave_send(uint8_t b);
void OneWire_Slave_send2byte(uint16_t b);
uint8_t OneWire_Master_ReadBit();
uint8_t OneWire_Slave_ReadBit();
uint8_t OneWire_Master_read();
uint16_t OneWire_Master_read2byte();
uint8_t OneWire_Slave_read();
void init_timer6();
//void TIM6_DAC_IRQHandler();
void OneWireDelay(uint32_t us);
void Delay_us(uint32_t value);
void InitTimer2ForDelay(void);
uint32_t getTimer();

