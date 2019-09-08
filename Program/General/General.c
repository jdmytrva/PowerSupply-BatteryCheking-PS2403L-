#include "General.h"
#define ADDRESS_FLASH_CALIBRATTION FLASH_BASE+1024*124
#define ADDRESS_FLASH_SETTINGS     FLASH_BASE+1024*123
#define ADDRESS_FLASH_WHEN_OFF     FLASH_BASE+1024*122


struct StructCalibrationValuetoSaveInFlash CalibrationData=
{
		0,
		ADDRESS_FLASH_CALIBRATTION,
		0,
		ADDRESS_FLASH_CALIBRATTION+4,
		0,
		ADDRESS_FLASH_CALIBRATTION+8,
		0,
		ADDRESS_FLASH_CALIBRATTION+12,
		0,
		ADDRESS_FLASH_CALIBRATTION+16,
		0,
		ADDRESS_FLASH_CALIBRATTION+20,
		0,
		ADDRESS_FLASH_CALIBRATTION+24,
		0,
		ADDRESS_FLASH_CALIBRATTION+28,
		0,
		ADDRESS_FLASH_CALIBRATTION+32,
		0,
		ADDRESS_FLASH_CALIBRATTION+36

};
struct StructCalibrationValuetoSaveInFlash CalibrationDataForVerify=
{
		0,
		ADDRESS_FLASH_CALIBRATTION,
		0,
		ADDRESS_FLASH_CALIBRATTION+4,
		0,
		ADDRESS_FLASH_CALIBRATTION+8,
		0,
		ADDRESS_FLASH_CALIBRATTION+12,
		0,
		ADDRESS_FLASH_CALIBRATTION+16,
		0,
		ADDRESS_FLASH_CALIBRATTION+20,
		0,
		ADDRESS_FLASH_CALIBRATTION+24,
		0,
		ADDRESS_FLASH_CALIBRATTION+28,
		0,
		ADDRESS_FLASH_CALIBRATTION+32,
		0,
		ADDRESS_FLASH_CALIBRATTION+36

};
struct StructCalibrationValuetoSaveInFlash CalibrationDataFactory=
{
		0,//CRC
		ADDRESS_FLASH_CALIBRATTION,
		0,//Calibration0ValueForCurrent
		ADDRESS_FLASH_CALIBRATTION+4,
		0,//Calibration0ValueForCurrent1
		ADDRESS_FLASH_CALIBRATTION+8,
		132,//CalibrationValueForCurrent
		ADDRESS_FLASH_CALIBRATTION+12,
		132,//CalibrationValueForCurrent1
		ADDRESS_FLASH_CALIBRATTION+16,
		1331,//CalibrationValueForVoltage
		ADDRESS_FLASH_CALIBRATTION+20,
		1328,//CalibrationValueForVoltage1
		ADDRESS_FLASH_CALIBRATTION+24,
		1331,//CalibrationValueForVoltage2
		ADDRESS_FLASH_CALIBRATTION+28,
		116,//ResistanceComp_Ishunt_Wires mOm
		ADDRESS_FLASH_CALIBRATTION+32,
		34,//ResistanceComp_MOSFET mOm
		ADDRESS_FLASH_CALIBRATTION+36
};
//=================================================

struct StructSettingsValuetoSaveInFlash SettingsData=
{
		0,
		ADDRESS_FLASH_SETTINGS,
		0,
		ADDRESS_FLASH_SETTINGS+4,
		0,
		ADDRESS_FLASH_SETTINGS+8,
		0,
		ADDRESS_FLASH_SETTINGS+12,
		0,
		ADDRESS_FLASH_SETTINGS+16,
		0,
		ADDRESS_FLASH_SETTINGS+20,
		0,
		ADDRESS_FLASH_SETTINGS+24,
		0,
		ADDRESS_FLASH_SETTINGS+28
};
struct StructSettingsValuetoSaveInFlash SettingsDataForVerify=
{
		0,
		ADDRESS_FLASH_SETTINGS,
		0,
		ADDRESS_FLASH_SETTINGS+4,
		0,
		ADDRESS_FLASH_SETTINGS+8,
		0,
		ADDRESS_FLASH_SETTINGS+12,
		0,
		ADDRESS_FLASH_SETTINGS+16,
		0,
		ADDRESS_FLASH_SETTINGS+20,
		0,
		ADDRESS_FLASH_SETTINGS+24,
		0,
		ADDRESS_FLASH_SETTINGS+28
};

struct StructSettingsValuetoSaveInFlash SettingsDataFactory=
{
		0,//CRC
		ADDRESS_FLASH_SETTINGS,
		1,//option1 -  Menu Position
		ADDRESS_FLASH_SETTINGS+4,
		15,//Charge time
		ADDRESS_FLASH_SETTINGS+8,
		1050,//LowVoltage
		ADDRESS_FLASH_SETTINGS+12,
		1450,//MaxVoltage
		ADDRESS_FLASH_SETTINGS+16,
		10,//Swing_Chrg_time
		ADDRESS_FLASH_SETTINGS+20,
		10,//Swing_DChrg_time
		ADDRESS_FLASH_SETTINGS+24,
		6,//ChargeAdapt %
		ADDRESS_FLASH_SETTINGS+28
};
//=====================================
struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOff=
{
		0,
		ADDRESS_FLASH_WHEN_OFF,
		0,
		ADDRESS_FLASH_WHEN_OFF+4,
		0,
		ADDRESS_FLASH_WHEN_OFF+8
};

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffForVerify=
{
		0,
		ADDRESS_FLASH_WHEN_OFF,
		0,
		ADDRESS_FLASH_WHEN_OFF+4,
		0,
		ADDRESS_FLASH_WHEN_OFF+8
};

struct StructValuetoSaveInFlashWhenPowerOFF SaveDataWhenPowerOffFactory=
{
		0,//CRC
		ADDRESS_FLASH_WHEN_OFF,
		0,//BatteryCapacityDischargePreviousValue
		ADDRESS_FLASH_WHEN_OFF+4,
		0,//BatteryCapacityDischargeCurrent
		ADDRESS_FLASH_WHEN_OFF+8
};


volatile uint16_t U_OUT_ForSetResistance=0;
volatile int16_t Current_load = 0;
volatile int16_t Current_Out = 0;


void WriteInLOG(char  str [17])
{
	static uint16_t i_log=0;
	uint8_t i;
	uint8_t j;

	char *number;
	number = itoa(LoggingData.RecordsQuantity);


	for(i=0;number[i]!='\0';i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = number[i];
	}
	LoggingData.Records[LoggingData.RecordsQuantity][i] = ':';
	i++;

	for(j=0;str[j]!='\0';i++,j++)
	{
		if (i<16) LoggingData.Records[LoggingData.RecordsQuantity][i] = str[j];
		else
		{
			LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';
			break;
		}
	}
	for (;i<16;i++)
	{
		LoggingData.Records[LoggingData.RecordsQuantity][i] = ' ';
	}
	Print_to_USART1_d(i,"ii: ",0);
	LoggingData.Records[LoggingData.RecordsQuantity][i] = '\0';

	LoggingData.RecordsQuantity++;
	if (LoggingData.RecordsQuantity>=(MAX_LOG_ITEMS))LoggingData.RecordsQuantity=0;
	flash_write_block();
	Print_to_USART1_d(LoggingData.RecordsQuantity,"Q: ",0);
}

void InfoToUARTBeforeStart(void)
{

	Print_to_USART1_d(CalibrationData.CRC_data,"CRC(Calibration) =",0);
	Print_to_USART1_d(CalibrationData.Calibration0ValueForCurrent ,"Calibration0ValueForCurrent =",0);
	Print_to_USART1_d(CalibrationData.Calibration0ValueForCurrent1 ,"Calibration0ValueForCurrent1 =",0);
	Print_to_USART1_d(CalibrationData.CalibrationValueForCurrent ,"CalibrationValueForCurrent =",0);
	Print_to_USART1_d(CalibrationData.CalibrationValueForCurrent1 ,"CalibrationValueForCurrent1 =",0);
	Print_to_USART1_d(CalibrationData.CalibrationValueForVoltage ,"CalibrationValueForVoltage =",0);
	Print_to_USART1_d(CalibrationData.CalibrationValueForVoltage1 ,"CalibrationValueForVoltage1 =",0);
	Print_to_USART1_d(CalibrationData.CalibrationValueForVoltage2 ,"CalibrationValueForVoltage2 =",0);
	Print_to_USART1_d(CalibrationData.ResistanceComp_Ishunt_Wires ,"ResistanceComp_Ishunt_Wires =",0) ;
	Print_to_USART1_d(CalibrationData.ResistanceComp_MOSFET ,"ResistanceComp_MOSFET =",0) ;

	Print_to_USART1_d(SettingsData.CRC_data,"CRC(SettingsData) =",0);
	Print_to_USART1_d(SettingsData.Option1 ,"Option1(menu position) =",0);
	Print_to_USART1_d(SettingsData.ChargeTime ,"ChargeTime =",0);
	Print_to_USART1_d(SettingsData.LowVoltage ,"LowVoltage =",0);
	Print_to_USART1_d(SettingsData.MaxVoltage ,"MaxVoltage =",0);
	Print_to_USART1_d(SettingsData.ChargeAdapt ,"ChargeAdapt =",0);
	Print_to_USART1_d(SettingsData.Swing_Chrg_time ,"Swing_Chrg_time =",0);
	Print_to_USART1_d(SettingsData.Swing_DChrg_time ,"Swing_DChrg_time =",0);

	Print_to_USART1_d(SaveDataWhenPowerOff.CRC_data,"CRC(SaveDataWhenPowerOff) =",0);
	Print_to_USART1_d(SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue ,"BatteryCapacityDischargePreviousValue =",0);
	Print_to_USART1_d(SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent ,"BatteryCapacityDischargeCurrent =",0);

}

