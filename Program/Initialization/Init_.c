#include "Init_.h"

void Initialization(void)
{


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

//	InitTimer2ForDelay();
	InitLCD();
	USART1Init();
//	OneWireInitGPIOs();
// 	init_timer6();
	//AdcInit();
	Init_Out();
	Init_button();
	init_timer7();

	init_timer3();
	init_timer16();
	Init_Out();

	ADC1_CH_DMA_Config();
}

Init_Out()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*GPIOA->CRL &= ~GPIO_CRL_MODE4;
	GPIOA->CRL &= ~GPIO_CRL_CNF4;
	GPIOA->CRL |=  GPIO_CRL_MODE4_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF4;

	GPIOA->CRL &= ~GPIO_CRL_MODE0;
	GPIOA->CRL &= ~GPIO_CRL_CNF0;
	GPIOA->CRL |=  GPIO_CRL_MODE0_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF0;

	GPIOA->CRL &= ~GPIO_CRL_MODE1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL |=  GPIO_CRL_MODE1_1;
	GPIOA->CRL &= ~GPIO_CRL_CNF1;

	*/
//out
	GPIOB->CRL &= ~GPIO_CRL_MODE0;
	GPIOB->CRL &= ~GPIO_CRL_CNF0;
	GPIOB->CRL |=  GPIO_CRL_MODE0_1;
	GPIOB->CRL &= ~GPIO_CRL_CNF0;

	//led
	GPIOA->CRH &= ~GPIO_CRH_MODE8;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	GPIOA->CRH |=  GPIO_CRH_MODE8_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	//led
	GPIOA->CRH &= ~GPIO_CRH_MODE11;
	GPIOA->CRH &= ~GPIO_CRH_CNF11;
	GPIOA->CRH |=  GPIO_CRH_MODE11_1;
	GPIOA->CRH &= ~GPIO_CRH_CNF11;
//fan
//	GPIOA->CRL &= ~GPIO_CRL_MODE1;
//	GPIOA->CRL &= ~GPIO_CRL_CNF1;
//	GPIOA->CRL |=  GPIO_CRL_MODE1_1;
//	GPIOA->CRL &= ~GPIO_CRL_CNF1;

//Load
	GPIOB->CRL &= ~GPIO_CRL_MODE1;
	GPIOB->CRL &= ~GPIO_CRL_CNF1;
	GPIOB->CRL |=  GPIO_CRL_MODE1_1;
	GPIOB->CRL &= ~GPIO_CRL_CNF1;
}

void Init_button(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIOB->CRL &= ~GPIO_CRL_MODE5;
	GPIOB->CRL &= ~GPIO_CRL_CNF5;
	GPIOB->CRL |=  GPIO_CRL_CNF5_1;
	GPIOB->BSRR =  GPIO_BSRR_BS5;

	GPIOB->CRL &= ~GPIO_CRL_MODE4;
	GPIOB->CRL &= ~GPIO_CRL_CNF4;
	GPIOB->CRL |=  GPIO_CRL_CNF4_1;
	GPIOB->BSRR =  GPIO_BSRR_BS4;

	GPIOB->CRL &= ~GPIO_CRL_MODE6;
	GPIOB->CRL &= ~GPIO_CRL_CNF6;
	GPIOB->CRL |=  GPIO_CRL_CNF6_1;
	GPIOB->BSRR =  GPIO_BSRR_BS6;

	//GPIOA->CRH &= ~GPIO_CRH_MODE11; //OK
	//GPIOA->CRH &= ~GPIO_CRH_CNF11;
	//GPIOA->CRH |=  GPIO_CRH_CNF11_1;
	//GPIOA->BSRR =  GPIO_BSRR_BS11;

}

void init_timer7()
{

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);

  //base_timer.TIM_Prescaler = 240 - 1;
  //base_timer.TIM_Period = 10;
  base_timer.TIM_Prescaler = 2400 - 1;
  base_timer.TIM_Period = 10;//10- 1ms
  TIM_TimeBaseInit(TIM7, &base_timer);

  TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM7, ENABLE);


  NVIC_EnableIRQ(TIM7_IRQn);
}
void init_timer16()
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
  TIM_TimeBaseInitTypeDef base_timer;
  TIM_TimeBaseStructInit(&base_timer);

  //base_timer.TIM_Prescaler = 240 - 1;
  //base_timer.TIM_Period = 10;
  base_timer.TIM_CounterMode = TIM_CounterMode_Up;
  base_timer.TIM_Prescaler = 24000 - 1;
  base_timer.TIM_Period = 1000;//1c
  TIM_TimeBaseInit(TIM16, &base_timer);

  TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM16, ENABLE);


  NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
}
