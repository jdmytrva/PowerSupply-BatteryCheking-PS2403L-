#include "General.h"
#define ADDRESS_FLASH FLASH_BASE+1024*124
struct StructOptionValuetoSaveInEEprom SaveData=
{
		0,
		ADDRESS_FLASH,
		0,
		ADDRESS_FLASH+4,
		0,
		ADDRESS_FLASH+8,
		0,
		ADDRESS_FLASH+12,
		0,
		ADDRESS_FLASH+16,
		0,
		ADDRESS_FLASH+20,
		0,
		ADDRESS_FLASH+24,
		0,
		ADDRESS_FLASH+28,
		0,
		ADDRESS_FLASH+32,
		0,
		ADDRESS_FLASH+36,
		0,
		ADDRESS_FLASH+40,
		0,
		ADDRESS_FLASH+44,
		0,
		ADDRESS_FLASH+48,
		0,
		ADDRESS_FLASH+52,
		0,
		ADDRESS_FLASH+56,
		0,
		ADDRESS_FLASH+60,
		0,
		ADDRESS_FLASH+64,
		0,
		ADDRESS_FLASH+68
};
struct StructOptionValuetoSaveInEEprom FactorySaveData=
{
		1,
		ADDRESS_FLASH,
		1,//option
		ADDRESS_FLASH+4,
		15,//value
		ADDRESS_FLASH+8,
		0,//BatteryCapacityDischargePreviousValue
		ADDRESS_FLASH+12,
		1050,//LowVoltage
		ADDRESS_FLASH+16,
		1450,//MaxVoltage
		ADDRESS_FLASH+20,
		10,//Swing_Chrg_time
		ADDRESS_FLASH+24,
		10,//Swing_DChrg_time
		ADDRESS_FLASH+28,
		0,//BatteryCapacityDischargeCurrent
		ADDRESS_FLASH+32,
		0,//Calibration0ValueForCurrent
		ADDRESS_FLASH+36,
		0,//Calibration0ValueForCurrent1
		ADDRESS_FLASH+40,
		155,//CalibrationValueForCurrent
		ADDRESS_FLASH+44,
		155,//CalibrationValueForCurrent1
		ADDRESS_FLASH+48,
		1331,//CalibrationValueForVoltage
		ADDRESS_FLASH+52,
		1331,//CalibrationValueForVoltage1
		ADDRESS_FLASH+56,
		1331,//CalibrationValueForVoltage2
		ADDRESS_FLASH+60,

		10,//ChargeAdapt %
		ADDRESS_FLASH+64,
		131,//ResistanceComp_Ishunt_Wires mOm
		ADDRESS_FLASH+68

};
volatile uint16_t U_OUT_ForSetResistance=0;
volatile int16_t Current_load = 0;
volatile int16_t Current_Out = 0;

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
	Print_to_USART1_d(SaveData.ResistanceComp_Ishunt_Wires ,"ResistanceComp_Ishunt_Wires =",0) ;


}

