#include <stm32f10x.h>
#ifndef _EE_
#define _EE_
struct StructOptionValuetoSaveInEEpromAndMenu
{
	uint32_t DefaultValuesFromEEpromOrNotAfterBurning;
	uint32_t DefaultValuesFromEEpromOrNotAfterBurning_AddresInEEprom;

	uint32_t Option1;
	uint32_t Option1_AddresInEEprom;
//
	uint32_t Value;
	uint32_t Value_AddresInEEprom;

	uint32_t BatteryCapacityDischargePreviousValue;
	uint32_t BatteryCapacityDischargePrevious_AddresInEEprom;

	uint32_t LowVoltage;
	uint32_t LowVoltage_AddresInEEprom;

	uint32_t MaxVoltage;
	uint32_t MaxVoltage_AddresInEEprom;

	uint32_t Swing_Chrg_time;
	uint32_t Swing_Chrg_time_AddresInEEprom;

	uint32_t Swing_DChrg_time;
	uint32_t Swing_DChrg_time_AddresInEEprom;

	uint32_t BatteryCapacityDischargeCurrent;
	uint32_t BatteryCapacityDischargeCurrent_AddresInEEprom;

	uint32_t Calibration0ValueForCurrent;
	uint32_t Calibration0ValueForCurrent_AddresInEEprom;

	uint32_t Calibration0ValueForCurrent1;
	uint32_t Calibration0ValueForCurrent1_AddresInEEprom;

	uint32_t CalibrationValueForCurrent;
	uint32_t CalibrationValueForCurrent_AddresInEEprom;

	uint32_t CalibrationValueForCurrent1;
	uint32_t CalibrationValueForCurrent1_AddresInEEprom;

	uint32_t CalibrationValueForVoltage;
	uint32_t CalibrationValueForVoltage_AddresInEEprom;

	uint32_t CalibrationValueForVoltage1;
	uint32_t CalibrationValueForVoltage1_AddresInEEprom;

	uint32_t CalibrationValueForVoltage2;
	uint32_t CalibrationValueForVoltage2_AddresInEEprom;

	uint32_t ChargeAdapt;
	uint32_t ChargeAdapt_AddresInEEprom;
	uint32_t ResistanceComp;
	uint32_t ResistanceComp_AddresInEEprom;
};
struct StructOptionValuetoSaveInEEpromAndMenu SaveData;
struct StructOptionValuetoSaveInEEpromAndMenu FactorySaveData;
uint16_t EEpromSaveStatus;


//ADC
volatile uint16_t U_OUT_ForSetResistance;
volatile int16_t Current_RAW;
volatile int16_t Current_load ;
volatile int16_t Current_Out ;


void InfoToUARTBeforeStart(void);


#endif
