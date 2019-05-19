#include "General.h"
#define AddressFlash FLASH_BASE+1024*124
struct StructOptionValuetoSaveInEEpromAndMenu SaveData=
{
		1,//DefaultValuesFromEEpromOrNotAfterBurning; 0-from this structure, 1- from EEprom
		AddressFlash,
		1,//option
		AddressFlash+4,
		15,//value
		AddressFlash+8,
		0,//BatteryCapacityDischargePreviousValue
		AddressFlash+12,
		105,//LowVoltage
		AddressFlash+16,
		145,//MaxVoltage
		AddressFlash+20,
		10,//Swing_Chrg_time
		AddressFlash+24,
		10,//Swing_DChrg_time
		AddressFlash+28,
		0,//BatteryCapacityDischargeCurrent
		AddressFlash+32,
		0,//Calibration0ValueForCurrent
		AddressFlash+36,
		0,//Calibration0ValueForCurrent1
		AddressFlash+40,
		155,//CalibrationValueForCurrent
		AddressFlash+44,
		155,//CalibrationValueForCurrent1
		AddressFlash+48,
		1331,//CalibrationValueForVoltage
		AddressFlash+52,
		1331,//CalibrationValueForVoltage1
		AddressFlash+56,
		1331,//CalibrationValueForVoltage2
		AddressFlash+60,

		10,//ChargeAdapt %
		AddressFlash+64,
		131,//Resistance_Comp
		AddressFlash+68

};
struct StructOptionValuetoSaveInEEpromAndMenu FactorySaveData=
{
		1,//DefaultValuesFromEEpromOrNotAfterBurning; 0-from this structure, 1- from EEprom
		AddressFlash,
		1,//option
		AddressFlash+4,
		15,//value
		AddressFlash+8,
		0,//BatteryCapacityDischargePreviousValue
		AddressFlash+12,
		105,//LowVoltage
		AddressFlash+16,
		145,//MaxVoltage
		AddressFlash+20,
		10,//Swing_Chrg_time
		AddressFlash+24,
		10,//Swing_DChrg_time
		AddressFlash+28,
		0,//BatteryCapacityDischargeCurrent
		AddressFlash+32,
		0,//Calibration0ValueForCurrent
		AddressFlash+36,
		0,//Calibration0ValueForCurrent1
		AddressFlash+40,
		155,//CalibrationValueForCurrent
		AddressFlash+44,
		155,//CalibrationValueForCurrent1
		AddressFlash+48,
		1331,//CalibrationValueForVoltage
		AddressFlash+52,
		1331,//CalibrationValueForVoltage1
		AddressFlash+56,
		1331,//CalibrationValueForVoltage2
		AddressFlash+60,

		10,//ChargeAdapt %
		AddressFlash+64,
		131,//Resistance_Comp mOm
		AddressFlash+68

};
uint16_t EEpromSaveStatus=0;
volatile uint16_t U_OUT_ForSetResistance=0;
volatile int16_t Current_RAW=0;
volatile int16_t Current_load = 0;
volatile int16_t Current_Out = 0;
volatile int16_t Calibration0ValueForCurrent = 0;
volatile int16_t Calibration0ValueForCurrent1 = 0;
volatile int16_t CalibrationValueForCurrent=1250;
volatile int16_t CalibrationValueForCurrent1=1200;
volatile int16_t CalibrationValueForVoltage=1331;//U_PS
volatile int16_t CalibrationValueForVoltage1=1331;//U_OUT
volatile int16_t CalibrationValueForVoltage2=1331;//U_IN
volatile int16_t ChargeAdapt = 5;
volatile int16_t ResistanceComp= 1;

void InfoToUARTBeforeStart(void)
{

	Print_to_USART1_d(SaveData.DefaultValuesFromEEpromOrNotAfterBurning,"DefaultValuesFromEEpromOrNotAfterBurning =",0);
	Print_to_USART1_d(SaveData.Option1 ,"Option1 =",0);
	Print_to_USART1_d(SaveData.Value ,"Value =",0);
	Print_to_USART1_d(SaveData.BatteryCapacityDischargePreviousValue ,"BatteryCapacityDischargePreviousValue =",0);
	Print_to_USART1_d(SaveData.LowVoltage ,"LowVoltage =",0);
	Print_to_USART1_d(SaveData.MaxVoltage ,"MaxVoltage =",0);
	Print_to_USART1_d(SaveData.Swing_Chrg_time ,"Swing_Chrg_time =",0);
	Print_to_USART1_d(SaveData.Swing_DChrg_time ,"Swing_DChrg_time =",0);
	Print_to_USART1_d(SaveData.BatteryCapacityDischargeCurrent ,"BatteryCapacityDischargeCurrent =",0);
	Print_to_USART1_d(SaveData.Calibration0ValueForCurrent ,"Calibration0ValueForCurrent =",0);
	Print_to_USART1_d(SaveData.Calibration0ValueForCurrent1 ,"Calibration0ValueForCurrent1 =",0);
	Print_to_USART1_d(SaveData.CalibrationValueForCurrent ,"CalibrationValueForCurrent =",0);
	Print_to_USART1_d(SaveData.CalibrationValueForCurrent1 ,"CalibrationValueForCurrent1 =",0);
	Print_to_USART1_d(SaveData.CalibrationValueForVoltage ,"CalibrationValueForVoltage =",0);
	Print_to_USART1_d(SaveData.CalibrationValueForVoltage1 ,"CalibrationValueForVoltage1 =",0);
	Print_to_USART1_d(SaveData.CalibrationValueForVoltage2 ,"CalibrationValueForVoltage2 =",0);
	Print_to_USART1_d(SaveData.ChargeAdapt ,"ChargeAdapt =",0);
	Print_to_USART1_d(SaveData.ResistanceComp ,"ResistanceComp =",0) ;


}

