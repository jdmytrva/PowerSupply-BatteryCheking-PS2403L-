#include <stm32f10x.h>
#include "usart.h"
#ifndef _GENERAL_
#define _GENERAL_



struct StructCalibrationValuetoSaveInFlash
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

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

	uint32_t ResistanceComp_Ishunt_Wires;
	uint32_t ResistanceComp_Ishunt_Wires_AddresInEEprom;

	uint32_t ResistanceComp_MOSFET;
	uint32_t ResistanceComp_MOSFET_AddresInEEprom;
};

struct StructSettingsValuetoSaveInFlash
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t Option1;
	uint32_t Option1_AddresInEEprom;
//
	uint32_t ChargeTime;
	uint32_t ChargeTime_AddresInEEprom;

	uint32_t LowVoltage;
	uint32_t LowVoltage_AddresInEEprom;

	uint32_t MaxVoltage;
	uint32_t MaxVoltage_AddresInEEprom;

	uint32_t Swing_Chrg_time;
	uint32_t Swing_Chrg_time_AddresInEEprom;

	uint32_t Swing_DChrg_time;
	uint32_t Swing_DChrg_time_AddresInEEprom;

	uint32_t ChargeAdapt;
	uint32_t ChargeAdapt_AddresInEEprom;

	uint32_t Option2;
	uint32_t Option2_AddresInEEprom;

};
struct StructValuetoSaveInFlashWhenPowerOFF
{
	uint32_t CRC_data;
	uint32_t CRC_AddresInEEprom;

	uint32_t BatteryCapacityDischargePreviousValue;
	uint32_t BatteryCapacityDischargePrevious_AddresInEEprom;

	uint32_t BatteryCapacityDischargeCurrent;
	uint32_t BatteryCapacityDischargeCurrent_AddresInEEprom;

	uint32_t OutState;
	uint32_t OutState_AddresInEEprom;
};


struct StructCalibrationValuetoSaveInFlash CalibrationData;
struct StructCalibrationValuetoSaveInFlash CalibrationDataForVerify;
struct StructCalibrationValuetoSaveInFlash CalibrationDataFactory;

struct StructSettingsValuetoSaveInFlash SettingsData;
struct StructSettingsValuetoSaveInFlash SettingsDataForVerify;
struct StructSettingsValuetoSaveInFlash SettingsDataFactory;

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOff;
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffForVerify;
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffFactory;

struct StructTemperatureLinearTable
{
	int16_t T;
	int16_t R;
};
struct StructTemperature_R_A_B
{
	int32_t R;
	int32_t A10000;
	int32_t B;
};
#define TLT 8
struct StructTemperatureLinearTable T_Table[TLT];
struct StructTemperature_R_A_B Temp_R_A_B[TLT];

//ADC
volatile uint16_t U_OUT_ForSetResistance;
volatile int16_t Current_RAW;
volatile int16_t Current_load ;
volatile int16_t Current_Out ;
volatile int32_t Temperature;
volatile int32_t Rt;

#define MAX_LOG_ITEMS 24

struct StructLoggingData
{
	uint32_t RecordsQuantity;
	char Records[MAX_LOG_ITEMS][17];
};
struct StructLoggingData LoggingData;
void WriteInLOG(char  str[17]);
void WriteInLOGc(char  str [17],char c);


void InfoToUARTBeforeStart(void);
void Generation_Coefficients_R_A_B();
int32_t GetTemperature(int32_t Rtemp);


#endif
