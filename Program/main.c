#include "hd44780_driver/hd44780_driver.h"
#include "Menu/Menu.h"
#include "General/General.h"
#include "Eeprom/Eeprom.h"
#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include "usart.h"
#include "ADC.h"
//test




#define THRESHOLD2 1500
#define THRESHOLD 20
#define KEY_OK 3
#define KEY_NEXT 1
#define KEY_BACK 2
#define MENUDELAY 100



uint8_t Version[] = "PS 30V 3A v1.4 ";
volatile uint8_t pressedKey = 0;
volatile uint32_t  time_sec = 0;
volatile uint32_t  Timer_Sec = 0;
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
volatile uint32_t BatteryCapacityDischargePrevious = 0;
volatile uint16_t U_12V_Previous = 0;
volatile uint8_t U_12_Status = 1;
volatile uint16_t ResistanceComp_Voltage = 470;
void UpdateDataFromMenuToEEpromAndSaveData(void);
int ReadFromEEprom();
void UpdateMenuFromEEprom();
void EEpromWrite(void);
void FactoryEEpromWrite(void);
struct menu_item *MenuLevel = &Menu[0];

void BUT_Debrief(void);
void Init_button(void);
void init_timer6();
uint8_t BUT_GetKey(void);
void ResetTime(void);
void clock (void);
uint8_t strlen1(uint8_t string[]);

void delay_ms(uint16_t value);
void InitLCD();
void init_timer7();
void init_timer16();
void InitUART();
void PrintOnLCD(uint8_t* str);
void PrintOnLCDFull(uint8_t* str);
void PrintOnLCD_xy(uint8_t* str ,uint8_t x, uint8_t y);
void Init_Out(void);


int32_t Module32 (int32_t value);
int16_t Module16 (int16_t value);
int8_t Module8 (int8_t value);
void ClockOnLCD (uint32_t time);
void ClockOnLCD_noSec (uint32_t time);

void discharge();
void charge();
void OFF();
void OUT_OFF();
void OUT_ON();
void OUT_ON_OFF_Toggle();
void LOAD1_ON();
void LOAD1_OFF();
void LOAD1_ON_OFF_Toggle();
void LOAD2_ON();
void LOAD2_OFF();
void FAN_ON();
void FAN_OFF();
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


uint8_t On_off = 0;
uint8_t Status_Load = 0;
uint8_t Status_Out = 0;
int8_t CountShow = 0;
int8_t CountShow1 = 0;
uint8_t entered_in_charge_discharge_menu = 0;
int8_t PopUpMessage = 0;
uint32_t SwingtimeSec = 0;
volatile uint8_t ChargeStatusForTimer = 0;
volatile uint8_t DisChargeStatusForTimer = 0;
//=========================MAIN==================
int main(void)
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
	OFF();


	PrintOnLCDFull(Version);
	SetSymbols();


	lcd_set_xy(0,0);


	ADC1_CH_DMA_Config();


	Delay_mSec(2000);
	uint8_t i=0;
	uint8_t s[16];
    char c, key, Button=0;

    uint32_t savedefault;
    struct menu_item *MenuLevelTemp = NULL;
    MenuLevel = MenuLevel->Next;
    //MenuLevel = MenuLevel->Next;
   // MenuLevel = MenuLevel->Next;
   // MenuLevel = MenuLevel->Next;

    MenuLevel = MenuLevel->Child;
    //EEpromWrite();
    UpdateMenuFromEEprom();
    BatteryCapacityDischargePrevious = SaveData.BatteryCapacityDischargePreviousValue;
    BatteryCapacityDischargeCurrentAfterPOwerUp = SaveData.BatteryCapacityDischargeCurrent;
    Print_to_USART1(Version);
    InfoToUARTBeforeStart();
    //HSE_PLL();
    uint16_t Data = 0;
    uint8_t *Package = (uint8_t*) &Data;
    lcd_clear();
    uint32_t U_Battery_Timer = 0;
    uint32_t VeryLowBattery_Timer = 0;
    uint32_t OffCurrent_Timer = 0;
    uint8_t BatteryLow=0;
    uint8_t BatteryVeryLow=0;
    uint16_t U_BatteryTmp = 0;
    uint8_t OffCurrentStatus = 0;
    uint8_t ChargedStatus = 1;
    uint8_t BATERYSTATUS = 0;
    uint8_t InitiStatus = 0;


	uint32_t Low_Voltage = SaveData.LowVoltage*10;
	uint32_t Max_Voltage = SaveData.MaxVoltage*10;
	uint32_t SwingChargeTime = SaveData.Swing_Chrg_time;
	uint32_t SwingDChargeTime = SaveData.Swing_DChrg_time;
    uint32_t ChargeDurationSec =SaveData.Value*3600;
    uint8_t Blink_message_counter = 0;
    while(1)
    {
        entered_in_charge_discharge_menu = 0;
    	Blink_message_counter++;
    	//Print_to_USART1("START ");
    	Button=BUT_GetKey();
        if (Button == KEY_NEXT)
        {
        	lcd_clear();
        	if (MenuLevel->TypeOfMenu == CHARGE_TIME||MenuLevel->TypeOfMenu == LOW_VOLTAGE || MenuLevel->TypeOfMenu == MAX_VOLTAGE || MenuLevel->TypeOfMenu == SWING_CHRG_TIME || MenuLevel->TypeOfMenu == SWING_DCHRG_TIME)
            {
                 MenuLevel->SelectValues++;
                 //if (MenuLevel->SelectValues>100) MenuLevel->SelectValues = 0;
            }
        	if (MenuLevel->TypeOfMenu == VOLTAGE)
        		CalibrationValueForVoltage++;
           	if (MenuLevel->TypeOfMenu == CURRENTOUT)
            		CalibrationValueForCurrent++;
          	if (MenuLevel->TypeOfMenu == CURRENTLOAD)
            		CalibrationValueForCurrent1++;
           	if (MenuLevel->TypeOfMenu == VOLTAGE1)
            		CalibrationValueForVoltage1++;
           	if (MenuLevel->TypeOfMenu == VOLTAGEIN)
            		CalibrationValueForVoltage2++;
           	if (MenuLevel->TypeOfMenu == CHARGE_ADAPT)
           			ChargeAdapt++;
           	if (MenuLevel->TypeOfMenu == RESISTANCE_COMP)
           		ResistanceComp_Voltage++;



            if (MenuLevel->Next) MenuLevel = MenuLevel->Next;//Next Menu

        }
        if (Button == KEY_BACK)
        {
        	lcd_clear();
            if (MenuLevel->TypeOfMenu == CHARGE_TIME||MenuLevel->TypeOfMenu == LOW_VOLTAGE || MenuLevel->TypeOfMenu == MAX_VOLTAGE || MenuLevel->TypeOfMenu == SWING_CHRG_TIME || MenuLevel->TypeOfMenu == SWING_DCHRG_TIME)
            {
                 MenuLevel->SelectValues--;
                 //if (MenuLevel->SelectValues>100) MenuLevel->SelectValues = 0;
            }

           	if (MenuLevel->TypeOfMenu == VOLTAGE)
             		CalibrationValueForVoltage--;
           	if (MenuLevel->TypeOfMenu == CURRENTOUT)
            		CalibrationValueForCurrent--;
           	if (MenuLevel->TypeOfMenu == CURRENTLOAD)
            		CalibrationValueForCurrent1--;
           	if (MenuLevel->TypeOfMenu == VOLTAGE1)
            		CalibrationValueForVoltage1--;
           	if (MenuLevel->TypeOfMenu == VOLTAGEIN)
            		CalibrationValueForVoltage2--;
           	if (MenuLevel->TypeOfMenu == CHARGE_ADAPT)
           			ChargeAdapt--;
           	if (MenuLevel->TypeOfMenu == RESISTANCE_COMP)
           		ResistanceComp_Voltage--;

            if (MenuLevel->Prev) MenuLevel = MenuLevel->Prev;//Next Menu

        }
        if (Button == KEY_OK)
        {
        	lcd_clear();
            if (MenuLevel->Child)
            {
                if ((MenuLevelTemp!=MenuLevel)&&(MenuLevel->TypeOfMenu==OPTION)&&(MenuLevel->SelectValues != SET))
                {
                    //MenuLevelTemp->select = 0;
                    MenuLevel->SelectValues = SET;
                    MenuLevelTemp->SelectValues = UNSET;
                    MenuLevelTemp = MenuLevel;
                    UpdateDataFromMenuToEEpromAndSaveData();//update in eeprom

                }else
                {
                	if (MenuLevel->TypeOfMenu == CHARGE_TIME)
                	{
                		UpdateDataFromMenuToEEpromAndSaveData();//update value in eeprom
                		 ChargeDurationSec =SaveData.Value*3600;//update value
                	}
                	if (MenuLevel->TypeOfMenu == LOW_VOLTAGE)
                	{
                		UpdateDataFromMenuToEEpromAndSaveData();//update value in eeprom
                		 Low_Voltage =SaveData.LowVoltage*10;//update value
                	}
                	if (MenuLevel->TypeOfMenu == MAX_VOLTAGE)
                	{
                		UpdateDataFromMenuToEEpromAndSaveData();//update value in eeprom
                		Max_Voltage =SaveData.MaxVoltage*10;//update value
                	}
                	if (MenuLevel->TypeOfMenu == SWING_CHRG_TIME)
                	{
                		UpdateDataFromMenuToEEpromAndSaveData();//update value in eeprom
                		SwingChargeTime =SaveData.Swing_Chrg_time;//update value
                	}
                	if (MenuLevel->TypeOfMenu == SWING_DCHRG_TIME)
                	{
                		UpdateDataFromMenuToEEpromAndSaveData();//update value in eeprom
                		SwingDChargeTime =SaveData.Swing_DChrg_time;//update value
                	}
                   	if (MenuLevel->TypeOfMenu == VOLTAGE)
                    {
                   		SaveData.CalibrationValueForVoltage = CalibrationValueForVoltage;
                   		EEpromWrite();
                    }
                  	if (MenuLevel->TypeOfMenu == VOLTAGE1)
                    {
                   		SaveData.CalibrationValueForVoltage1 = CalibrationValueForVoltage1;
                   		EEpromWrite();
                    }
                  	if (MenuLevel->TypeOfMenu == VOLTAGEIN)
                    {
                   		SaveData.CalibrationValueForVoltage2 = CalibrationValueForVoltage2;
                   		EEpromWrite();
                    }
                  	if (MenuLevel->TypeOfMenu == CURRENTOUT)
                    {
                   		SaveData.CalibrationValueForCurrent = CalibrationValueForCurrent;
                   		EEpromWrite();
                    }

                 	if (MenuLevel->TypeOfMenu == CURRENTLOAD)
                    {
                   		SaveData.CalibrationValueForCurrent1 = CalibrationValueForCurrent1;
                   		EEpromWrite();
                    }
                 	if (MenuLevel->TypeOfMenu == CHARGE_ADAPT)
                    {
                   		SaveData.ChargeAdapt = ChargeAdapt;
                   		EEpromWrite();
                    }

                 	if (MenuLevel->TypeOfMenu == RESISTANCE_COMP)
                    {
                   		SaveData.ResistanceComp = ResistanceComp;
                   		EEpromWrite();
                    }



                    MenuLevelTemp = NULL;
                    MenuLevel = MenuLevel->Child;//Enter to down menu
                    if (MenuLevel->TypeOfMenu == OPTION)
                    {
                        while((MenuLevel->SelectValues!=SET))
                        {
                            MenuLevel=MenuLevel->Next;
                        }
                        MenuLevelTemp = MenuLevel;
                    }
                }
            }//if (MenuLevel->Child)
        }//if Button == KEY_OK
        //print menu and option
        if (MenuLevel->TypeOfMenu == MENU || MenuLevel->TypeOfMenu == OPTION )
       	{
       		PrintOnLCD_xy(MenuLevel->Name,0,0);
       	}
        if (MenuLevel->TypeOfMenu == VOLTAGE||MenuLevel->TypeOfMenu == CHARGE_TIME||MenuLevel->TypeOfMenu == LOW_VOLTAGE || MenuLevel->TypeOfMenu == MAX_VOLTAGE || MenuLevel->TypeOfMenu == SWING_CHRG_TIME || MenuLevel->TypeOfMenu == SWING_DCHRG_TIME)
       	{
			lcd_set_xy(0,0);
			itoa(MenuLevel->SelectValues,s);
			PrintOnLCD(s);
       	}
        //ChargeAdapt option
		if(MenuLevel->TypeOfMenu == CHARGE_ADAPT)
		{
			lcd_set_xy(0,0);
			itoa(ChargeAdapt,s);
			PrintOnLCD(s);
			PrintOnLCD("% ");
			itoa_koma(Max_Voltage/10,s,1);
			PrintOnLCDFull(s);
			PrintOnLCDFull("V ");
			itoa_koma((Max_Voltage-(Max_Voltage*ChargeAdapt/100))/10,s,1);
			PrintOnLCDFull(s);
			PrintOnLCDFull("V   ");

		}


        //Calibration print
		if(MenuLevel->TypeOfMenu == VOLTAGE)
		{
			entered_in_charge_discharge_menu=1;
			OUT_ON();
			lcd_set_xy(0,0);
			itoa(CalibrationValueForVoltage,s);
			PrintOnLCD(s);
			PrintOnLCD("   ");
			lcd_set_xy(0,1);
			itoa_koma(U_PS,s,2);
			PrintOnLCDFull(s);
			PrintOnLCDFull("V  ");
		}
		if(MenuLevel->TypeOfMenu == VOLTAGE1)
		{
			entered_in_charge_discharge_menu=1;
			OUT_ON();
			lcd_set_xy(0,0);
			itoa(CalibrationValueForVoltage1,s);
			PrintOnLCD(s);
			PrintOnLCD("   ");
			lcd_set_xy(0,1);
			itoa_koma(U_OUT,s,2);
			PrintOnLCDFull(s);
			PrintOnLCDFull("V  ");
		}
		if(MenuLevel->TypeOfMenu == VOLTAGEIN)
		{
			entered_in_charge_discharge_menu=1;
			OUT_ON();
			lcd_set_xy(0,0);
			itoa(CalibrationValueForVoltage2,s);
			PrintOnLCD(s);
			PrintOnLCD("   ");
			lcd_set_xy(0,1);
			itoa_koma(U_IN,s,2);
			PrintOnLCDFull(s);
			PrintOnLCDFull("V  ");
		}
		if(MenuLevel->TypeOfMenu == CURRENTOUT)
		{
			entered_in_charge_discharge_menu=1;
			OUT_ON();
			lcd_set_xy(0,0);
			itoa(CalibrationValueForCurrent,s);
			PrintOnLCD(s);
			PrintOnLCD("   ");
			lcd_set_xy(0,1);
			itoa(Current,s);
			PrintOnLCDFull(s);
			PrintOnLCDFull("mA  ");
		}
		if(MenuLevel->TypeOfMenu == CURRENTLOAD)
		{
			entered_in_charge_discharge_menu=1;
			discharge();
			lcd_set_xy(0,0);
			itoa(CalibrationValueForCurrent1,s);
			PrintOnLCD(s);
			PrintOnLCD("   ");
			lcd_set_xy(0,1);
			itoa(Current,s);
			PrintOnLCDFull(s);
			PrintOnLCDFull("mA  ");
		}
        //SET  option
        if (MenuLevel->SelectValues == SET && MenuLevel->TypeOfMenu==OPTION)
        {
			lcd_set_xy(7,1);
			lcd_send(255,DATA);
        }


        // Calibration for current0
         if (MenuLevel->TypeOfMenu == CURRENTSET0 )
          {


 			lcd_set_xy(0,0);
    			itoa(Current,s);
 			PrintOnLCDFull(s);
 			PrintOnLCDFull("mA >> set 0   ");
 			if (Button == KEY_NEXT)
 			{

 				Calibration0ValueForCurrent = Current_Out;
 				SaveData.Calibration0ValueForCurrent = Calibration0ValueForCurrent;
 				EEpromWrite();
 			}
          }

         if (MenuLevel->TypeOfMenu == CURRENT1SET0 )
          {


 			lcd_set_xy(0,0);
    		itoa(Current,s);
 			PrintOnLCDFull(s);
 			PrintOnLCDFull("mA >> set 0   ");
 			if (Button == KEY_NEXT)
 			{
 				Calibration0ValueForCurrent1 = Current_load;
 				SaveData.Calibration0ValueForCurrent1 = Calibration0ValueForCurrent1;
 				EEpromWrite();
 			}
          }

 		if(MenuLevel->TypeOfMenu == RESISTANCE_COMP)
 		{
 			entered_in_charge_discharge_menu=1;
 			OUT_ON();
 			lcd_set_xy(0,0);
 			itoa(ResistanceComp_Voltage*10,s);
 			PrintOnLCD(s);
 			PrintOnLCD("mV ");
 			ResistanceComp = (U_OUT_ForSetResistance - ResistanceComp_Voltage)*10000/Current;
 			itoa(ResistanceComp,s);
 			PrintOnLCDFull(s);
 			PrintOnLCDFull("mOm    ");
 		}
         // Back to factory

	   if (MenuLevel->TypeOfMenu == BACKTOFACTORY )
		{
			lcd_set_xy(0,0);
			PrintOnLCDFull("Factory-press >>");
			if (Button == KEY_NEXT)
			{

				FactoryEEpromWrite();
				UpdateMenuFromEEprom();
				lcd_set_xy(0,0);
				PrintOnLCDFull("In Proccess ....");
				Delay_mSec(2000);
			}

	   }

   // LOAD LOAD LOAD
		   //entered_in_charge_discharge_menu = 0;
		if (MenuLevel->TypeOfMenu == LOAD )
		{

			entered_in_charge_discharge_menu = 1;

			if (Button == KEY_NEXT)
			{
				if (On_off == 0)
					discharge();
				else
					OFF();
			}
			lcd_set_xy(0,0);
			itoa_koma(U_OUT,s,2);
			PrintOnLCDFull(s);
			PrintOnLCDFull("V ");

			itoa(Current,s);
			PrintOnLCDFull(s);
			PrintOnLCDFull("mA    ");
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

 // POWERSUPPLY_MENU POWERSUPPLY_MENU POWERSUPPLY_MENU POWERSUPPLY_MENU
        //entered_in_charge_discharge_menu = 0;
		if (MenuLevel->TypeOfMenu == POWERSUPPLY )
		{

			entered_in_charge_discharge_menu = 1;


			lcd_set_xy(0,0);



			if (On_off == 0)
			{
				itoa_koma(U_PS,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
	 			itoa(Current,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA ");
				PrintOnLCDFull("OFF   ");
			}
			else
			{
				itoa_koma(U_OUT,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
	 			itoa(Current,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA ");
				PrintOnLCDFull("ON   ");
			}
	        if (Button == KEY_NEXT)
	        {
	        	if (On_off == 0)
	        		OUT_ON();
	        	else
	        		OUT_OFF();
	        }


	        //Delay_mSec(MENUDELAY);

		}


        //DIAGNOSTIC
        //DIAGNOSTIC
        if (MenuLevel->TypeOfMenu == DIAGNOSTIC )
        {
        	entered_in_charge_discharge_menu=1;

        	//discharge();
			#define MAXITEM 8
			if (Button == KEY_NEXT)
			{
				CountShow1++;
				if (CountShow1==MAXITEM) CountShow1=0;
			}
			if (Button == KEY_BACK)
			{
				if ((CountShow1 == 2)|| (CountShow1 == 3))
					OUT_ON_OFF_Toggle();
				else OUT_OFF();

				if (CountShow1 == 4)
					LOAD1_ON_OFF_Toggle();

			}
			if(CountShow1 == 0)
			{
				lcd_set_xy(0,0);
				//PrintOnLCD(" ");
				itoa_koma(U_IN/10,s,1);
				PrintOnLCDFull("Uin=");
				PrintOnLCDFull(s);
				itoa_koma(U_Controller,s,2);
				PrintOnLCDFull(" Uc=");
				PrintOnLCDFull(s);
				PrintOnLCDFull("    ");

			}
			if(CountShow1 == 1)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("Ups ");
				itoa_koma(U_PS,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
				itoa((RegularConvData[3]),s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("      ");
			}
			if(CountShow1 == 2)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("Uout ");
				itoa_koma(U_OUT,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
				itoa((RegularConvData[2]),s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("      ");
			}
			if(CountShow1 == 3)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("Ips ");
				itoa(Current,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA ");
				itoa((RegularConvData[1]),s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("      ");
			}
			if(CountShow1 == 4)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("Iload ");
				itoa(Current_load,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA ");
				itoa((RegularConvData[0]),s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("      ");
			}
			if(CountShow1 == 5)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("R=");
				itoa(ResistanceComp,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mOm                 ");
			}
			if(CountShow1 == 6)
			{
				lcd_set_xy(0,0);
				ClockOnLCD(time_sec);


			}

			if(CountShow1 == 7)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("Old C=");
				itoa(BatteryCapacityDischargeCurrentAfterPOwerUp/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAh          ");
			}

			//Delay_mSec(MENUDELAY);
        }
	//CHARGE Adapt menu
        if (MenuLevel->TypeOfMenu == CHARGE_ADAPT_MENU )
        {

        	entered_in_charge_discharge_menu = 1;

        	if (InitiStatus==0)
        	{
        		ChargeStatusForTimer = 1;
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
				if (U_OUT> Max_Voltage)
				{
					OFF();
				}else
				{
					if (U_OUT< (Max_Voltage-(Max_Voltage*ChargeAdapt/100))   )
						charge();
				}
			}

			#define MAXITEM0 4
			if (Button == KEY_NEXT)
			{
	        	if (On_off == 0)
	        		charge();
	        	else
	        		OFF();
			}
			if (Button == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAXITEM0-1;
			}
			if(CountShow == 0)
			{
				lcd_set_xy(0,0);
				//PrintOnLCD(" ");


				if (On_off ==0)
				{
					itoa_koma(U_PS,s,2);
					PrintOnLCDFull(s);
					PrintOnLCDFull("V ");
					itoa_koma(U_OUT,s,2);
					PrintOnLCDFull(s);
					PrintOnLCDFull("V   ");

				}
				else
				{
					itoa_koma(U_OUT,s,2);
					PrintOnLCDFull(s);
					PrintOnLCDFull("V ");
					itoa(Current,s);
					PrintOnLCDFull(s);
					PrintOnLCDFull("mA    ");
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
				PrintOnLCDFull("T:");
				itoa(SaveData.Value,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("h MaxV:");

				itoa_koma(Max_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V   ");
			}
			if(CountShow == 2)
			{
				lcd_set_xy(0,0);
				itoa(ChargeAdapt,s);
				PrintOnLCD(s);
				PrintOnLCD("% ");
				itoa_koma(Max_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
				itoa_koma((Max_Voltage-(Max_Voltage*ChargeAdapt/100))/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V   ");
			}
			if(CountShow == 3)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("C ");
				itoa(BatteryCapacityCharge/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(ChargeTimeSec);
			}
        	Delay_mSec(MENUDELAY);
        }

 //CHARGE menu CHARGE menu CHARGE menu CHARGE menu CHARGE menu CHARGE menu CHARGE menu CHARGE menu CHARGE menu
        if (MenuLevel->TypeOfMenu == CHARGE )
        {

        	entered_in_charge_discharge_menu = 1;

        	if (InitiStatus==0)
        	{
        		ChargeStatusForTimer = 1;
        		DisChargeStatusForTimer = 0;
        		BatteryCapacityCharge = 0;
        		InitiStatus = 1;
            	ChargeTimeSec = 0;
        	}

        	if ((ChargeTimeSec > ChargeDurationSec)|| (U_OUT> Max_Voltage))
			{
        		OFF();
        		ChargeStatusForTimer = 0;

			}


			#define MAXITEM0 3
			if (Button == KEY_NEXT)
			{
	        	if (On_off == 0)
	        		charge();
	        	else
	        		OFF();
			}
			if (Button == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAXITEM0-1;
			}
			if(CountShow == 0)
			{
				lcd_set_xy(0,0);
				//PrintOnLCD(" ");


				if (On_off ==0)
				{
					itoa_koma(U_PS,s,2);
					PrintOnLCDFull(s);
					PrintOnLCDFull("V ");
					itoa_koma(U_OUT,s,2);
					PrintOnLCDFull(s);
					PrintOnLCDFull("V   ");

				}
				else
				{
					itoa_koma(U_OUT,s,2);
					PrintOnLCDFull(s);
					PrintOnLCDFull("V ");
					itoa(Current,s);
					PrintOnLCDFull(s);
					PrintOnLCDFull("mA    ");
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
				PrintOnLCDFull("T:");
				itoa(SaveData.Value,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("h MaxV:");

				itoa_koma(Max_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V   ");
			}
			if(CountShow == 2)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("C ");
				itoa(BatteryCapacityCharge/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(ChargeTimeSec);
			}
        	Delay_mSec(MENUDELAY);
        }

//DISCHARGE menu DISCHARGE menu DISCHARGE menu DISCHARGE menu DISCHARGE menu DISCHARGE menu DISCHARGE menu
        if (MenuLevel->TypeOfMenu == DISCHARGE)
        {

        	entered_in_charge_discharge_menu = 1;
        	if (InitiStatus==0)
        	{
        		InitiStatus = 1;
        		ChargeStatusForTimer = 0;
        		DisChargeStatusForTimer = 1;
				BatteryCapacityDischargeCurrent = 0;
				DischargeTimeSec = 0;
				BatteryLow=0;
				//discharge();
        	}

    		U_BatteryTmp = U_OUT;
    		//low bat
    		if (U_BatteryTmp >=Low_Voltage) BatteryLow=0;
    		if (U_BatteryTmp < Low_Voltage)
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
			if (Button == KEY_NEXT)
			{
	        	if (On_off == 0)
	        		discharge();
	        	else
	        		OFF();
			}
			if (Button == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAXITEM2-1;
			}
			if(CountShow == 0)
			{
				lcd_set_xy(0,0);
				//PrintOnLCD(" ");
				itoa_koma(U_OUT,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");

				itoa(Current,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA    ");

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
				PrintOnLCDFull("OffWhen Vb<");
				itoa_koma(Low_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V   ");

			}
			if(CountShow == 2)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("P ");
				itoa(BatteryCapacityDischargePrevious/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec_Previous);

			}
			if(CountShow == 3)
			{
				lcd_set_xy(0,0);
				PrintOnLCDFull("N ");
				itoa(BatteryCapacityDischargeCurrent/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec);
			}
        	Delay_mSec(MENUDELAY);

        }

        //Training menu Training menu Training menu Training menu Training menu Training menu Training menu Training menu
        if (MenuLevel->TypeOfMenu == TRAINING)
        {

        	entered_in_charge_discharge_menu = 1;
        	if (InitiStatus==0)
        	{
    			//InitiStatus
    			if (U_OUT >=Low_Voltage)
    			{
    				BATERYSTATUS = 1;//1 discahrge
    				discharge();
    				DischargeTimeSec = 0;
    				BatteryCapacityDischargeCurrent = 0;
    				DisChargeStatusForTimer = 1;
    			}
    			else
    			{
    				BATERYSTATUS = 0;//1 charge
    				ChargeTimeSec = 0;
    				BatteryCapacityCharge = 0;
    				ChargeStatusForTimer = 1;
    				charge();
    			}
    			InitiStatus = 1;
    	    	Print_to_USART1("Training Init ");
        	}


        	if (BATERYSTATUS == 1)
        	{
        		ChargeStatusForTimer = 0;
        		DisChargeStatusForTimer = 1;
    			U_BatteryTmp = U_OUT;
    			//low bat
    			if (U_BatteryTmp >=Low_Voltage) BatteryLow=0;
    			if (U_BatteryTmp < Low_Voltage)
    			{
    				if (BatteryLow == 0) U_Battery_Timer = time_sec;
    				BatteryLow = 1;
    			}
    			if ((time_sec - U_Battery_Timer)>3 && BatteryLow !=0)//was 10
    			{
     				charge();
    				BATERYSTATUS = 0;
    				ChargeTimeSec = 0;
    				BatteryCapacityCharge = 0;
    			}
        	}
        	else
        	{
    	    	DisChargeStatusForTimer = 0;
    	    	ChargeStatusForTimer = 1;
            	if ((ChargeTimeSec > ChargeDurationSec)|| (U_OUT> Max_Voltage))
    			{
    				discharge();
    				BATERYSTATUS = 1;
    				BatteryLow=0;
    				DischargeTimeSec_Previous = DischargeTimeSec;
    				DischargeTimeSec = 0;
    				BatteryCapacityDischargePrevious = BatteryCapacityDischargeCurrent;
    				//SaveData.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
    				//EEpromWrite();
    				BatteryCapacityDischargeCurrent = 0;

    			}
        	}


        	//Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu Menu
			#define MAXITEM3 5
			if (Button == KEY_NEXT)
			{
				CountShow++;
				if (CountShow==MAXITEM3) CountShow=0;
			}
			if (Button == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAXITEM3-1;
			}
			if(CountShow == 0)
			{
				lcd_set_xy(0,0);
				itoa_koma(U_OUT,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");

				itoa(Current,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA    ");
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
				itoa(SaveData.Value,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("h ");
				itoa_koma(Low_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
				itoa_koma(Max_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V     ");


			}
			if(CountShow == 2)
			{
				lcd_set_xy(0,0);
				LcdOutbyNumber(2,1);//discharge
				lcd_set_xy(1,0);
				PrintOnLCDFull("N ");
				itoa(BatteryCapacityDischargeCurrent/3600,s);
				//itoa(BatteryCapacityDischargeCurrent,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec);
			}
			if(CountShow == 3)
			{
				lcd_set_xy(0,0);
				LcdOutbyNumber(2,1);//discharge
				lcd_set_xy(1,0);
				PrintOnLCDFull("P ");
				itoa(BatteryCapacityDischargePrevious/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec_Previous);

			}

			if(CountShow == 4)
			{
				lcd_set_xy(0,0);
				LcdOutbyNumber(0,1);//charge
				lcd_set_xy(1,0);
				PrintOnLCDFull("C ");
				itoa(BatteryCapacityCharge/3600,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mAH ");
				lcd_set_xy(3,1);
				ClockOnLCD_noSec(ChargeTimeSec);
			}
        	Delay_mSec(MENUDELAY);

        }

// swing swing swing swing swing swing swing swing swing
        if (MenuLevel->TypeOfMenu == SWING)
        {
        	lcd_set_xy(0,0);
           	entered_in_charge_discharge_menu = 1;
           	if (InitiStatus==0)
            {
           		InitiStatus=1;
           		Start_Timer_sec();
            }

           if (Timer_Sec<=SwingChargeTime)
           {
        	   charge();
           }
           if (Timer_Sec > SwingChargeTime)
           {
        	   discharge();
           }
           if (Timer_Sec > (SwingChargeTime+SwingDChargeTime))
        	   ReStart_Timer_sec();
           if (U_OUT<Low_Voltage)
        	   ReStart_Timer_sec();

           if (U_OUT>Max_Voltage)
           {
        	   ReStart_Timer_sec();
               Timer_Sec  = Timer_Sec+ SwingChargeTime;
           }


			#define MAXITEM6 3
			if (Button == KEY_NEXT)
			{
				CountShow++;
				if (CountShow==MAXITEM6) CountShow=0;
			}
			if (Button == KEY_BACK)
			{
				CountShow--;
				if (CountShow<0) CountShow=MAXITEM6-1;
			}
			if(CountShow == 0)
			{
				lcd_set_xy(0,0);
				itoa_koma(U_OUT,s,2);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");

				itoa(Current,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("mA    ");

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
				itoa(SaveData.Value,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("h ");
				itoa_koma(Low_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V ");
				itoa_koma(Max_Voltage/10,s,1);
				PrintOnLCDFull(s);
				PrintOnLCDFull("V     ");

			}
			if(CountShow == 2)
			{
				lcd_set_xy(0,0);
				itoa(SwingChargeTime,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("s ");

				itoa(SwingDChargeTime,s);
				PrintOnLCDFull(s);
				PrintOnLCDFull("s   ");


				lcd_set_xy(3,1);
				ClockOnLCD_noSec(DischargeTimeSec);
			}

        	Delay_mSec(MENUDELAY);
        }


        if (entered_in_charge_discharge_menu == 0)
        {
        	OFF();
        	InitiStatus = 0;
        	CountShow = 0;
        	BatteryCapacityDischargePrevious = BatteryCapacityDischargeCurrent;
        	DischargeTimeSec_Previous = DischargeTimeSec;
        }

       // Delay_ms(5);
        Delay_mSec(100);
    }//while
}//main

//================EEprom===========================================
void EEpromWrite(void)
{

	flash_unlock();
	flash_erase_page( SaveData.DefaultValuesFromEEpromOrNotAfterBurning_AddresInEEprom);
	flash_write( SaveData.DefaultValuesFromEEpromOrNotAfterBurning_AddresInEEprom, SaveData.DefaultValuesFromEEpromOrNotAfterBurning);
	flash_write( SaveData.Option1_AddresInEEprom, SaveData.Option1);
	flash_write( SaveData.Value_AddresInEEprom, SaveData.Value);
	flash_write( SaveData.BatteryCapacityDischargePrevious_AddresInEEprom, SaveData.BatteryCapacityDischargePreviousValue);
	flash_write( SaveData.LowVoltage_AddresInEEprom, SaveData.LowVoltage);
	flash_write( SaveData.MaxVoltage_AddresInEEprom, SaveData.MaxVoltage);
	flash_write( SaveData.Swing_Chrg_time_AddresInEEprom, SaveData.Swing_Chrg_time);
	flash_write( SaveData.Swing_DChrg_time_AddresInEEprom, SaveData.Swing_DChrg_time);
	flash_write( SaveData.BatteryCapacityDischargeCurrent_AddresInEEprom, SaveData.BatteryCapacityDischargeCurrent);

	flash_write( SaveData.Calibration0ValueForCurrent_AddresInEEprom, SaveData.Calibration0ValueForCurrent);
	flash_write( SaveData.Calibration0ValueForCurrent1_AddresInEEprom, SaveData.Calibration0ValueForCurrent1);
	flash_write( SaveData.CalibrationValueForCurrent_AddresInEEprom, SaveData.CalibrationValueForCurrent);
	flash_write( SaveData.CalibrationValueForCurrent1_AddresInEEprom, SaveData.CalibrationValueForCurrent1);
	flash_write( SaveData.CalibrationValueForVoltage_AddresInEEprom, SaveData.CalibrationValueForVoltage);
	flash_write( SaveData.CalibrationValueForVoltage1_AddresInEEprom, SaveData.CalibrationValueForVoltage1);
	flash_write( SaveData.CalibrationValueForVoltage2_AddresInEEprom, SaveData.CalibrationValueForVoltage2);
	flash_write( SaveData.ChargeAdapt_AddresInEEprom, SaveData.ChargeAdapt);
	flash_write( SaveData.ResistanceComp_AddresInEEprom, SaveData.ResistanceComp);

	flash_lock();
	Print_to_USART1("EEprom write ");
}
void FactoryEEpromWrite(void)
{

	flash_unlock();
	flash_erase_page( FactorySaveData.DefaultValuesFromEEpromOrNotAfterBurning_AddresInEEprom);
	flash_write( FactorySaveData.DefaultValuesFromEEpromOrNotAfterBurning_AddresInEEprom, FactorySaveData.DefaultValuesFromEEpromOrNotAfterBurning);
	flash_write( FactorySaveData.Option1_AddresInEEprom, FactorySaveData.Option1);
	flash_write( FactorySaveData.Value_AddresInEEprom, FactorySaveData.Value);
	flash_write( FactorySaveData.BatteryCapacityDischargePrevious_AddresInEEprom, FactorySaveData.BatteryCapacityDischargePreviousValue);
	flash_write( FactorySaveData.LowVoltage_AddresInEEprom, FactorySaveData.LowVoltage);
	flash_write( FactorySaveData.MaxVoltage_AddresInEEprom, FactorySaveData.MaxVoltage);
	flash_write( FactorySaveData.Swing_Chrg_time_AddresInEEprom, FactorySaveData.Swing_Chrg_time);
	flash_write( FactorySaveData.Swing_DChrg_time_AddresInEEprom, FactorySaveData.Swing_DChrg_time);
	flash_write( FactorySaveData.BatteryCapacityDischargeCurrent_AddresInEEprom, FactorySaveData.BatteryCapacityDischargeCurrent);

	flash_write( FactorySaveData.Calibration0ValueForCurrent_AddresInEEprom, FactorySaveData.Calibration0ValueForCurrent);
	flash_write( FactorySaveData.Calibration0ValueForCurrent1_AddresInEEprom, FactorySaveData.Calibration0ValueForCurrent1);
	flash_write( FactorySaveData.CalibrationValueForCurrent_AddresInEEprom, FactorySaveData.CalibrationValueForCurrent);
	flash_write( FactorySaveData.CalibrationValueForCurrent1_AddresInEEprom, FactorySaveData.CalibrationValueForCurrent1);
	flash_write( FactorySaveData.CalibrationValueForVoltage_AddresInEEprom, FactorySaveData.CalibrationValueForVoltage);
	flash_write( FactorySaveData.CalibrationValueForVoltage1_AddresInEEprom, FactorySaveData.CalibrationValueForVoltage1);
	flash_write( FactorySaveData.CalibrationValueForVoltage2_AddresInEEprom, FactorySaveData.CalibrationValueForVoltage2);
	flash_write( FactorySaveData.ChargeAdapt_AddresInEEprom, FactorySaveData.ChargeAdapt);
	flash_write( FactorySaveData.ResistanceComp_AddresInEEprom, FactorySaveData.ResistanceComp);

	flash_lock();
	Print_to_USART1("FactoryEEpromWrite write ");
}

void UpdateDataFromMenuToEEpromAndSaveData(void)
{
	//Updating option from menu to SaveData
    struct menu_item *MenuLevelFind = &Menu[0];
    /*char SetOption=1;
    MenuLevelFind = MenuLevelFind->Child;
    while((MenuLevelFind->SelectValues!=SET))
    {
        MenuLevelFind=MenuLevelFind->Next;
        SetOption++;
        if (SetOption>200) break;
    }
    SaveData.Option1=SetOption;
*/
	//Updating value from menu to SaveData
    MenuLevelFind=&Menu[2];
    SaveData.Value = MenuLevelFind->SelectValues;

    MenuLevelFind=&Menu[4];
    SaveData.LowVoltage = MenuLevelFind->SelectValues;

    MenuLevelFind=&Menu[6];
    SaveData.MaxVoltage = MenuLevelFind->SelectValues;

    MenuLevelFind=&Menu[8];
    SaveData.Swing_Chrg_time = MenuLevelFind->SelectValues;

    MenuLevelFind=&Menu[10];
    SaveData.Swing_DChrg_time = MenuLevelFind->SelectValues;




    EEpromWrite();//saving options&values from SaveData to EEprom
	Print_to_USART1("UpdateDataFromMenuToEEpromAndSaveData");
}

int ReadFromEEprom()
{

		SaveData.Option1 =  flash_read(SaveData.Option1_AddresInEEprom);
		SaveData.Value =  flash_read(SaveData.Value_AddresInEEprom);
		Print_to_USART1_d(SaveData.Value,"value=  ",0);
		SaveData.BatteryCapacityDischargePreviousValue =  flash_read(SaveData.BatteryCapacityDischargePrevious_AddresInEEprom);
		SaveData.LowVoltage =  flash_read(SaveData.LowVoltage_AddresInEEprom);
		SaveData.MaxVoltage =  flash_read(SaveData.MaxVoltage_AddresInEEprom);
		SaveData.Swing_Chrg_time =  flash_read(SaveData.Swing_Chrg_time_AddresInEEprom);
		SaveData.Swing_DChrg_time =  flash_read(SaveData.Swing_DChrg_time_AddresInEEprom);
		SaveData.BatteryCapacityDischargeCurrent = flash_read(SaveData.BatteryCapacityDischargeCurrent_AddresInEEprom);
		SaveData.Calibration0ValueForCurrent = flash_read(SaveData.Calibration0ValueForCurrent_AddresInEEprom);
		SaveData.Calibration0ValueForCurrent1 = flash_read(SaveData.Calibration0ValueForCurrent1_AddresInEEprom);
		SaveData.CalibrationValueForCurrent = flash_read(SaveData.CalibrationValueForCurrent_AddresInEEprom);
		SaveData.CalibrationValueForCurrent1 = flash_read(SaveData.CalibrationValueForCurrent1_AddresInEEprom);
		SaveData.CalibrationValueForVoltage = flash_read(SaveData.CalibrationValueForVoltage_AddresInEEprom);
		SaveData.CalibrationValueForVoltage1 = flash_read(SaveData.CalibrationValueForVoltage1_AddresInEEprom);
		SaveData.CalibrationValueForVoltage2 = flash_read(SaveData.CalibrationValueForVoltage2_AddresInEEprom);
		SaveData.ChargeAdapt = flash_read(SaveData.ChargeAdapt_AddresInEEprom);
		SaveData.ResistanceComp = flash_read(SaveData.ResistanceComp_AddresInEEprom);

}
void UpdateMenuFromEEprom()
{
	ReadFromEEprom();//Reading options&values from EEprom to SaveData

	//Updating option from SaveData to Menu
    struct menu_item *MenuLevelFind = &Menu[0];
   /* MenuLevelFind = MenuLevelFind->Child;
    char SetOption=1;
    while((MenuLevelFind->SelectValues!=SET))
    {
        MenuLevelFind=MenuLevelFind->Next;
        SetOption++;
        if (SetOption>200) break;
    }
    MenuLevelFind->SelectValues = UNSET;
    SetOption = 1;
    MenuLevelFind = &Menu[11];
    MenuLevelFind = MenuLevelFind->Child;
    while(SetOption!= SaveData.Option1)
    {
        MenuLevelFind=MenuLevelFind->Next;
        SetOption++;
        if (SetOption>200) break;
    }
    MenuLevelFind->SelectValues = SET;
*/
	//Updating value from SaveData to Menu
    MenuLevelFind=&Menu[2];//CHARGE_TIME
    MenuLevelFind->SelectValues = SaveData.Value;

    MenuLevelFind=&Menu[4];//CHARGE_TIME
    MenuLevelFind->SelectValues = SaveData.LowVoltage;

    MenuLevelFind=&Menu[6];//CHARGE_TIME
    MenuLevelFind->SelectValues = SaveData.MaxVoltage;

    MenuLevelFind=&Menu[8];//CHARGE_TIME
    MenuLevelFind->SelectValues = SaveData.Swing_Chrg_time;

    MenuLevelFind=&Menu[10];//CHARGE_TIME
    MenuLevelFind->SelectValues = SaveData.Swing_DChrg_time;


    Calibration0ValueForCurrent = SaveData.Calibration0ValueForCurrent;
    Calibration0ValueForCurrent1 = SaveData.Calibration0ValueForCurrent1;
    CalibrationValueForCurrent=SaveData.CalibrationValueForCurrent;
    CalibrationValueForCurrent1=SaveData.CalibrationValueForCurrent1;
    CalibrationValueForVoltage=SaveData.CalibrationValueForVoltage;//U_PS
    CalibrationValueForVoltage1=SaveData.CalibrationValueForVoltage1;//U_OUT
    CalibrationValueForVoltage2=SaveData.CalibrationValueForVoltage2;//U_OUT
    ChargeAdapt = SaveData.ChargeAdapt;
    ResistanceComp = SaveData.ResistanceComp;

	Print_to_USART1("UpdateMenuFromEEprom");
}


//================End EEprom===========================================

void InitLCD()
{

	GPIOB->CRH &= ~GPIO_CRH_MODE10; //set 00 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH |= GPIO_CRH_MODE10_1;//set 10 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH &= ~GPIO_CRH_CNF10;  //set 00  Mode_Out_OD = 01
	GPIOB->CRH |= GPIO_CRH_CNF10_0; //set 01  Mode_Out_OD = 01

	GPIOB->CRH &= ~GPIO_CRH_MODE11; //set 00 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH |= GPIO_CRH_MODE11_1;//set 10 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH &= ~GPIO_CRH_CNF11;  //set 00  Mode_Out_OD = 01
	GPIOB->CRH |= GPIO_CRH_CNF11_0; //set 01  Mode_Out_OD = 01

	GPIOB->CRH &= ~GPIO_CRH_MODE12; //set 00 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH |= GPIO_CRH_MODE12_1;//set 10 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH &= ~GPIO_CRH_CNF12;  //set 00  Mode_Out_OD = 01
	GPIOB->CRH |= GPIO_CRH_CNF12_0; //set 01  Mode_Out_OD = 01

	GPIOB->CRH &= ~GPIO_CRH_MODE13; //set 00 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH |= GPIO_CRH_MODE13_1;//set 10 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH &= ~GPIO_CRH_CNF13;  //set 00  Mode_Out_OD = 01
	GPIOB->CRH |= GPIO_CRH_CNF13_0; //set 01  Mode_Out_OD = 01

	GPIOB->CRH &= ~GPIO_CRH_MODE14; //set 00 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH |= GPIO_CRH_MODE14_1;//set 10 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH &= ~GPIO_CRH_CNF14;  //set 00  Mode_Out_OD = 01
	GPIOB->CRH |= GPIO_CRH_CNF14_0; //set 01  Mode_Out_OD = 01

	GPIOB->CRH &= ~GPIO_CRH_MODE15; //set 00 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH |= GPIO_CRH_MODE15_1;//set 10 MODEy[1:0] = 10 2Mgz;
	GPIOB->CRH &= ~GPIO_CRH_CNF15;  //set 00  Mode_Out_OD = 01
	GPIOB->CRH |= GPIO_CRH_CNF15_0; //set 01  Mode_Out_OD = 01

	lcd_init();
	lcd_clear();
	lcd_set_state(LCD_ENABLE, CURSOR_DISABLE, NO_BLINK);
}

void Init_Out()
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



uint16_t comp = 0;
void BUT_Debrief(void)
{
	uint8_t key;

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

uint8_t BUT_GetKey(void)
{
	if (pressedKey) PowerOffTimesec=0;
	uint8_t key = pressedKey;
	pressedKey = 0;
	return key;
}



uint8_t strlen1(uint8_t string[])
{
	uint8_t i=0;
	while(string[i]!='\0')
		i++;
	return i;
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

void TIM7_IRQHandler()
{




  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    BUT_Debrief();
    All_OUT_OFF_When_Power_OFF();


  }
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

void TIM1_UP_TIM16_IRQHandler()
{
  if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM16, TIM_IT_Update);


	//capacity
	if (Current < 1)
		BatteryCapacityDischargeCurrent = BatteryCapacityDischargeCurrent + Module16(Current);
	if (Current > 1)
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
	Timer_Sec = 0;
	Status_Timer_Sec = 1;

}
void Stop_Timer_sec()
{

	Status_Timer_Sec = 0;

}

void PrintOnLCD(uint8_t* str)
{
	PrintOnLCD_(str, 1);
	//Transmit(str);
	//Print_to_USART1(str);


}
void PrintOnLCDFull(uint8_t* str)
{
	PrintOnLCD_(str, 2);
	//Transmit(str);

}

void PrintOnLCD_xy(uint8_t* str ,uint8_t x, uint8_t y)
{
	PrintOnLCD_xy_(str,x,y);
	//Transmit(str);

}

void ClockOnLCD (uint32_t time)
{
	uint8_t string[16];
	//(" ",1);
	itoa(time/3600,string);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

	lcd_out(":",1);
	itoa((time/60)%60,string);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

	lcd_out(":",1);
	itoa(time%60,string);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

}
void ClockOnLCD_noSec (uint32_t time)
{
	uint8_t string[16];
	//(" ",1);
	itoa(time/3600,string);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);

	lcd_out(":",1);
	itoa((time/60)%60,string);
	if (strlen1(string)<2) lcd_out("0",1);
	lcd_out(string,1);


}
void Power_Mosfets_1Wire(uint8_t state)
{
	if (state == 0)
	{
		if (EEpromSaveStatus == 0)
		{
			if (BatteryCapacityDischargeCurrent/360 > 100)
			{
				SaveData.BatteryCapacityDischargePreviousValue = BatteryCapacityDischargeCurrent;
				EEpromWrite();
				Print_to_USART1("Data saved ");
			}
		}
    	EEpromSaveStatus = 1;
    	//GPIOA->BSRR =  GPIO_BSRR_BR5; //Mosfet, S - ON, R - OFF
    	//GPIOA->BSRR =  GPIO_BSRR_BR6; //Mosfet, S - ON, R - OFF
		//GPIOC->BSRR =  GPIO_BSRR_BR15;//1WIRE,  S - ON, R - OFF
		//GPIOC->BSRR =  GPIO_BSRR_BR14;//Power,  S - ON, R - OFF
	}else
	{
		//GPIOC->BSRR =  GPIO_BSRR_BS14;//Power,  S - ON, R - OFF
    	//GPIOA->BSRR =  GPIO_BSRR_BS5; //Mosfet, S - ON, R - OFF
    	//GPIOA->BSRR =  GPIO_BSRR_BS6; //Mosfet, S - ON, R - OFF
		//GPIOC->BSRR =  GPIO_BSRR_BS15;//1WIRE,  S - ON, R - OFF
	}
}

void All_OUT_OFF_When_Power_OFF()
{
	//Print_to_USART1_d(U_IN,"U1 ",0);
	if (U_IN < 1400)
	{
		//Print_to_USART1_d(U_IN,"U2 ",0);
		OFF();
		if (U_12_Status == 0)
		{
			//OFF();

			U_12_Status = 1;
			SaveData.BatteryCapacityDischargeCurrent = BatteryCapacityDischargeCurrent;
			EEpromWrite();
			Print_to_USART1_d(U_IN,"U off ",0);
		}
		//Print_to_USART1("<9v");



	}
	else
	{
		U_12_Status = 0;

	}
}
int16_t Module16 (int16_t value)
{
	if (value == 0) return 0;
	if (value > 0) return value;
	if (value < 0) return (-1)*value;
}

int8_t Module8 (int8_t value)
{
	if (value == 0) return 0;
	if (value > 0) return value;
	if (value < 0) return (-1)*value;
}
int32_t Module32 (int32_t value)
{
	if (value == 0) return 0;
	if (value > 0) return value;
	if (value < 0) return (-1)*value;
}

void charge()
{
	LOAD1_OFF();
	LOAD2_OFF();
	OUT_ON();
}

void discharge()
{
	OUT_OFF();
	LOAD1_ON();

}
void OFF()
{
	LOAD1_OFF();
	LOAD2_OFF();
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
	GPIOB->BSRR =  GPIO_BSRR_BR1; //load1

	GPIOA->BSRR =  GPIO_BSRR_BR8;//led out on/off

	On_off = 0;
	Status_Out = 0;

}
void OUT_ON()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	GPIOB->BSRR =  GPIO_BSRR_BS0;//ON-OFF OUT
	GPIOB->BSRR =  GPIO_BSRR_BR1; //load1
	On_off = 1;
	Status_Out = 1;
	GPIOA->BSRR =  GPIO_BSRR_BS8;//led out on/off
}
void OUT_ON_OFF_Toggle()
{
	if (Status_Out ==0)
		OUT_ON();
	else OUT_OFF();
}

void LOAD2_ON()
{

}
void LOAD2_OFF()
{



}
void LOAD1_ON()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	GPIOB->BSRR =  GPIO_BSRR_BR0;//ON-OFF OUT
	GPIOB->BSRR =  GPIO_BSRR_BS1; //load1
	On_off = 1;
	Status_Load = 1;
	GPIOA->BSRR =  GPIO_BSRR_BS11;//led load on/off
}

void LOAD1_OFF()
{
	//GPIOB->BSRR =  GPIO_BSRR_BR0;//Diode 1 OUT ON//OFF
	GPIOB->BSRR =  GPIO_BSRR_BR0;//ON-OFF OUT
	GPIOB->BSRR =  GPIO_BSRR_BR1; //load1
	On_off = 0;
	Status_Load = 0;
	GPIOA->BSRR =  GPIO_BSRR_BR11;//led load on/off
}
void LOAD1_ON_OFF_Toggle()
{
	if (Status_Load == 0)
		LOAD1_ON();
	else LOAD1_OFF();
}
void FAN_ON()
{


}
void FAN_OFF()
{

}
