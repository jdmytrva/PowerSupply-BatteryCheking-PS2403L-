/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THRESHOLD2 1500
#define THRESHOLD 20

#define MENUDELAY 100
#define DELTA_UPS_BAT_MAX 5  //50mv
#define VOLTAGE_OFF_SYSTEM 1800
//#define VOLTAGE_OFF_SYSTEM 1400
//#define VOLTAGE_OFF_SYSTEM 700

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char Version[] = "PS+L 20V3A v2.60";

Key_Pressed_t pressedKey = 0;
volatile uint32_t  time_sec = 0;
volatile uint32_t  Timer_Sec = 0;
volatile uint32_t  TimerForReadyMeasurement_ms = 0;
volatile uint8_t  Status_Timer_Sec = 0;
volatile uint32_t  ChargeTimeSec = 0;
volatile uint32_t  DischargeTimeSec = 0;
volatile uint32_t  DischargeTimeSec_Previous = 0;
volatile uint32_t  PowerOffTimesec = 0;
volatile uint32_t  sec = 0;
volatile uint32_t  time_min = 0;
volatile uint32_t  time_hour = 0;
volatile uint32_t BatteryCapacityDischargeCurrent = 0;
volatile uint32_t BatteryCapacityDischargeCurrentAfterPOwerUp = 0;
volatile uint32_t BatteryCapacityCharge = 0;
int16_t i_LogItems=0;

volatile int32_t SumI1 = 0;
volatile int32_t SumI2 = 0;
volatile int32_t SumU1 = 0;
volatile int32_t SumU2 = 0;
volatile int32_t SumU3 = 0;

volatile int16_t SumI1Counter = 0;
volatile int16_t SumI2Counter = 0;
volatile int16_t SumU1Counter = 0;
volatile int16_t SumU2Counter = 0;
volatile int16_t SumU3Counter = 0;
volatile int32_t U_OUTtmp = 0;

void BUT_Debrief(void);
void init_timer6();
uint8_t BUT_GetKey(void);
void ResetTime(void);
void clock (void);



void ClockOnLCD (uint32_t time);
void ClockOnLCD_noSec (uint32_t time);

void discharge();
void charge();
void discharge1();
void charge1();
void OFF();
void OUT_OFF();
void OUT_ON();
void OUT_ON_OFF_Toggle();
void LOAD_ON();
void LOAD_OFF();
void LOAD_ON_OFF_Toggle();
void Start_Timer_sec();
void ReStart_Timer_sec();
void Stop_Timer_sec();
void All_OUT_OFF_When_Power_OFF();

volatile uint8_t On_off = 0;
uint8_t Status_Load = 0;
uint8_t Status_Out = 0;
int8_t CountShow = 0;
int8_t CountShow1 = 0;
volatile uint8_t EnterInMenu_Status = 0;
int8_t PopUpMessage = 0;
uint32_t SwingtimeSec = 0;
volatile uint8_t ChargeStatusForTimer = 0;
volatile uint8_t DisChargeStatusForTimer = 0;
uint8_t Blink_message_counter = 0;
uint8_t InitiStatus = 0;

uint32_t U_Battery_Timer = 0;
uint32_t VeryLowBattery_Timer = 0;
uint32_t OffCurrent_Timer = 0;
uint8_t BatteryLow=0;
uint8_t BatteryVeryLow=0;
uint16_t U_BatteryTmp = 0;
uint8_t OffCurrentStatus = 0;
uint8_t ChargedStatus = 1;
uint8_t BATERYSTATUS = 0;
uint32_t ChargeDurationSec;
uint32_t SelectedOptionValue;
uint32_t SelectedOptionValue1;
uint32_t OutStateAfterPowerUp = 1;
//uint32_t OutState = 0;


//////MENU FUNCTIONS ///////////////MENU FUNCTIONS ///////////////MENU FUNCTIONS /////////
void Generic_Write(char* Text)
{
	if (Text) PrintToLCD_xy(Text,0,0);
}
void OUTStateAfterPowerUpFunction(void)
{
    if (OutStateAfterPowerUp ==2)
    {
    	OUT_ON();
    	OutStateAfterPowerUp = 1;
    }
    if (OutStateAfterPowerUp ==3)
    {
    	if (SaveDataWhenPowerOff.OutState==1)
    	{
    		OUT_ON();
    		OutStateAfterPowerUp = 1;
    	}
    }

}
void MenuPowerSupply(Key_Pressed_t key) //PowerSupply
{
	EnterInMenu_Status = 1;
	lcd_set_xy(0,0);
	if (On_off == 0)
	{
		PrintToLCD(itoa_koma(U_PS,2));
		PrintToLCD("V  ");
		lcd_set_xy(7,0);
		PrintToLCD(itoa(Current));
		PrintToLCD("mA   ");
		lcd_set_xy(5,1);
		PrintToLCD("OFF ");
	}
	else
	{
		PrintToLCD(itoa_koma(U_OUT,2));
		PrintToLCD("V  ");
		lcd_set_xy(7,0);
		PrintToLCD(itoa(Current));
		PrintToLCD("mA    ");
		lcd_set_xy(6,1);
		PrintToLCD("ON ");
	}

    if (key == KEY_NEXT)
    {
    	if (On_off == 0)
    	{
    		OUT_ON();
    	}
    	else
    	{
    		OUT_OFF();
    	}
    }
    OUTStateAfterPowerUpFunction();

}

void MenuLoad(Key_Pressed_t key) //Load
{
	EnterInMenu_Status = 1;
	if (key == KEY_NEXT)
	{
		if (On_off == 0)
			LOAD_ON();
		else
			LOAD_OFF();
	}
	lcd_set_xy(0,0);
	PrintToLCD(itoa_koma(U_OUT,2));
	PrintToLCD("V ");

	PrintToLCD(itoa(Current));
	PrintToLCD("mA    ");
	if (On_off ==0)
		{
			lcd_set_xy(6,1);
			LcdOutbyNumber(2,1);
			LcdOutbyNumber(2,1);
		}
		else
			if (Blink_message_counter<=5)
			{
				lcd_set_xy(6,1);
				LcdOutbyNumber(2,1);
				LcdOutbyNumber(2,1);
			}
			else
			{
					lcd_set_xy(6,1);
					LcdOutbyNumber(3,1);
					LcdOutbyNumber(3,1);
			}
		if (Blink_message_counter>10)
			Blink_message_counter = 0;
}
void MenuChargeCC_CV(Key_Pressed_t key)
{
	EnterInMenu_Status = 1;
	if (InitiStatus==0)
	{
		if (U_PS < (SettingsData.MaxVoltage+DELTA_UPS_BAT_MAX))
		{
			lcd_set_xy(0,0);
			PrintToLCD("U(ps)<U(bat.max)");
			delay_ms(2000);
			lcd_set_xy(0,0);
			PrintToLCD("UP U(ps)        ");
			delay_ms(3000);

		}
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;
		BatteryCapacityCharge = 0;
		InitiStatus = 1;
		ChargeTimeSec = 0;
	}

	if ((ChargeTimeSec > ChargeDurationSec)|| (U_OUT> SettingsData.MaxVoltage))
	{
		OFF();
		ChargeStatusForTimer = 0;

	}


	#define MAXITEM0 3
    OUTStateAfterPowerUpFunction();
	if (key == KEY_NEXT)
	{
		if (On_off == 0)
		{
			charge();
			ChargeStatusForTimer = 1;
		}
		else
		{
			OFF();
			ChargeStatusForTimer = 0;
		}
	}
	if (key == KEY_BACK)
	{
		CountShow--;
		if (CountShow<0) CountShow=MAXITEM0-1;
	}
	if(CountShow == 0)
	{
		lcd_set_xy(0,0);
		if (On_off ==0)
		{
			PrintToLCD(itoa_koma(U_PS,2));
			PrintToLCD("V ");
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V     ");

		}
		else
		{
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V ");
			PrintToLCD(itoa(Current));
			PrintToLCD("mA     ");
		}
		if (On_off ==0)
		{
			lcd_set_xy(7,1);
			LcdOutbyNumber(0,1);
		}
		else
			if (Blink_message_counter<=5)
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(0,1);
			}
			else
			{
					lcd_set_xy(7,1);
					LcdOutbyNumber(1,1);
			}
		if (Blink_message_counter>10)
			Blink_message_counter = 0;
	}

	if(CountShow == 1)
	{
		lcd_set_xy(0,0);
		PrintToLCD("T:");
		PrintToLCD(itoa(SettingsData.ChargeTime));
		PrintToLCD("h MaxV:");
		PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
		PrintToLCD("V   ");
	}
	if(CountShow == 2)


	{
		lcd_set_xy(0,0);
		PrintToLCD("C ");
		PrintToLCD(itoa(BatteryCapacityCharge/3600));
		PrintToLCD("mAh     ");
		lcd_set_xy(3,1);
		ClockOnLCD_noSec(ChargeTimeSec);
	}
	logDebugD("Init status: ",InitiStatus,0);
	logDebugD("EnterInMenu_Status: ",EnterInMenu_Status,0);

	logDebugD("Chargestatus timer: ",ChargeStatusForTimer,0);
	logDebugD("ChargeTimeSec: ",ChargeTimeSec,0);
	logDebugD("Dischargestatus timer: ",DisChargeStatusForTimer,0);
	logDebugD("DischargeTimeSec: ",DischargeTimeSec,0);
}
void MenuChargeAdapt(Key_Pressed_t key)
{
	EnterInMenu_Status = 1;
	if (InitiStatus==0)
	{
		if (U_PS < (SettingsData.MaxVoltage+DELTA_UPS_BAT_MAX))
		{
			lcd_set_xy(0,0);
			PrintToLCD("U(ps)<U(bat.max)");
			delay_ms(2000);
			lcd_set_xy(0,0);
			PrintToLCD("UP U(ps)        ");
			delay_ms(3000);

		}
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;
		BatteryCapacityCharge = 0;
		InitiStatus = 1;
		ChargeTimeSec = 0;
	}
	if (ChargeTimeSec > ChargeDurationSec)
	{
		OFF();
		ChargeStatusForTimer = 0;

	}else
	{
		if (U_OUT> SettingsData.MaxVoltage)
		{
			OFF();
			ChargeStatusForTimer = 0;
		}else
		{
			if (U_OUT< (SettingsData.MaxVoltage-(SettingsData.MaxVoltage*SettingsData.ChargeAdapt/100))   )
			{
				charge();
				ChargeStatusForTimer = 1;
			}
		}
	}

	#define MAXITEM1 4
    OUTStateAfterPowerUpFunction();
	if (key == KEY_NEXT)
	{
		if (On_off == 0)
		{
			charge();
			ChargeStatusForTimer = 1;
		}
		else
		{
			OFF();
			ChargeStatusForTimer = 0;
		}
	}
	if (key == KEY_BACK)
	{
		CountShow--;
		if (CountShow<0) CountShow=MAXITEM1-1;
	}
	if(CountShow == 0)
	{
		lcd_set_xy(0,0);
		if (On_off ==0)
		{
			PrintToLCD(itoa_koma(U_PS,2));
			PrintToLCD("V ");
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V   ");
		}
		else
		{
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V ");
			PrintToLCD(itoa(Current));
			PrintToLCD("mA      ");
		}
		if (On_off ==0)
		{
			lcd_set_xy(7,1);
			LcdOutbyNumber(0,1);
		}
		else
			if (Blink_message_counter<=5)
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(0,1);
			}
			else
			{
					lcd_set_xy(7,1);
					LcdOutbyNumber(1,1);
			}
		if (Blink_message_counter>10)
			Blink_message_counter = 0;
	}

	if(CountShow == 1)
	{
		lcd_set_xy(0,0);
		PrintToLCD("T:");
		PrintToLCD(itoa(SettingsData.ChargeTime));
		PrintToLCD("h MaxV:");
		PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
		PrintToLCD("V   ");
	}
	if(CountShow == 2)
	{
		lcd_set_xy(0,0);
		PrintToLCD(itoa(SettingsData.ChargeAdapt));
		PrintToLCD("% ");
		PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
		PrintToLCD("V ");
		PrintToLCD(itoa_koma((SettingsData.MaxVoltage-(SettingsData.MaxVoltage*SettingsData.ChargeAdapt/100))/10,1));
		PrintToLCD("V   ");
	}
	if(CountShow == 3)
	{
		lcd_set_xy(0,0);
		PrintToLCD("C ");
		PrintToLCD(itoa(BatteryCapacityCharge/3600));
		PrintToLCD("mAH     ");
		lcd_set_xy(3,1);
		ClockOnLCD_noSec(ChargeTimeSec);
	}
	delay_ms(MENUDELAY);

	logDebugD("Init status: ",InitiStatus,0);
	logDebugD("EnterInMenu_Status: ",EnterInMenu_Status,0);

	logDebugD("Chargestatus timer: ",ChargeStatusForTimer,0);
	logDebugD("ChargeTimeSec: ",ChargeTimeSec,0);
	logDebugD("Dischargestatus timer: ",DisChargeStatusForTimer,0);
	logDebugD("DischargeTimeSec: ",DischargeTimeSec,0);

}
void MenuDisCharge(Key_Pressed_t key)
{
	EnterInMenu_Status = 1;
	if (InitiStatus==0)
	{
		InitiStatus = 1;
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;
		BatteryCapacityDischargeCurrent = 0;
		DischargeTimeSec = 0;
		BatteryLow=0;
		//discharge();
	}

	U_BatteryTmp = U_OUT;
	//low bat
	if (U_BatteryTmp >=SettingsData.LowVoltage) BatteryLow=0;
	if (U_BatteryTmp < SettingsData.LowVoltage)
	{
		if (BatteryLow == 0) U_Battery_Timer = time_sec;
		BatteryLow = 1;
	}
	if ((time_sec - U_Battery_Timer)>3 && BatteryLow !=0)//was 10
	{

		OFF();
		DisChargeStatusForTimer = 0;
	}
	#define MAXITEM2 4
	if (key == KEY_NEXT)
	{
    	if (On_off == 0)
    	{
    		discharge();
    		DisChargeStatusForTimer = 1;
    	}
    	else
    	{
    		OFF();
    		DisChargeStatusForTimer = 0;
    	}
	}
	if (key == KEY_BACK)
	{
		CountShow--;
		if (CountShow<0) CountShow=MAXITEM2-1;
	}
	if(CountShow == 0)
	{
		lcd_set_xy(0,0);
		PrintToLCD(itoa_koma(U_OUT ,2));
		PrintToLCD("V ");
		PrintToLCD(itoa(Current ));
		PrintToLCD("mA      ");

		if (On_off ==0)
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(2,1);
			}
			else
				if (Blink_message_counter<=5)
				{
					lcd_set_xy(7,1);
					LcdOutbyNumber(2,1);
				}
				else
				{
						lcd_set_xy(7,1);
						LcdOutbyNumber(3,1);
				}
			if (Blink_message_counter>10)
				Blink_message_counter = 0;
}
	if(CountShow == 1)
	{
		lcd_set_xy(0,0);
		PrintToLCD("OffWhen Vb<");
		PrintToLCD(itoa_koma(SettingsData.LowVoltage/10 ,1));
		PrintToLCD("V   ");

	}
	if(CountShow == 2)
	{
		lcd_set_xy(0,0);
		PrintToLCD("P ");
		PrintToLCD(itoa(SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue/3600 ));
		PrintToLCD("mAh     ");
		lcd_set_xy(3,1);
		ClockOnLCD_noSec(DischargeTimeSec_Previous);

	}
	if(CountShow == 3)
	{
		lcd_set_xy(0,0);
		PrintToLCD("N ");
		PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600 ));
		PrintToLCD("mAH     ");
		lcd_set_xy(3,1);
		ClockOnLCD_noSec(DischargeTimeSec);
	}
	logDebugD("Init status: ",InitiStatus,0);
	logDebugD("EnterInMenu_Status: ",EnterInMenu_Status,0);

	logDebugD("Chargestatus timer: ",ChargeStatusForTimer,0);
	logDebugD("ChargeTimeSec: ",ChargeTimeSec,0);
	logDebugD("Dischargestatus timer: ",DisChargeStatusForTimer,0);
	logDebugD("DischargeTimeSec: ",DischargeTimeSec,0);

}
void MenuTraining_WriteInLOG(uint32_t Capacity, uint32_t Time, char c  )
{
	char s[2];
	s[0] = c;
	s[1] = '\0';
	char str[17];
	char strout[17];
	char strout1[17];
	char s_clock[17];
	ClockStringNoSec(Time,s_clock);
	Merge3Strings(itoaP(Capacity/3600,str),"mAh",s_clock,strout);
	Merge2Strings(s,strout,strout1);
	WriteInLOGc(strout1,TRAINING_);
}

typedef enum ChargeDischargeStatus_e {
    CHARGEs=1,
    DISCHARGEs,
    IDLEs
} ChargeDischargeStatus_t ;
ChargeDischargeStatus_t ChargeDischargeStatus = DISCHARGEs;
int8_t CheckingCapacityCounter = 0;
void MenuCheckingCapacity(Key_Pressed_t key)
{
	if (EnterInMenu_Status == 0)
	{
		if (U_PS < (SettingsData.MaxVoltage+DELTA_UPS_BAT_MAX))
		{
			lcd_set_xy(0,0);
			PrintToLCD("U(ps)<U(bat.max)");
			delay_ms(2000);
			lcd_set_xy(0,0);
			PrintToLCD("UP U(ps)        ");
			delay_ms(3000);

		}
		EnterInMenu_Status = 1;
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;
		ChargeTimeSec = 0;
		DischargeTimeSec = 0;
		DischargeTimeSec_Previous = 0;
		BatteryCapacityCharge = 0;
		BatteryCapacityDischargeCurrent = 0;

		//Start charge activity
		ChargeStatusForTimer = 1;
		charge1();
		ChargeDischargeStatus = CHARGEs;
		lcd_set_xy(0,0);
		PrintToLCD("Testing Battery.");
		delay_ms(1000);
		CheckingCapacityCounter = 1;
	}
	if (CheckingCapacityCounter<=3)
	{
		if (Module16(Current)>5)
		{
			if (U_OUT < SettingsData.LowVoltage && ChargeDischargeStatus == DISCHARGEs  && TimerForReadyMeasurement_ms>1000)
			{
				//stop discharge action
				DisChargeStatusForTimer = 0;
				if (BatteryCapacityDischargeCurrent/3600 > 100)
				{
					MenuTraining_WriteInLOG(BatteryCapacityDischargeCurrent,DischargeTimeSec,DISCHARGE_l);
				}
				DischargeTimeSec_Previous = DischargeTimeSec;

				SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;


				//Start charge action
				ChargeStatusForTimer = 1;
				BatteryCapacityCharge = 0;
				ChargeTimeSec = 0;
				charge1();
				CheckingCapacityCounter ++;
				ChargeDischargeStatus = CHARGEs;
			}
			if (((U_OUT > SettingsData.MaxVoltage && ChargeDischargeStatus == CHARGEs  && TimerForReadyMeasurement_ms>1000) || (ChargeTimeSec > ChargeDurationSec && ChargeDischargeStatus == CHARGEs))&&CheckingCapacityCounter<3)
			{


				//stop charge activity
				ChargeStatusForTimer = 0;
				if (BatteryCapacityCharge/3600 > 100)
				{
					MenuTraining_WriteInLOG(BatteryCapacityCharge,ChargeTimeSec,CHARGE_H);
				}



				//Start discharge activity
				DisChargeStatusForTimer = 1;
				BatteryCapacityDischargeCurrent = 0;
				DischargeTimeSec = 0;
				discharge1();
				CheckingCapacityCounter ++;
				//delay_ms(100);
				ChargeDischargeStatus = DISCHARGEs;
			}
			if (((U_OUT > SettingsData.MaxVoltage && ChargeDischargeStatus == CHARGEs  && TimerForReadyMeasurement_ms>1000) || (ChargeTimeSec > ChargeDurationSec && ChargeDischargeStatus == CHARGEs))&&CheckingCapacityCounter>=3)
			{


				//stop charge activity
				OFF();
				ChargeStatusForTimer = 0;
				DisChargeStatusForTimer = 0;
				if (BatteryCapacityCharge/3600 > 100)
				{
					MenuTraining_WriteInLOG(BatteryCapacityCharge,ChargeTimeSec,CHARGE_H);
				}
				CheckingCapacityCounter ++;
				CountShow = 2;
			}
			//Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu
			#define MAX_SCREENS__MENU_CAPACITY 4
			if (key == KEY_NEXT)
			{
				CountShow++;
				if (CountShow==MAX_SCREENS__MENU_CAPACITY) CountShow=0;
			}
			if (key == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAX_SCREENS__MENU_CAPACITY-1;
			}
			if(CountShow == 0)
			{
				lcd_set_xy(0,0);
				PrintToLCD(itoa_koma(U_OUT,2));
				PrintToLCD("V ");

				PrintToLCD(itoa(Current));
				PrintToLCD("mA     ");
				lcd_set_xy(6,1);
				LcdOutbyNumber(4,1);
				if (Status_Out ==1)
				{
					lcd_set_xy(7,1);
					LcdOutbyNumber(0,1);//charge
				}
				else
				{
					lcd_set_xy(7,1);
					LcdOutbyNumber(2,1);//discharge
				}
			}
			if(CountShow == 1)
			{
				lcd_set_xy(0,0);
				PrintToLCD(itoa(SettingsData.ChargeTime));
				PrintToLCD("h ");
				PrintToLCD(itoa_koma(SettingsData.LowVoltage/10,1));
				PrintToLCD("V ");
				PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
				PrintToLCD("V     ");
			}
			if(CountShow == 2)
			{
				lcd_set_xy(0,0);
				LcdOutbyNumber(2,1);//discharge
				lcd_set_xy(1,0);
				PrintToLCD(" ");
				PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600));
				PrintToLCD("mAH      ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec);
			}

			if(CountShow == 3)
			{
				lcd_set_xy(0,0);
				LcdOutbyNumber(0,1);//charge
				lcd_set_xy(1,0);
				PrintToLCD(" ");
				PrintToLCD(itoa(BatteryCapacityCharge/3600));
				PrintToLCD("mAH      ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(ChargeTimeSec);
			}
		}
		else
		{
			if (TimerForReadyMeasurement_ms>1000)
			{
				lcd_set_xy(0,0);
				PrintToLCD("NO/BAD Battery  ");
			}
		}
	}else
	{


		#define MAX_SCREENS__MENU_CAPACITY1 4
		if (key == KEY_NEXT)
		{
			CountShow++;
			if (CountShow==MAX_SCREENS__MENU_CAPACITY1) CountShow=0;
		}
		if (key == KEY_BACK)
		{
			CountShow--;
			if (CountShow<0) CountShow=MAX_SCREENS__MENU_CAPACITY1-1;
		}
		if(CountShow == 0)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V ");

			PrintToLCD(itoa(Current));
			PrintToLCD("mA     ");
			lcd_set_xy(6,1);
			LcdOutbyNumber(4,1);
			if (Status_Out ==1)
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(0,1);//charge
			}
			else
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(2,1);//discharge
			}
		}
		if(CountShow == 1)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa(SettingsData.ChargeTime));
			PrintToLCD("h ");
			PrintToLCD(itoa_koma(SettingsData.LowVoltage/10,1));
			PrintToLCD("V ");
			PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
			PrintToLCD("V     ");
		}
		if(CountShow == 2)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(2,1);//discharge
			lcd_set_xy(1,0);
			PrintToLCD(" ");
			PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600));
			PrintToLCD("mAH      ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(DischargeTimeSec);
		}

		if(CountShow == 3)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(0,1);//charge
			lcd_set_xy(1,0);
			PrintToLCD(" ");
			PrintToLCD(itoa(BatteryCapacityCharge/3600));
			PrintToLCD("mAH      ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(ChargeTimeSec);
		}
	}
	logDebugD("EnterInMenu_Status: ",EnterInMenu_Status,0);

	logDebugD("Chargestatus timer: ",ChargeStatusForTimer,0);
	logDebugD("ChargeTimeSec: ",ChargeTimeSec,0);
	logDebugD("BatteryCapacityCharge: ",BatteryCapacityCharge,0);
	logDebugD("Dischargestatus timer: ",DisChargeStatusForTimer,0);
	logDebugD("DischargeTimeSec: ",DischargeTimeSec,0);
	logDebugD("BatteryCapacityDischargeCurrent: ",BatteryCapacityDischargeCurrent,0);

	//delay_ms(100);

}

void MenuTraining_new(Key_Pressed_t key)
{
	if (EnterInMenu_Status == 0)
	{
		if (U_PS < (SettingsData.MaxVoltage+DELTA_UPS_BAT_MAX))
		{
			lcd_set_xy(0,0);
			PrintToLCD("U(ps)<U(bat.max)");
			delay_ms(2000);
			lcd_set_xy(0,0);
			PrintToLCD("UP U(ps)        ");
			delay_ms(3000);

		}
		EnterInMenu_Status = 1;
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;
		ChargeTimeSec = 0;
		DischargeTimeSec = 0;
		DischargeTimeSec_Previous = 0;
		BatteryCapacityCharge = 0;
		BatteryCapacityDischargeCurrent = 0;

		//Start charge activity
		ChargeStatusForTimer = 1;
		charge1();
		ChargeDischargeStatus = CHARGEs;
		lcd_set_xy(0,0);
		PrintToLCD("Testing Battery.");
		delay_ms(1000);
	}

	if (Module16(Current)>5)
	{
		if (U_OUT < SettingsData.LowVoltage && ChargeDischargeStatus == DISCHARGEs  && TimerForReadyMeasurement_ms>1000)
		{
			//stop discharge action
			DisChargeStatusForTimer = 0;
			if (BatteryCapacityDischargeCurrent/3600 > 100)
			{
				MenuTraining_WriteInLOG(BatteryCapacityDischargeCurrent,DischargeTimeSec,DISCHARGE_l);
			}

			//Start charge action
			ChargeTimeSec = 0;
			BatteryCapacityCharge = 0;
			ChargeStatusForTimer = 1;
			charge1();

			//delay_ms(300);
			ChargeDischargeStatus = CHARGEs;
		}
		if ((U_OUT > SettingsData.MaxVoltage && ChargeDischargeStatus == CHARGEs  && TimerForReadyMeasurement_ms>1000) || (ChargeTimeSec > ChargeDurationSec && ChargeDischargeStatus == CHARGEs))
		{


			//stop charge activity
			ChargeStatusForTimer = 0;
			if (BatteryCapacityCharge/3600 > 100)
			{
				MenuTraining_WriteInLOG(BatteryCapacityCharge,ChargeTimeSec,CHARGE_H);
			}
			DischargeTimeSec_Previous = DischargeTimeSec;
			SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;

			//Start discharge activity
			DischargeTimeSec = 0;
			BatteryCapacityDischargeCurrent = 0;
			DisChargeStatusForTimer = 1;
			discharge1();
			//delay_ms(100);
			ChargeDischargeStatus = DISCHARGEs;
		}

		//Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu
		#define MAX_SCREENS__MENU_TRAINING 5
		if (key == KEY_NEXT)
		{
			CountShow++;
			if (CountShow==MAX_SCREENS__MENU_TRAINING) CountShow=0;
		}
		if (key == KEY_BACK)
		{
			CountShow--;
			if (CountShow<0) CountShow=MAX_SCREENS__MENU_TRAINING-1;
		}
		if(CountShow == 0)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V ");

			PrintToLCD(itoa(Current));
			PrintToLCD("mA     ");
			lcd_set_xy(6,1);
			LcdOutbyNumber(4,1);
			if (Status_Out ==1)
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(0,1);//charge
			}
			else
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(2,1);//discharge
			}
		}
		if(CountShow == 1)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa(SettingsData.ChargeTime));
			PrintToLCD("h ");
			PrintToLCD(itoa_koma(SettingsData.LowVoltage/10,1));
			PrintToLCD("V ");
			PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
			PrintToLCD("V     ");
		}
		if(CountShow == 2)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(2,1);//discharge
			lcd_set_xy(1,0);
			PrintToLCD(" ");
			PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600));
			PrintToLCD("mAH      ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(DischargeTimeSec);
		}
		if(CountShow == 3)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(0,1);//charge
			lcd_set_xy(1,0);
			PrintToLCD(" ");
			PrintToLCD(itoa(BatteryCapacityCharge/3600));
			PrintToLCD("mAH      ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(ChargeTimeSec);
		}
		if(CountShow == 4)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(2,1);//discharge
			lcd_set_xy(1,0);
			PrintToLCD("P ");
			PrintToLCD(itoa(SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue/3600));
			PrintToLCD("mAH     ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(DischargeTimeSec_Previous);
		}
	}
	else
	{
		if (TimerForReadyMeasurement_ms>1000)
		{
			lcd_set_xy(0,0);
			PrintToLCD("NO/BAD Battery  ");
		}
	}
	logDebugD("EnterInMenu_Status: ",EnterInMenu_Status,0);

	logDebugD("Chargestatus timer: ",ChargeStatusForTimer,0);
	logDebugD("ChargeTimeSec: ",ChargeTimeSec,0);
	logDebugD("BatteryCapacityCharge: ",BatteryCapacityCharge,0);
	logDebugD("Dischargestatus timer: ",DisChargeStatusForTimer,0);
	logDebugD("DischargeTimeSec: ",DischargeTimeSec,0);
	logDebugD("BatteryCapacityDischargeCurrent: ",BatteryCapacityDischargeCurrent,0);

}

void MenuBatterySimilation(Key_Pressed_t key)
{
	EnterInMenu_Status = 1;
	lcd_set_xy(0,0);
	if (On_off == 0)
	{
		PrintToLCD(itoa_koma(U_PS,2));
		PrintToLCD("V  ");
		lcd_set_xy(7,0);
		PrintToLCD(itoa(Current));
		PrintToLCD("mA   ");
		lcd_set_xy(5,1);
		PrintToLCD("OFF ");
	}
	else
	{
		PrintToLCD(itoa_koma(U_OUT,2));
		PrintToLCD("V  ");
		lcd_set_xy(7,0);
		PrintToLCD(itoa(Current));
		PrintToLCD("mA    ");
		lcd_set_xy(6,1);
		PrintToLCD("ON ");
	}
    if (key == KEY_NEXT)
    {
    	if (On_off == 0)
    	{
    		OUT_ON();
    		LOAD_ON();
    	}
    	else
    	{
    		OUT_OFF();
    		LOAD_OFF();
    	}
    }
}


void MenuSwing(Key_Pressed_t key)
{
   	lcd_set_xy(0,0);
       	EnterInMenu_Status = 1;
       	if (InitiStatus==0)
        {
       		InitiStatus=1;
       		Start_Timer_sec();
        }

       if (Timer_Sec<=SettingsData.Swing_Chrg_time)
       {
    	   charge1();
           if (U_OUT>SettingsData.MaxVoltage && TimerForReadyMeasurement_ms>400)
           {
        	   ReStart_Timer_sec();
               Timer_Sec  = Timer_Sec+ SettingsData.Swing_Chrg_time;
           }
       }
       if (Timer_Sec > SettingsData.Swing_Chrg_time)
       {
           discharge1();
           if (U_OUT<SettingsData.LowVoltage && TimerForReadyMeasurement_ms>400)
           {
        	   ReStart_Timer_sec();
            }
        }
       if (Timer_Sec > (SettingsData.Swing_Chrg_time+SettingsData.Swing_DChrg_time))
       {
    	   ReStart_Timer_sec();
       }

		#define MAXITEM6 3
		if (key == KEY_NEXT)
		{
			CountShow++;
			if (CountShow==MAXITEM6) CountShow=0;
		}
		if (key == KEY_BACK)
		{
			CountShow--;
			if (CountShow<0) CountShow=MAXITEM6-1;
		}
		if(CountShow == 0)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa_koma(U_OUT,2));
			PrintToLCD("V ");
			PrintToLCD(itoa(Current));
			PrintToLCD("mA     ");

			lcd_set_xy(6,1);
			LcdOutbyNumber(5,1);
			if (Status_Out ==1)
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(0,1);//charge
			}
			else
			{
				lcd_set_xy(7,1);
				LcdOutbyNumber(2,1);//discharge
			}
		}
		if(CountShow == 1)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa(SettingsData.ChargeTime));
			PrintToLCD("h ");
			PrintToLCD(itoa_koma(SettingsData.LowVoltage/10,1));
			PrintToLCD("V ");
			PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
			PrintToLCD("V     ");

		}
		if(CountShow == 2)
		{
			lcd_set_xy(0,0);
			PrintToLCD(itoa(SettingsData.Swing_Chrg_time));
			PrintToLCD("s ");

			PrintToLCD(itoa(SettingsData.Swing_DChrg_time));
			PrintToLCD("s      ");


			lcd_set_xy(3,1);
			ClockOnLCD_noSec(DischargeTimeSec);
		}
}
void MenuLog_Enter()
{
	i_LogItems = 0;
}
void MenuLog(Key_Pressed_t key)
{

	if (key == KEY_NEXT)
	{
		i_LogItems++;
		if (i_LogItems>=LoggingData.RecordsQuantity) i_LogItems=0;
	}
	if (key == KEY_BACK)
	{
		i_LogItems--;
		if (i_LogItems<0) i_LogItems=LoggingData.RecordsQuantity-1;
		if (i_LogItems<0) i_LogItems=0;
	}
	lcd_set_xy(0,0);
	PrintToLCD(LoggingData.Records[LoggingData.RecordsQuantity - i_LogItems-1]);
	logDebugD("i_LogItems: ",i_LogItems,0);
	logDebugD("MRecordsQuantity: ",LoggingData.RecordsQuantity,0);
}
void MenuDIAGNOSTIC(Key_Pressed_t key)
{
	EnterInMenu_Status=1;

	//discharge();
	#define MAXITEM 8
	if (key == KEY_NEXT)
	{
		CountShow1++;
		if (CountShow1==MAXITEM) CountShow1=0;

	}
	if (key == KEY_BACK)
	{
		CountShow1--;
		if (CountShow1<0) CountShow1=MAXITEM-1;
	}
	if(CountShow1 == 0)
	{
		OFF();
		lcd_set_xy(0,0);
		//PrintToLCD(" ");
		PrintToLCD("Uin=");
		PrintToLCD(		itoa_koma(U_IN/10,1));
		PrintToLCD(" Uc=");
		PrintToLCD(itoa_koma(U_Controller,2));
		PrintToLCD("    ");

	}
	if(CountShow1 == 1)
	{
		OFF();
		lcd_set_xy(0,0);
		PrintToLCD("Ups ");
		PrintToLCD(itoa_koma(U_PS,2));
		PrintToLCD("V ");
		PrintToLCD(itoa((RegularConvData[3])));
		PrintToLCD("      ");
	}
	if(CountShow1 == 2)
	{
		OUT_ON();
		lcd_set_xy(0,0);
		PrintToLCD("Uout ");
		PrintToLCD(itoa_koma(U_OUT,2));
		PrintToLCD("V ");
		PrintToLCD(itoa((RegularConvData[2])));
		PrintToLCD("      ");
	}
	if(CountShow1 == 3)
	{
		LOAD_OFF();
		OUT_ON();
		lcd_set_xy(0,0);
		PrintToLCD("Iout ");
		PrintToLCD(itoa(Current));
		PrintToLCD("mA ");
		PrintToLCD(itoa((RegularConvData[1])));
		PrintToLCD("      ");
	}
	if(CountShow1 == 4)
	{
		OUT_OFF();
		LOAD_ON();
		lcd_set_xy(0,0);
		PrintToLCD("I(l) ");
		PrintToLCD(itoa(Current_load));
		PrintToLCD("mA ");
		PrintToLCD(itoa(RegularConvData[0]));
		PrintToLCD("       ");
	}
	if(CountShow1 == 5)
	{
		LOAD_OFF();
		OUT_ON();
		CalibrationData.ResistanceComp_MOSFET= (int32_t)(U_PS-U_OUT_ForSetResistance)*10000/Current;
		lcd_set_xy(0,0);
		PrintToLCD("R=");
		PrintToLCD(itoa(CalibrationData.ResistanceComp_Ishunt_Wires));
		PrintToLCD("mOm ");
		PrintToLCD(itoa(CalibrationData.ResistanceComp_MOSFET));
		PrintToLCD("mOm    ");
		logDebugD("I: ",Current,0);
		logDebugD("U_PS: ",U_PS,2);
		logDebugD("U out: ",U_OUT_ForSetResistance,2);
	}
	if(CountShow1 == 6)
	{
		OFF();
		lcd_set_xy(0,0);
		char s[17];
		PrintToLCD(ClockStringWithSec(time_sec,s));
		PrintToLCD("        ");
	}

	if(CountShow1 == 7)
	{

		LOAD_OFF();
		OUT_ON();
		lcd_set_xy(0,0);
		PrintToLCD("T=");
		PrintToLCD(itoa(GetTemperature(Rt)));
		PrintToLCD("C   ");
		PrintToLCD(itoa(RegularConvData[5]));
		PrintToLCD("    ");
	}
}
void MenuCalibration_CURRENT_Out_to_0(Key_Pressed_t key)
{
	lcd_set_xy(0,0);
	PrintToLCD(itoa(Current));
	PrintToLCD("mA >> set 0   ");
	if (key == KEY_NEXT)
	{
		CalibrationData.Calibration0ValueForCurrent = Current_Out;
		CalibrationWriteToFlash_CRC();
	}
}

void MenuCalibration_CURRENT_Load_to_0(Key_Pressed_t key)
{
	lcd_set_xy(0,0);
	PrintToLCD(	itoa(Current));
	PrintToLCD("mA >> set 0   ");
	if (key == KEY_NEXT)
	{
		CalibrationData.Calibration0ValueForCurrent1 = Current_load;
		CalibrationWriteToFlash_CRC();
	}
}
void MenuCalibration_CURRENT_Out(Key_Pressed_t key)
{
	EnterInMenu_Status=1;
	OUT_ON();
	if (key == KEY_NEXT) CalibrationData.CalibrationValueForCurrent++;
	if (key == KEY_BACK) CalibrationData.CalibrationValueForCurrent--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(CalibrationData.CalibrationValueForCurrent));
	PrintToLCD("   ");
	lcd_set_xy(0,1);
	PrintToLCD(itoa(Current));
	PrintToLCD("mA  ");
}
void MenuCalibration_CURRENT_Load(Key_Pressed_t key)
{
	EnterInMenu_Status=1;
	discharge();
	if (key == KEY_NEXT) CalibrationData.CalibrationValueForCurrent1++;
	if (key == KEY_BACK) CalibrationData.CalibrationValueForCurrent1--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(CalibrationData.CalibrationValueForCurrent1));
	PrintToLCD("   ");
	lcd_set_xy(0,1);
	PrintToLCD(itoa(Current));
	PrintToLCD("mA  ");
}
void MenuCalibration_VoltagePS(Key_Pressed_t key)
{
	EnterInMenu_Status=1;
	OUT_ON();
	if (key == KEY_NEXT) CalibrationData.CalibrationValueForVoltage++;
	if (key == KEY_BACK) CalibrationData.CalibrationValueForVoltage--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(CalibrationData.CalibrationValueForVoltage));
	PrintToLCD("   ");
	lcd_set_xy(0,1);
	PrintToLCD(itoa_koma(U_PS,2));
	PrintToLCD("V  ");
}
void MenuCalibration_VoltageOut(Key_Pressed_t key)
{
	EnterInMenu_Status=1;
	OUT_ON();
	if (key == KEY_NEXT) CalibrationData.CalibrationValueForVoltage1++;
	if (key == KEY_BACK) CalibrationData.CalibrationValueForVoltage1--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(CalibrationData.CalibrationValueForVoltage1));
	PrintToLCD("   ");
	lcd_set_xy(0,1);
	PrintToLCD(itoa_koma(U_OUT,2));
	PrintToLCD("V  ");
}
void MenuCalibration_VoltageIn(Key_Pressed_t key)
{
	EnterInMenu_Status=1;
	OUT_ON();
	if (key == KEY_NEXT) CalibrationData.CalibrationValueForVoltage2++;
	if (key == KEY_BACK) CalibrationData.CalibrationValueForVoltage2--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(CalibrationData.CalibrationValueForVoltage2));
	PrintToLCD("   ");
	lcd_set_xy(0,1);
	PrintToLCD(itoa_koma(U_IN,2));
	PrintToLCD("V  ");
}

void MenuCalibration_Resist_Comp_5V1A(Key_Pressed_t key)
{
		static uint16_t ResistanceComp_Voltage = 470;
		int16_t Delta;

		EnterInMenu_Status=1;
		OUT_ON();
		if (key == KEY_NEXT) ResistanceComp_Voltage++;
		if (key == KEY_BACK) ResistanceComp_Voltage--;

		lcd_set_xy(0,0);
		PrintToLCD(itoa_koma(ResistanceComp_Voltage,2));
		PrintToLCD("V ");
		Delta = U_OUT_ForSetResistance - ResistanceComp_Voltage;
		if(Delta<=0) Delta = 0;
		if (Current <= 0) CalibrationData.ResistanceComp_Ishunt_Wires = 70;
		else CalibrationData.ResistanceComp_Ishunt_Wires = Delta*10000/Current;
		PrintToLCD(itoa(CalibrationData.ResistanceComp_Ishunt_Wires));
		PrintToLCD("mOm       ");


		Delta = U_PS - U_OUT_ForSetResistance;
		if(Delta<=0) Delta = 0;
		if (Current <= 0) CalibrationData.ResistanceComp_MOSFET = 10;
		else CalibrationData.ResistanceComp_MOSFET = Delta*10000/Current;


}

void MenuCalibration_BackToFactory(Key_Pressed_t key)
{
	lcd_set_xy(0,0);
	PrintToLCD("Factory-press >>");
	if (key == KEY_NEXT)
	{
		uint8_t EEpromReadStatus;
		FactoryWriteToFlash_CRC();
		EEpromReadStatus = ReadFromFlash();
		lcd_set_xy(0,0);
		PrintToLCD("In Proccess ....");
		delay_ms(2000);
		lcd_set_xy(0,0);
		if (EEpromReadStatus == 1)
		{
			PrintToLCD("Factory are OK  ");
			WriteInLOG("Factory are OK");
			delay_ms(2000);
		    BatteryCapacityDischargeCurrentAfterPOwerUp = SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent;
		    ChargeDurationSec = SettingsData.ChargeTime*3600;
		    SelectedOptionValue = SettingsData.Option1;
		    SelectedOptionValue1 = SettingsData.Option2;
		}
		else
		{
			PrintToLCD("EEprom read FAIL");
			delay_ms(4000);

		}
	}
}

void MenuSettingsChargeTime_Enter(void)
{

}
void MenuSettingsChargeTime(Key_Pressed_t key)
{
	if (key == KEY_NEXT) SettingsData.ChargeTime++;
	if (key == KEY_BACK) SettingsData.ChargeTime--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(SettingsData.ChargeTime));
	PrintToLCD("h   ");
    ChargeDurationSec = SettingsData.ChargeTime*3600;
}
int8_t CountShowBT = 0;
#define MAXITEM_BT 7
void MenuSettingsBatteryType(Key_Pressed_t key)
{
	if (key == KEY_NEXT)
	{
		CountShowBT++;
		if (CountShowBT==MAXITEM_BT) CountShowBT=0;
	}
	if (key == KEY_BACK)
	{
		CountShowBT--;
		if (CountShowBT<0) CountShowBT=MAXITEM_BT-1;
	}
	if(CountShowBT == 0)
	{
		lcd_set_xy(0,0);
		PrintToLCD("12V Pb Battery  ");
	}
	if(CountShowBT == 1)
	{
		lcd_set_xy(0,0);
		PrintToLCD("Li ion  Battery  ");
	}
	if(CountShowBT == 2)
	{
		lcd_set_xy(0,0);
		PrintToLCD("LiFePo4 Battery ");
	}
	if(CountShowBT == 3)
	{
		lcd_set_xy(0,0);
		PrintToLCD("LiTo Battery   ");
	}
	if(CountShowBT == 4)
	{
		lcd_set_xy(0,0);
		PrintToLCD("NIMh 9.6V Bat.  ");
	}
	if(CountShowBT == 5)
	{
		lcd_set_xy(0,0);
		PrintToLCD("Min 1V  MAX 20V");
	}
	if(CountShowBT == 6)
	{
		lcd_set_xy(0,0);
		PrintToLCD("   OK to Exit  ");
	}
}
void MenuSettingsBatteryType_Enter(Key_Pressed_t key)
{
	if(CountShowBT == 0)
	{
		lcd_set_xy(0,0);
		PrintToLCD(" 10.8V  14.6V   ");
		SettingsData.LowVoltage = 1080;
		SettingsData.MaxVoltage = 1460;
	}
	if(CountShowBT == 1)
	{
		lcd_set_xy(0,0);
		PrintToLCD("  3.0V   4.2V   ");
		SettingsData.LowVoltage = 300;
		SettingsData.MaxVoltage = 420;
	}
	if(CountShowBT == 2)
	{
		lcd_set_xy(0,0);
		PrintToLCD("  2.8V   3.7V   ");
		SettingsData.LowVoltage = 280;
		SettingsData.MaxVoltage = 370;
	}
	if(CountShowBT == 3)
	{
		lcd_set_xy(0,0);
		PrintToLCD("  1.6V   2.8V   ");
		SettingsData.LowVoltage = 160;
		SettingsData.MaxVoltage = 280;
	}
	if(CountShowBT == 4)
	{
		lcd_set_xy(0,0);
		PrintToLCD("   8V  13.6V    ");
		SettingsData.LowVoltage = 800;
		SettingsData.MaxVoltage = 1360;
	}

	if(CountShowBT == 5)
	{
		lcd_set_xy(0,0);
		PrintToLCD("  1V    20V     ");
		SettingsData.LowVoltage = 100;
		SettingsData.MaxVoltage = 2000;
	}
	if(CountShowBT == 6)
	{
		CountShowBT = 0;
	}

	SettingsWriteToFlash_CRC();
	//WriteInLOG("Seet");
    delay_ms(600);
}
void MenuSettingsLowVolt_Enter(void)
{

}
void MenuSettingsLowVolt(Key_Pressed_t key)
{
	if (key == KEY_NEXT) SettingsData.LowVoltage = SettingsData.LowVoltage + 10;
	if (key == KEY_BACK) SettingsData.LowVoltage = SettingsData.LowVoltage - 10;

	lcd_set_xy(0,0);
	PrintToLCD(itoa_koma(SettingsData.LowVoltage/10,1));
	PrintToLCD("V   ");
}
void MenuSettingsMaxVolt_Enter(void)
{

}
void MenuSettingsMaxVolt(Key_Pressed_t key)
{
	if (key == KEY_NEXT) SettingsData.MaxVoltage = SettingsData.MaxVoltage + 10;
	if (key == KEY_BACK) SettingsData.MaxVoltage = SettingsData.MaxVoltage - 10;

	lcd_set_xy(0,0);
	PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
	PrintToLCD("V   ");

}
void MenuSettingsSwngChrgTime_Enter(void)
{

}
void MenuSettingsSwngChrgTime(Key_Pressed_t key)
{
	if (key == KEY_NEXT)  SettingsData.Swing_Chrg_time++;
	if (key == KEY_BACK)  SettingsData.Swing_Chrg_time--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(SettingsData.Swing_Chrg_time));
	PrintToLCD(" sec.   ");
}
void MenuSettingsSwngDChrgTime_Enter(void)
{

}
void MenuSettingsSwngDChrgTime(Key_Pressed_t key)
{
	if (key == KEY_NEXT)  SettingsData.Swing_DChrg_time++;
	if (key == KEY_BACK)  SettingsData.Swing_DChrg_time--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(SettingsData.Swing_DChrg_time));
	PrintToLCD(" sec.   ");
}
void MenuSettingsChargeAddapt_Enter(void)
{

}
void MenuSettingsChargeAddapt(Key_Pressed_t key)
{
	if (key == KEY_NEXT)  SettingsData.ChargeAdapt++;
	if (key == KEY_BACK)  SettingsData.ChargeAdapt--;

	lcd_set_xy(0,0);
	PrintToLCD(itoa(SettingsData.ChargeAdapt));
	PrintToLCD("% ");
	PrintToLCD(itoa_koma(SettingsData.MaxVoltage/10,1));
	PrintToLCD("V ");
	PrintToLCD(itoa_koma((SettingsData.MaxVoltage-(SettingsData.MaxVoltage * SettingsData.ChargeAdapt/100))/10,1));
	PrintToLCD("V   ");
}
void SelectedOption(void)
{
	if (SelectedOptionValue == SettingsData.Option1)
	{
		lcd_set_xy(7,1);
		lcd_send(255,DATA);
	}
}

void SelectedOption1(void)
{
	if (SelectedOptionValue1 == SettingsData.Option2)
	{
		lcd_set_xy(7,1);
		lcd_send(255,DATA);
	}
}



#define QUANTITY_OPTIONS 7
void MenuSettingsSaveMenuPosWhenOFF(Key_Pressed_t key)
{
	lcd_set_xy(0,0);
	if (key == KEY_NEXT)
	{
		SettingsData.Option1++;
		if (SettingsData.Option1==QUANTITY_OPTIONS+1) SettingsData.Option1=1;
	}
	if (key == KEY_BACK)
	{
		SettingsData.Option1--;
		if (SettingsData.Option1==0) SettingsData.Option1 = QUANTITY_OPTIONS;
	}

	switch (SettingsData.Option1)
	{
		case 1:
			PrintToLCD("PowerSupply     ");
			SelectedOption();
			break;
		case 2:
			PrintToLCD("Load Menu       ");
			SelectedOption();
			break;
		case 3:
			PrintToLCD("Charge CC CV    ");
			SelectedOption();
			break;
		case 4:
			PrintToLCD("Charge addapt   ");
			SelectedOption();

			break;
		case 5:
			PrintToLCD("DisCharge       ");
			SelectedOption();
			break;
		case 6:
			PrintToLCD("Training        ");
			SelectedOption();
			break;
		case 7:
			PrintToLCD("Swing           ");
			SelectedOption();
			break;
		default:
			SettingsData.Option1 =1;
			break;
	}
}
#define QUANTITY_OPTIONS1 3
void MenuSettingsOutAfterPowerUp(Key_Pressed_t key)
{
	lcd_set_xy(0,0);
	if (key == KEY_NEXT)
	{
		SettingsData.Option2++;
		if (SettingsData.Option2==QUANTITY_OPTIONS1+1) SettingsData.Option2=1;
	}
	if (key == KEY_BACK)
	{
		SettingsData.Option2--;
		if (SettingsData.Option2==0) SettingsData.Option2 = QUANTITY_OPTIONS1;
	}

	switch (SettingsData.Option2)
	{
		case 1:
			PrintToLCD("OFF             ");
			SelectedOption1();
			break;
		case 2:
			PrintToLCD("ON              ");
			SelectedOption1();
			break;
		case 3:
			PrintToLCD("Previous        ");
			SelectedOption1();
			break;

		default:
			SettingsData.Option2 =1;
			break;
	}
}

void MenuCalibrationWriteToFlash_Enter(Key_Pressed_t key)
{
	CalibrationWriteToFlash_CRC();
	WriteInLOG("Calibration");
}

void MenuSettingsWriteToFlash_Enter(Key_Pressed_t key)
{
	SettingsWriteToFlash_CRC();
}

void MenuDischarge_Enter(Key_Pressed_t key)
{
	if (BatteryCapacityDischargeCurrent/3600>10)
	{
		char str[17];
		char strout[17];
		char s_clock[17];
		ClockStringNoSec(DischargeTimeSec,s_clock);
		Merge3Strings(itoaP(BatteryCapacityDischargeCurrent/3600,str),"mAh",s_clock,strout);
		WriteInLOGc(strout,DISCHARGE_l);
	}

}

void MenuCharge_Enter(Key_Pressed_t key)
{
	logDebugD("charge=",BatteryCapacityCharge,0);
	if (BatteryCapacityCharge/3600>10)
	{
		char str[17];
		char strout[17];
		char s_clock[17];
		ClockStringNoSec(ChargeTimeSec,s_clock);
		Merge3Strings(itoaP(BatteryCapacityCharge/3600,str),"mAh",s_clock,strout);
		WriteInLOGc(strout,CHARGE_H);
		logDebug(strout);
	}

}

void MenuOption_Enter(Key_Pressed_t key)
{
	lcd_set_xy(7,1);
	lcd_send(255,DATA);
	SettingsWriteToFlash_CRC();
	SelectedOptionValue = SettingsData.Option1;
	delay_ms(200);
}

void MenuOption_Enter1(Key_Pressed_t key)
{
	lcd_set_xy(7,1);
	lcd_send(255,DATA);
	SettingsWriteToFlash_CRC();
	SelectedOptionValue1 = SettingsData.Option2;
	delay_ms(200);
}

int16_t comp = 0;
void BUT_Debrief(void)
{
	Key_Pressed_t key;

	if (!LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4))
		key = KEY_OK;
	else if (!LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_5))
		key = KEY_NEXT;
	else if (!LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_6))
		key = KEY_BACK;
	else if (!LL_GPIO_IsInputPinSet(GPIOB,LL_GPIO_PIN_7))
		key = KEY_UP;
	else
	{
		key = 0;
	}

	if (key)
	{
		if (comp > THRESHOLD2)
		{
			comp = THRESHOLD2 - 40;
			pressedKey = key;
			return;
		}
		else comp++;

		if (comp == THRESHOLD)
			{
			pressedKey = key;
			return;
		}
	}
	else comp=0;
}

Key_Pressed_t BUT_GetKey(void)
{
	if (pressedKey) PowerOffTimesec=0;
	Key_Pressed_t key = pressedKey;
	pressedKey = 0;
	return key;
}

void Start_Timer_sec()
{
	Timer_Sec = 0;
	Status_Timer_Sec = 1;
}
void ReStart_Timer_sec()
{
	Timer_Sec = 0;
	Status_Timer_Sec = 1;
}
void Stop_Timer_sec()
{

	Status_Timer_Sec = 0;

}

void ClockOnLCD (uint32_t time)
{
	char *string;
	//(" ",1);
	string = itoa(time/3600);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

	lcd_out(":",1);
	string = itoa((time/60)%60);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

	lcd_out(":",1);
	string = itoa(time%60);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

}
void ClockOnLCD_noSec (uint32_t time)
{
	char *string;
	//(" ",1);
	string = itoa(time/3600);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

	lcd_out(":",1);
	string = itoa((time/60)%60);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);


}
void All_OUT_OFF_When_Power_OFF()
{
	static uint8_t EEpromWrite_status = 1;
	if ( (U_IN < VOLTAGE_OFF_SYSTEM) && (time_sec>6))
	{
		//logDebug("<");
		if ((GPIOB->IDR & 0x01)==1)//if Out on
		{
			SaveDataWhenPowerOff.OutState = 1;
		}
		else
		{
			SaveDataWhenPowerOff.OutState = 0;
		}
		OFF();
		if (EEpromWrite_status == 0)
		{

			EEpromWrite_status = 1;
			SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent = BatteryCapacityDischargeCurrent;
			DataWhenPowerOffWriteToFlash_CRC();
			logDebugD("dc: ",SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent,2);
			char str[17];
			char strout[17];
			char s_clock[17];
			if (BatteryCapacityDischargeCurrent/3600>10)
			{
				ClockStringNoSec(DischargeTimeSec,s_clock);
				Merge3Strings(itoaP(SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent/3600,str),"mAh",s_clock,strout);
				WriteInLOGc(strout,DISCHARGE_l);
			}
			WriteInLOG(Merge2Strings("PowerOFF ",itoa_komaP(U_IN/10,str,1),strout));
			uint8_t i=0;
			for (i = 0; i<5; i++)
			{
				logDebugD("U off(10msec): ",U_IN,2);
				delay_ms(10);
			}

		}
	}
	else
	{
		EEpromWrite_status = 0;
	}
}
void charge()
{
	LOAD_OFF();
	OUT_ON();
	delay_ms(50);
}

void discharge()
{
	OUT_OFF();
	LOAD_ON();
	delay_ms(50);

}
int8_t ChargeDischargeState = 0;
void charge1()//ChargeDischargeState = 1
{
	if (ChargeDischargeState !=1)
	{
		ChargeDischargeState = 1;
		LOAD_OFF();
		OUT_ON();
		TimerForReadyMeasurement_ms = 0;
		delay_ms(50);
		logDebug("Charge");
	}
}

void discharge1()
{
	if (ChargeDischargeState !=2)
	{
		ChargeDischargeState = 2;
		OUT_OFF();
		LOAD_ON();
		TimerForReadyMeasurement_ms = 0;
		delay_ms(50);
		logDebug("DIsCharge");
	}
}
void OFF()
{
	LOAD_OFF();
	OUT_OFF();
}
/*
GPIOB->BSRR =  GPIO_BSRR_BS0;//Diode 1 OUT ON//OFF
GPIOA->BSRR =  GPIO_BSRR_BS8;//Diode 2 Load on/OFF

GPIOA->BSRR =  GPIO_BSRR_BS1;//Fan
GPIOA->BSRR =  GPIO_BSRR_BS6;//ON-OFF OUT
GPIOA->BSRR =  GPIO_BSRR_BS4; //load
GPIOA->BSRR =  GPIO_BSRR_BS5;//load
*/

void OUT_OFF()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	GPIOB->BSRR =  GPIO_BSRR_BR0;//ON-OFF OUT
	//GPIOB->BSRR =  GPIO_BSRR_BR1; //load1
	GPIOA->BSRR =  GPIO_BSRR_BR8;//led out on/off
	On_off = 0;
	Status_Out = 0;
}
void OUT_ON()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	GPIOB->BSRR =  GPIO_BSRR_BS0;//ON-OFF OUT
	//GPIOB->BSRR =  GPIO_BSRR_BR1; //load1
	On_off = 1;
	Status_Out = 1;
	GPIOA->BSRR =  GPIO_BSRR_BS8;//led out on/off
}

void LOAD_ON()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//ON-OFF OUT
	GPIOB->BSRR =  GPIO_BSRR_BS1; //load1
	On_off = 1;
	Status_Load = 1;
	GPIOA->BSRR =  GPIO_BSRR_BS11;//led load on/off
}

void LOAD_OFF()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//ON-OFF OUT
	GPIOB->BSRR =  GPIO_BSRR_BR1; //load1
	On_off = 0;
	Status_Load = 0;
	GPIOA->BSRR =  GPIO_BSRR_BR11;//led load on/off
}
void LOAD_ON_OFF_Toggle()
{
	if (Status_Load == 0)
		LOAD_ON();
	else LOAD_OFF();
}
void OUT_ON_OFF_Toggle()
{
	if (Status_Out ==0)
		OUT_ON();
	else OUT_OFF();
}

void SysTick_Callback()//1 mc
{
	BUT_Debrief();

	TimerForReadyMeasurement_ms++;

	if (Count10mSecond >= 5)
	{
		Count10mSecond = 0;
		//f1 = SysTick->VAL;
		adc_func();
		All_OUT_OFF_When_Power_OFF();
		//f2 = SysTick->VAL;
	}

	if (Count100mSecond >= 100)
	{
		Count100mSecond = 0;
	}

	if (Count1000mSecond >= 1000)
	{
		Count1000mSecond = 0;
		if (Current < 2)
			BatteryCapacityDischargeCurrent = BatteryCapacityDischargeCurrent + Module16(Current);
		if (Current > 2)
			BatteryCapacityCharge = BatteryCapacityCharge + Module16(Current);
		if (ChargeStatusForTimer == 1)
			ChargeTimeSec++;
		if (DisChargeStatusForTimer == 1)
			DischargeTimeSec++;
		//Global timer
		if (Status_Timer_Sec == 1)
			Timer_Sec++;
		time_sec++;

		if (time_sec%2==0) GPIOA->BSRR =  GPIO_BSRR_BS15;
		else GPIOA->BSRR =  GPIO_BSRR_BR15;
	}
	Count10mSecond++;
	Count100mSecond++;
	Count1000mSecond++;
}


void adc_func()
{
	//0  I L
	//1  I out
	//2 U out
	//3 U PS
	//4 U In
	//5 T
	//6 temp
	//7 vref
	volatile int32_t Ut = 0;
	volatile int32_t It = 0;
	volatile int32_t Ut_m = 0;
	volatile int32_t It_m = 0;
	#define NUM_READ 32
	//LL_DMA_DisableChannel(DMA1,LL_DMA_CHANNEL_1);


	U_Controller = 491520 / RegularConvData[7];// Uref V/10;  1200 * 4096/ChVref
	Ut= (RegularConvData[3] * CalibrationData.CalibrationValueForVoltage) / RegularConvData[7];
	Ut_m = middle_of_3Umax1(Ut);
	SumU1 =SumU1 + RunningAverageU1(Ut_m);
	SumU1Counter ++;
	if (SumU1Counter >= NUM_READ)
	{
		U_PS = SumU1/NUM_READ;
		SumU1Counter = 0;
		SumU1 = 0;
		if (U_PS < 3) U_PS = 0;
	}

	Ut = (RegularConvData[2] * CalibrationData.CalibrationValueForVoltage1) / RegularConvData[7];
	Ut_m = middle_of_3Umax2(Ut);
	SumU2 = SumU2 + RunningAverageU2(Ut_m);
	SumU2Counter ++;


	Ut = (RegularConvData[4] * CalibrationData.CalibrationValueForVoltage2) / RegularConvData[7];
	U_IN = middle_of_3Umax3(Ut);


	It = (RegularConvData[1] * CalibrationData.CalibrationValueForCurrent*10) / RegularConvData[7] ;//  Current A/10
	It_m = middle_of_3Imax1(It);
	SumI1 =SumI1 + RunningAverageI1(It_m);
	SumI1Counter ++;
	if (SumI1Counter >= NUM_READ)
	{
		Current_Out = SumI1/NUM_READ;
		SumI1Counter = 0;
		SumI1 = 0;
	}

	It= (RegularConvData[0] * CalibrationData.CalibrationValueForCurrent1*10) / RegularConvData[7] ;//  Current A/10
	It_m = middle_of_3Imax2(It);
	SumI2 =SumI2 + RunningAverageI2(It_m);
	SumI2Counter ++;
	if (SumI2Counter >= NUM_READ)
	{
		Current_load = SumI2/NUM_READ;
		SumI2Counter = 0;
		SumI2 = 0;
	}
	if ((GPIOB->IDR & 0x02)==0x02)//if load on
	{
		Current =(int32_t)(Current_load-CalibrationData.Calibration0ValueForCurrent1)*(-1) ;//2745;
	}else
	{
		Current = (Current_Out-CalibrationData.Calibration0ValueForCurrent)/1 ;//2745;
	}
	if (SumU2Counter >= NUM_READ)
	{

		U_OUTtmp = SumU2/NUM_READ;
		SumU2 = 0;
		SumU2Counter = 0;

		U_OUT_ForSetResistance = U_OUTtmp;
		if (U_OUTtmp<3) U_OUTtmp = 0;

		if (Current>=0)
		{
			U_OUTtmp = U_OUTtmp - (int32_t)CalibrationData.ResistanceComp_Ishunt_Wires*Current/10000;
		}
		else
		{
			U_OUTtmp = U_OUTtmp - (int32_t)CalibrationData.ResistanceComp_Ishunt_Wires*Current/10000;
		}
		if (U_OUTtmp<3)
			U_OUTtmp=0;
		U_OUT = U_OUTtmp;
	}
	 //LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_1);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
  */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  InitLCD();
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);//SystemCoreClock/1000 - 1mc
	GPIOA->BSRR =  GPIO_BSRR_BS15;//Led on Board ON
	OFF();
	LoggingData.RecordsQuantity= 0;
	uint8_t EEpromReadStatus;
	PrintToLCD(Version);
	InfoToUARTBeforeStart();
	logInfo(Version);
	SetSymbols();
	lcd_set_xy(0,0);
	delay_ms(1000);
	flash_read_block();
	if (LoggingData.RecordsQuantity>=MAX_LOG_ITEMS) LoggingData.RecordsQuantity = 0;
	EEpromReadStatus = ReadFromFlash();
	if (EEpromReadStatus==0)
	{
		delay_ms(1000);
		EEpromReadStatus = ReadFromFlash();
		logInfo("Read from EEprom - FAIL");
	}
	if (EEpromReadStatus == 0)
	{
		PrintToLCD("EEprom Read FAIL");
		delay_ms(4000);
		WriteInLOG("EEprmReadFAIL");
		logInfo("Read from EEprom - FAIL");
	}
	if (EEpromReadStatus == 1)
		logInfo("Read from EEprom - SUCCESS");



  BatteryCapacityDischargeCurrentAfterPOwerUp = SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent;
  ChargeDurationSec = SettingsData.ChargeTime*3600;
  SelectedOptionValue = SettingsData.Option1;
  SelectedOptionValue1 = SettingsData.Option2;
  OutStateAfterPowerUp = SettingsData.Option2;
  Generation_Coefficients_R_A_B();
  delay_ms(1000);
  lcd_clear();
	Menu_SetGenericWriteCallback(Generic_Write);
	if (SettingsData.Option1 == 1)
		Menu_Navigate(&Menu_2_1);
	else if (SettingsData.Option1 == 2)
		Menu_Navigate(&Menu_3_1);
	else if (SettingsData.Option1 == 3)
		Menu_Navigate(&Menu_4_1);
	else if (SettingsData.Option1 == 4)
		Menu_Navigate(&Menu_5_1);
	else if (SettingsData.Option1 == 5)
		Menu_Navigate(&Menu_6_1);
	else if (SettingsData.Option1 == 6)
		Menu_Navigate(&Menu_7_1);
	else if (SettingsData.Option1 == 7)
		Menu_Navigate(&Menu_8_1);
	else Menu_Navigate(&Menu_2_1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //logDebugD("f1 ",f1,0);
	 // logDebugD("f2 ",f2,0);
	  //logDebugD("f1-f2 ",f1-f2,0);
  	Blink_message_counter++;
    Key_Pressed_t Button;
  	Button=BUT_GetKey();
	switch (Button)
	{

		case KEY_BACK:
			Menu_Navigate(MENU_PREVIOUS);
			break;
		case KEY_NEXT:
			Menu_Navigate(MENU_NEXT);
			break;
		case KEY_OK:
			Menu_SelectItem(KEY_NEXT);
			Menu_Navigate(MENU_CHILD);
			break;
		case KEY_UP:
			Menu_Navigate(MENU_PARENT);
			break;
		default:
			break;
	}

	if (Menu_GetCurrentMenu() == &Menu_2_1)
		MenuPowerSupply(Button);
	else if (Menu_GetCurrentMenu() == &Menu_3_1)
		MenuLoad(Button);
	else if (Menu_GetCurrentMenu() == &Menu_4_1)
		MenuChargeCC_CV(Button);
	else if (Menu_GetCurrentMenu() == &Menu_5_1)
		MenuChargeAdapt(Button);
	else if (Menu_GetCurrentMenu() == &Menu_6_1)
		MenuDisCharge(Button);
	else if (Menu_GetCurrentMenu() == &Menu_6_1a)
		MenuCheckingCapacity(Button);
	else if (Menu_GetCurrentMenu() == &Menu_7_1)
		MenuTraining_new(Button);
	else if (Menu_GetCurrentMenu() == &Menu_8_1)
		MenuSwing(Button);
	else if (Menu_GetCurrentMenu() == &Menu_8a_1)
		MenuBatterySimilation(Button);
	else if (Menu_GetCurrentMenu() == &Menu_9_1)
		MenuDIAGNOSTIC(Button);
	else if (Menu_GetCurrentMenu() == &Menu_11_1)
		MenuLog(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_2_1)
		MenuCalibration_CURRENT_Load_to_0(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_1_1)
		MenuCalibration_CURRENT_Out_to_0(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_4_1)
		MenuCalibration_CURRENT_Load(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_3_1)
		MenuCalibration_CURRENT_Out(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_7_1)
		MenuCalibration_VoltageIn(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_6_1)
		MenuCalibration_VoltageOut(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_5_1)
		MenuCalibration_VoltagePS(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_8_1)
		MenuCalibration_Resist_Comp_5V1A(Button);
	else if (Menu_GetCurrentMenu() == &Menu_10_9_1)
		MenuCalibration_BackToFactory(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_1_1)
		MenuSettingsChargeTime(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_1_1a)
		MenuSettingsBatteryType(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_2_1)
		MenuSettingsLowVolt(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_3_1)
		MenuSettingsMaxVolt(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_4_1)
		MenuSettingsSwngChrgTime(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_5_1)
		MenuSettingsSwngDChrgTime(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_6_1)
		MenuSettingsChargeAddapt(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_S_1)
		MenuSettingsSaveMenuPosWhenOFF(Button);
	else if (Menu_GetCurrentMenu() == &Menu_1_SO_1)
		MenuSettingsOutAfterPowerUp(Button);
	else EnterInMenu_Status = 0;
	if (EnterInMenu_Status == 0)
	{
		OFF();
		InitiStatus = 0;
		CountShow = 0;
		SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
		DischargeTimeSec_Previous = DischargeTimeSec;
		ChargeDischargeState = 0;
		OutStateAfterPowerUp = 1;
	}
	delay_ms(100);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_3);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(24000000);
  LL_SetSystemCoreClock(24000000);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_2);
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**ADC1 GPIO Configuration
  PA1   ------> ADC1_IN1
  PA2   ------> ADC1_IN2
  PA3   ------> ADC1_IN3
  PA4   ------> ADC1_IN4
  PA5   ------> ADC1_IN5
  PA6   ------> ADC1_IN6
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_2|LL_GPIO_PIN_3|LL_GPIO_PIN_4
                          |LL_GPIO_PIN_5|LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* ADC1 DMA Init */

  /* ADC1 Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);

  /* USER CODE BEGIN ADC1_Init 1 */

  LL_DMA_ConfigAddresses(DMA1,
                          LL_DMA_CHANNEL_1,
                          LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
                          (uint32_t)RegularConvData,
                          LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  //LL_DMA_SetPeriphAddress(DMA1,LL_DMA_CHANNEL_1,LL_ADC_DMA_GetRegAddr(ADC1,LL_ADC_DMA_REG_REGULAR_DATA));
  //LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_1,RegularConvData);
  LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, NUMBER_OF_CHANNELS);
  //LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
  //LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_1);
  LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_ENABLE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_8RANKS;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_1);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_1, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_2);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_3);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_4);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_4, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_5, LL_ADC_CHANNEL_5);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_5, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_6, LL_ADC_CHANNEL_6);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_6, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_7, LL_ADC_CHANNEL_TEMPSENSOR);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_TEMPSENSOR);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_8, LL_ADC_CHANNEL_VREFINT);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_239CYCLES_5);
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
  /* USER CODE BEGIN ADC1_Init 2 */

	 LL_ADC_Enable(ADC1);
	  delay_ms(100);
	  LL_ADC_StartCalibration(ADC1);
	  while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0) {}
	  LL_ADC_REG_StartConversionSWStart(ADC1);


  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(USART1_IRQn);

  /* USER CODE BEGIN USART1_Init 1 */
  LL_USART_EnableIT_TXE(USART1);
  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 38400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0|LL_GPIO_PIN_1|LL_GPIO_PIN_10|LL_GPIO_PIN_11
                          |LL_GPIO_PIN_12|LL_GPIO_PIN_13|LL_GPIO_PIN_14|LL_GPIO_PIN_15);

  /**/
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_15);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11|LL_GPIO_PIN_12|LL_GPIO_PIN_13
                          |LL_GPIO_PIN_14|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8|LL_GPIO_PIN_11|LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
