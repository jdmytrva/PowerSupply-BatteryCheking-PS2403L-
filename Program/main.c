#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include "Initialization/Init_.h"
#include "usart.h"
#include "ADC.h"
#include "hd44780_driver/hd44780_driver.h"
#include "Menu/MicroMenu2_1.h"
#include "Menu/Menu.h"
#include "Library/Lib.h"
#include "General/General.h"
#include "Eeprom/Eeprom.h"

#define THRESHOLD2 1500
#define THRESHOLD 20

#define MENUDELAY 100

#define VOLTAGE_OFF_SYSTEM 2000
//#define VOLTAGE_OFF_SYSTEM 1400
//#define VOLTAGE_OFF_SYSTEM 700

char Version[] = "PS 23V 3A v2.00R";


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

void BUT_Debrief(void);
void init_timer6();
uint8_t BUT_GetKey(void);
void ResetTime(void);
void clock (void);


void delay_ms(uint16_t value);

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

void Delay_mSec(volatile uint16_t Count)//mSec
{
	volatile uint32_t tmpvar;
	for (tmpvar=Count*2400;tmpvar!=0;tmpvar--);
}


//  CNF7  |  Mode7  |  CNF6  |  Mode6  |  CNF5  |  Mode5  |
//31 | 30 |29 | 28  | 27 | 26|25 | 24  |23 | 22 |21 | 20  |


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

void Delay_ms(volatile uint32_t value)
{
  TIM2->ARR = value;
  TIM2->CNT = 0;
  TIM2->CR1 = TIM_CR1_CEN | TIM_CR1_OPM;
  while((TIM2->SR & TIM_SR_UIF)==0){}
  TIM2->SR &= ~TIM_SR_UIF;
}


//////MENU FUNCTIONS ///////////////MENU FUNCTIONS ///////////////MENU FUNCTIONS /////////
void Generic_Write(char* Text)
{
	if (Text) PrintToLCD_xy(Text,0,0);
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
    		OUT_ON();
    	else
    		OUT_OFF();
    }
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
	Print_to_USART1_d(InitiStatus,"Init status: ",0);
	Print_to_USART1_d(EnterInMenu_Status,"EnterInMenu_Status: ",0);

	Print_to_USART1_d(ChargeStatusForTimer,"Chargestatus timer: ",0);
	Print_to_USART1_d(ChargeTimeSec,"ChargeTimeSec: ",0);
	Print_to_USART1_d(DisChargeStatusForTimer,"Dischargestatus timer: ",0);
	Print_to_USART1_d(DischargeTimeSec,"DischargeTimeSec: ",0);

}
void MenuChargeAdapt(Key_Pressed_t key)
{
	EnterInMenu_Status = 1;
	if (InitiStatus==0)
	{
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
	Delay_mSec(MENUDELAY);

	Print_to_USART1_d(InitiStatus,"Init status: ",0);
	Print_to_USART1_d(EnterInMenu_Status,"EnterInMenu_Status: ",0);

	Print_to_USART1_d(ChargeStatusForTimer,"Chargestatus timer: ",0);
	Print_to_USART1_d(ChargeTimeSec,"ChargeTimeSec: ",0);
	Print_to_USART1_d(DisChargeStatusForTimer,"Dischargestatus timer: ",0);
	Print_to_USART1_d(DischargeTimeSec,"DischargeTimeSec: ",0);

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
	Print_to_USART1_d(InitiStatus,"Init status: ",0);
	Print_to_USART1_d(EnterInMenu_Status,"EnterInMenu_Status: ",0);

	Print_to_USART1_d(ChargeStatusForTimer,"Chargestatus timer: ",0);
	Print_to_USART1_d(ChargeTimeSec,"ChargeTimeSec: ",0);
	Print_to_USART1_d(DisChargeStatusForTimer,"Dischargestatus timer: ",0);
	Print_to_USART1_d(DischargeTimeSec,"DischargeTimeSec: ",0);

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
		Delay_ms(1000);
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
				DischargeTimeSec = 0;
				SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
				BatteryCapacityDischargeCurrent = 0;

				//Start charge action
				ChargeStatusForTimer = 1;
				charge1();
				CheckingCapacityCounter ++;

				//Delay_ms(300);
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
				ChargeTimeSec = 0;
				BatteryCapacityCharge = 0;

				//Start discharge activity
				DisChargeStatusForTimer = 1;
				discharge1();
				CheckingCapacityCounter ++;
				//Delay_ms(100);
				ChargeDischargeStatus = DISCHARGEs;
			}
			if (CheckingCapacityCounter>3)
				CountShow = 3;
			//Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu
			#define MAXITEM3 5
			if (key == KEY_NEXT)
			{
				CountShow++;
				if (CountShow==MAXITEM3) CountShow=0;
			}
			if (key == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAXITEM3-1;
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
				PrintToLCD("N ");
				PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600));
				PrintToLCD("mAH     ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec);
			}
			if(CountShow == 3)
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
			if(CountShow == 4)
			{
				lcd_set_xy(0,0);
				LcdOutbyNumber(0,1);//charge
				lcd_set_xy(1,0);
				PrintToLCD("C ");
				PrintToLCD(itoa(BatteryCapacityCharge/3600));
				PrintToLCD("mAH     ");
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
		OFF();
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;

		#define MAXITEM3 5
		if (key == KEY_NEXT)
		{
			CountShow++;
			if (CountShow==MAXITEM3) CountShow=0;
		}
		if (key == KEY_BACK)
		{
			CountShow--;
			if (CountShow<0) CountShow=MAXITEM3-1;
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
			PrintToLCD("N ");
			PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600));
			PrintToLCD("mAH     ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(DischargeTimeSec);
		}
		if(CountShow == 3)
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
		if(CountShow == 4)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(0,1);//charge
			lcd_set_xy(1,0);
			PrintToLCD("C ");
			PrintToLCD(itoa(BatteryCapacityCharge/3600));
			PrintToLCD("mAH     ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(ChargeTimeSec);
		}
	}
	Print_to_USART1_d(EnterInMenu_Status,"EnterInMenu_Status: ",0);

	Print_to_USART1_d(ChargeStatusForTimer,"Chargestatus timer: ",0);
	Print_to_USART1_d(ChargeTimeSec,"ChargeTimeSec: ",0);
	Print_to_USART1_d(DisChargeStatusForTimer,"Dischargestatus timer: ",0);
	Print_to_USART1_d(DischargeTimeSec,"DischargeTimeSec: ",0);

	Print_to_USART1_d(BatteryCapacityDischargeCurrent,"BatteryCapacityDischargeCurrent: ",0);
	Print_to_USART1_d(BatteryCapacityCharge,"BatteryCapacityCharge: ",0);
	//Delay_ms(100);

}

void MenuTraining_new(Key_Pressed_t key)
{
	if (EnterInMenu_Status == 0)
	{
		EnterInMenu_Status = 1;
		ChargeStatusForTimer = 0;
		DisChargeStatusForTimer = 0;
		ChargeTimeSec = 0;
		DischargeTimeSec = 0;
		DischargeTimeSec_Previous = 0;
		BatteryCapacityCharge = 0;
		BatteryCapacityDischargeCurrent = 0;

		//Start discharge activity
		DisChargeStatusForTimer = 1;
		discharge1();
		ChargeDischargeStatus = DISCHARGEs;
		lcd_set_xy(0,0);
		PrintToLCD("Testing Battery.");
		Delay_ms(1000);
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
			DischargeTimeSec_Previous = DischargeTimeSec;
			DischargeTimeSec = 0;
			SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
			BatteryCapacityDischargeCurrent = 0;

			//Start charge action
			ChargeStatusForTimer = 1;
			charge1();

			//Delay_ms(300);
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
			ChargeTimeSec = 0;
			BatteryCapacityCharge = 0;

			//Start discharge activity
			DisChargeStatusForTimer = 1;
			discharge1();
			//Delay_ms(100);
			ChargeDischargeStatus = DISCHARGEs;
		}

		//Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu
		#define MAXITEM3 5
		if (key == KEY_NEXT)
		{
			CountShow++;
			if (CountShow==MAXITEM3) CountShow=0;
		}
		if (key == KEY_BACK)
		{
			CountShow--;
			if (CountShow<0) CountShow=MAXITEM3-1;
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
			PrintToLCD("N ");
			PrintToLCD(itoa(BatteryCapacityDischargeCurrent/3600));
			PrintToLCD("mAH     ");
			lcd_set_xy(3,1);
			ClockOnLCD_noSec(DischargeTimeSec);
		}
		if(CountShow == 3)
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
		if(CountShow == 4)
		{
			lcd_set_xy(0,0);
			LcdOutbyNumber(0,1);//charge
			lcd_set_xy(1,0);
			PrintToLCD("C ");
			PrintToLCD(itoa(BatteryCapacityCharge/3600));
			PrintToLCD("mAH     ");
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
	Print_to_USART1_d(EnterInMenu_Status,"EnterInMenu_Status: ",0);

	Print_to_USART1_d(ChargeStatusForTimer,"Chargestatus timer: ",0);
	Print_to_USART1_d(ChargeTimeSec,"ChargeTimeSec: ",0);
	Print_to_USART1_d(DisChargeStatusForTimer,"Dischargestatus timer: ",0);
	Print_to_USART1_d(DischargeTimeSec,"DischargeTimeSec: ",0);

	Print_to_USART1_d(BatteryCapacityDischargeCurrent,"BatteryCapacityDischargeCurrent: ",0);
	Print_to_USART1_d(BatteryCapacityCharge,"BatteryCapacityCharge: ",0);
	//Delay_ms(100);
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
	Print_to_USART1_d(i_LogItems,"i_LogItems: ",0);
	Print_to_USART1_d(LoggingData.RecordsQuantity,"MRecordsQuantity: ",0);




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
		if ((CountShow1 == 2)|| (CountShow1 == 3))
			OUT_ON_OFF_Toggle();
		else OUT_OFF();

		if (CountShow1 == 4)
			LOAD_ON_OFF_Toggle();
	}
	if(CountShow1 == 0)
	{
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
		lcd_set_xy(0,0);
		PrintToLCD("Ups ");
		PrintToLCD(itoa_koma(U_PS,2));
		PrintToLCD("V ");
		PrintToLCD(itoa((RegularConvData[3])));
		PrintToLCD("      ");
	}
	if(CountShow1 == 2)
	{
		lcd_set_xy(0,0);
		PrintToLCD("Uout ");
		PrintToLCD(itoa_koma(U_OUT,2));
		PrintToLCD("V ");
		PrintToLCD(itoa((RegularConvData[2])));
		PrintToLCD("      ");
	}
	if(CountShow1 == 3)
	{
		lcd_set_xy(0,0);
		PrintToLCD("Iout ");
		PrintToLCD(itoa(Current));
		PrintToLCD("mA ");
		PrintToLCD(itoa((RegularConvData[1])));
		PrintToLCD("      ");
	}
	if(CountShow1 == 4)
	{
		lcd_set_xy(0,0);
		PrintToLCD("Iload ");
		PrintToLCD(itoa(Current_load));
		PrintToLCD("mA ");
		PrintToLCD(itoa(RegularConvData[0]));
		PrintToLCD("      ");
	}
	if(CountShow1 == 5)
	{
		OUT_ON();
		CalibrationData.ResistanceComp_MOSFET= (int32_t)(U_PS-U_OUT_ForSetResistance)*10000/Current;
		lcd_set_xy(0,0);
		PrintToLCD("R=");
		PrintToLCD(itoa(CalibrationData.ResistanceComp_Ishunt_Wires));
		PrintToLCD("mOm ");
		PrintToLCD(itoa(CalibrationData.ResistanceComp_MOSFET));
		PrintToLCD("mOm    ");
		Print_to_USART1_d(Current,"I: ",0);
		Print_to_USART1_d(U_PS,"U_PS: ",2);
		Print_to_USART1_d(U_OUT_ForSetResistance,"U out: ",2);
	}
	if(CountShow1 == 6)
	{
		lcd_set_xy(0,0);
		//ClockOnLCD(time_sec);
		char s[17];
		PrintToLCD(ClockStringWithSec(time_sec,s));
	}

	if(CountShow1 == 7)
	{
		lcd_set_xy(0,0);
		PrintToLCD("Old C=");
		PrintToLCD(itoa(BatteryCapacityDischargeCurrentAfterPOwerUp/3600));
		PrintToLCD("mAh          ");
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
		Delay_mSec(2000);
		lcd_set_xy(0,0);
		if (EEpromReadStatus == 1)
		{
			PrintToLCD("Factory are OK  ");
			WriteInLOG("Factory are OK");
			Delay_mSec(2000);
		}
		else
		{
			PrintToLCD("EEprom read FAIL");
			Delay_mSec(4000);

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
    Delay_mSec(600);
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
void MenuCalibrationWriteToFlash_Enter(Key_Pressed_t key)
{
	CalibrationWriteToFlash_CRC();
	WriteInLOG("Calibration");
}

void MenuSettingsWriteToFlash_Enter(Key_Pressed_t key)
{
	SettingsWriteToFlash_CRC();
	//WriteInLOG("Seet");

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
	Print_to_USART1_d(BatteryCapacityCharge,"charge=",0);
	if (BatteryCapacityCharge/3600>10)
	{
		char str[17];
		char strout[17];
		char s_clock[17];
		ClockStringNoSec(ChargeTimeSec,s_clock);
		Merge3Strings(itoaP(BatteryCapacityCharge/3600,str),"mAh",s_clock,strout);
		WriteInLOGc(strout,CHARGE_H);
		Print_to_USART1(strout);
	}

}

void MenuOption_Enter(Key_Pressed_t key)
{
	lcd_set_xy(7,1);
	lcd_send(255,DATA);
	SettingsWriteToFlash_CRC();
	SelectedOptionValue = SettingsData.Option1;
	Delay_mSec(200);
}



int16_t comp = 0;
void BUT_Debrief(void)
{
	Key_Pressed_t key;

  if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
    key = KEY_OK;
  else if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5))
    key = KEY_NEXT;
  else if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
    key = KEY_BACK;
  else {
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



void TIM7_IRQHandler()
{

  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    BUT_Debrief();
    All_OUT_OFF_When_Power_OFF();
    TimerForReadyMeasurement_ms++;
  }
}

void TIM1_UP_TIM16_IRQHandler()
{
  if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM16, TIM_IT_Update);


	//capacity
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
  }
}

void Start_Timer_sec()
{
	Timer_Sec = 0;
	Status_Timer_Sec = 1;
}
void ReStart_Timer_sec()
{
	//Print_to_USART1_d(Timer_Sec,"Timer_Sec:Restart Timer ",0);
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
	if (U_IN < VOLTAGE_OFF_SYSTEM)
	{
		OFF();
		if (EEpromWrite_status == 0)
		{
			EEpromWrite_status = 1;
			SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent = BatteryCapacityDischargeCurrent;
			DataWhenPowerOffWriteToFlash_CRC();
			Print_to_USART1_d(SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent,"dc: ",2);
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
			for (i = 0; i<50; i++)
			{
				Print_to_USART1_d(U_IN,"U off: ",2);
				Delay_mSec(10);
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
	Delay_ms(50);
}

void discharge()
{
	OUT_OFF();
	LOAD_ON();
	Delay_ms(50);

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
		Delay_ms(50);
		Print_to_USART1("Charge");
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
		Delay_ms(50);
		Print_to_USART1("DIsCharge");
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
   	//Print_to_USART1_d(On_off,"Select OFF:",0);
	Status_Out = 0;

}
void OUT_ON()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	GPIOB->BSRR =  GPIO_BSRR_BS0;//ON-OFF OUT
	//GPIOB->BSRR =  GPIO_BSRR_BR1; //load1

	On_off = 1;
   	//Print_to_USART1_d(On_off,"SelectON:",0);
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


//=========================MAIN==================
int main(void)
{
	Initialization();
	OFF();
	LoggingData.RecordsQuantity= 0;
	uint8_t EEpromReadStatus;
	PrintToLCD(Version);
	SetSymbols();
	lcd_set_xy(0,0);
	Delay_mSec(2000);
	flash_read_block();
	if (LoggingData.RecordsQuantity>=MAX_LOG_ITEMS) LoggingData.RecordsQuantity = 0;
	//WriteInLOG("Power ON");
	EEpromReadStatus = ReadFromFlash();
	if (EEpromReadStatus==0)
	{
		Delay_mSec(1000);
		EEpromReadStatus = ReadFromFlash();
	}
	if (EEpromReadStatus == 0)
	{
		PrintToLCD("EEprom Read FAIL");
		Delay_mSec(4000);
		WriteInLOG("EEprmReadFAIL");
	}


    BatteryCapacityDischargeCurrentAfterPOwerUp = SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent;
    ChargeDurationSec = SettingsData.ChargeTime*3600;
    SelectedOptionValue = SettingsData.Option1;
    Print_to_USART1(Version);
    InfoToUARTBeforeStart();
    //HSE_PLL();

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

    while(1)
    {
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
			default:
				//Print_to_USART1("NO key Pressed");
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
			//MenuTraining(Button);
			MenuTraining_new(Button);
		else if (Menu_GetCurrentMenu() == &Menu_8_1)
			MenuSwing(Button);
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
		else EnterInMenu_Status = 0;

		if (EnterInMenu_Status == 0)
		{
			OFF();
			InitiStatus = 0;
			CountShow = 0;
			SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
			//Print_to_USART1_d(SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue,"Menu C maH : ",0);

			DischargeTimeSec_Previous = DischargeTimeSec;
			ChargeDischargeState = 0;

		}
        Delay_ms(100);
    }//while
}//main
