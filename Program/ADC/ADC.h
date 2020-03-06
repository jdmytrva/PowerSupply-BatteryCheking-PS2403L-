#include <stm32f10x_conf.h>
#include <stm32f10x.h>
#define ADC1_DR_Address               ((uint32_t)0x4001244C)
//median filter
#define NULL         0
#define STOPPER   0 /* Smaller than any datum */
#define MEDIAN_FILTER_SIZE 21 //Voltage
#define MEDIAN_FILTER_SIZE1 41 //Current
#define MEDIAN_FILTER_SIZE2 41 //Current MEDIAN_FILTER_SIZE1  = MEDIAN_FILTER_SIZE2 !!! it musts

#define NUMBER_OF_CHANNELS 8
__IO uint16_t RegularConvData[NUMBER_OF_CHANNELS] ;

volatile uint16_t U_OUT;
volatile uint16_t U_PS;
volatile uint16_t U_IN;
volatile uint16_t U_Controller;
volatile uint16_t U_12V;
volatile int16_t Current;


void ADC1_CH_DMA_Config(void);
void init_timer3();
uint16_t MedianFilter1(uint16_t datum);
uint16_t MedianFilter(uint16_t datum);
uint16_t MedianFilter2(uint16_t datum);
int32_t middle_of_3(int32_t a, int32_t b, int32_t c);
int32_t middle_of_3Umax(int32_t value);
int32_t middle_of_3Imax(int32_t value);
