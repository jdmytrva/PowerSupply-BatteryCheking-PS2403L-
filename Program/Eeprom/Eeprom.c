#include "Eeprom.h"

///////////////////////////////////////////////////////
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
///////////////////////////////////////////////////////
static uint8_t EEpromReadStatus = 1;
static uint8_t EEpromWriteStatus = 1;

uint8_t flash_ready(void)
{
	return !(FLASH->SR & FLASH_SR_EOP);
}

void flash_erase_page(uint32_t address)
{
	while (FLASH->SR & FLASH_SR_BSY);

		if (FLASH->SR & FLASH_SR_EOP) {
			FLASH->SR = FLASH_SR_EOP;
		}

		FLASH->CR |= FLASH_CR_PER;
		FLASH->AR = address;
		FLASH->CR |= FLASH_CR_STRT;
		while (!(FLASH->SR & FLASH_SR_EOP));
		FLASH->SR = FLASH_SR_EOP;
		FLASH->CR &= ~FLASH_CR_PER;
}


void flash_unlock(void)
{
	  FLASH->KEYR = FLASH_KEY1;
	  FLASH->KEYR = FLASH_KEY2;
}

void flash_lock()
{
	FLASH->CR |= FLASH_CR_LOCK;
}



void flash_write(uint32_t address,uint32_t data)
{


	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PG;


    *(__IO uint16_t*)address = (uint16_t)data;
    while (!(FLASH->SR & FLASH_SR_EOP));
    		FLASH->SR = FLASH_SR_EOP;


	address+=2;
	data>>=16;
    *(__IO uint16_t*)address = (uint16_t)data;
    while (!(FLASH->SR & FLASH_SR_EOP));
    		FLASH->SR = FLASH_SR_EOP;

    FLASH->CR &= ~(FLASH_CR_PG);

}

uint32_t flash_read(uint32_t address, uint32_t MinValue, uint32_t MaxValue)
{
	uint32_t EEpromValue;
	EEpromValue = (*(__IO uint32_t*) address);
	if ((EEpromValue>=MinValue) && (EEpromValue<=MaxValue))
	{
		return EEpromValue;
	}
	else // fail to read
	{
		EEpromReadStatus = 0;
		Print_to_USART1_d(address,"Fail to read address: ",0);
		Print_to_USART1_d(EEpromValue,"EEpromValue is : ",0);
		Print_to_USART1_d(FLASH_BASE+1024*124,"FLASH_BASE+1024*124 : ",0);
		return MinValue;
	}
}

uint8_t EEpromWrite(void)
{
	__disable_irq();
	char CRC_Calibration=0;
	char CRC_CalibrationVerify=0;
	char *Data_Calibration = (unsigned char *) &CalibrationData;
	Data_Calibration = Data_Calibration+4;
	CRC_Calibration = CalcCRC8(Data_Calibration,((unsigned char)sizeof(CalibrationData))-4,0);
	Print_to_USART1_d(CRC_Calibration,"CRC is : ",0);
	Print_to_USART1_d(((unsigned char)sizeof(CalibrationData))-4,"Size is : ",0);
	SaveData.CRC_data = DaCRC_Calibrationta_crc;

	flash_unlock();
	CalibrationWriteToFlash();
	CalibrationReadFromFlashForVerify();
	char CRC_CalibrationVerify=0;
	char *Data_CalibrationVerify = (unsigned char *) &CalibrationDataForVerify;
	Data_CalibrationVerify = Data_CalibrationVerify+4;
	CRC_CalibrationVerify = CalcCRC8(Data_CalibrationVerify,((unsigned char)sizeof(CalibrationDataForVerify))-4,0);
	Print_to_USART1_d(CRC_CalibrationVerify,"CRC is : ",0);
	Print_to_USART1_d(((unsigned char)sizeof(CalibrationDataForVerify))-4,"Size is : ",0);



	flash_lock();
	Print_to_USART1("EEprom write Verify ");
	ReadFromEEpromVerify();

	char *Data1 = (unsigned char *) &CalibrationDataForVerify;
	Data1 = Data1+4;
	Data_crc1 = CalcCRC8(Data1,((unsigned char)sizeof(CalibrationDataForVerify))-4,0);
	Print_to_USART1_d(Data_crc1,"CRC is : ",0);
	Print_to_USART1_d(((unsigned char)sizeof(CalibrationDataForVerify))-4,"Size is : ",0);

	if (Data_crc == Data_crc1 )
	{
		Print_to_USART1("EEprom write success ");
	}else
	{
		Print_to_USART1("EEprom write Fail, try again... ");
		flash_unlock();


		flash_lock();
	}

	__enable_irq();

	return 1;
}
uint8_t CalibrationWriteToFlash(void)
{
	flash_erase_page( CalibrationData.CRC_AddresInEEprom);
	flash_write( CalibrationData.CRC_AddresInEEprom, CalibrationData.CRC_data);
	flash_write( CalibrationData.Calibration0ValueForCurrent_AddresInEEprom, CalibrationData.Calibration0ValueForCurrent);
	flash_write( CalibrationData.Calibration0ValueForCurrent1_AddresInEEprom, CalibrationData.Calibration0ValueForCurrent1);
	flash_write( CalibrationData.CalibrationValueForCurrent_AddresInEEprom, CalibrationData.CalibrationValueForCurrent);
	flash_write( CalibrationData.CalibrationValueForCurrent1_AddresInEEprom, CalibrationData.CalibrationValueForCurrent1);
	flash_write( CalibrationData.CalibrationValueForVoltage_AddresInEEprom, CalibrationData.CalibrationValueForVoltage);
	flash_write( CalibrationData.CalibrationValueForVoltage1_AddresInEEprom, CalibrationData.CalibrationValueForVoltage1);
	flash_write( CalibrationData.CalibrationValueForVoltage2_AddresInEEprom, CalibrationData.CalibrationValueForVoltage2);
	flash_write( CalibrationData.ResistanceComp_Ishunt_Wires_AddresInEEprom, CalibrationData.ResistanceComp_Ishunt_Wires);
	flash_write( CalibrationData.ResistanceComp_MOSFET_AddresInEEprom, CalibrationData.ResistanceComp_MOSFET);
	Print_to_USART1("CalibrationWriteToFlash ");

}
uint8_t CalibrationFactoryWriteToFlash(void)
{
	flash_erase_page( CalibrationDataFactory.CRC_AddresInEEprom);
	flash_write( CalibrationDataFactory.CRC_AddresInEEprom, CalibrationDataFactory.CRC_data);
	flash_write( CalibrationDataFactory.Calibration0ValueForCurrent_AddresInEEprom, CalibrationDataFactory.Calibration0ValueForCurrent);
	flash_write( CalibrationDataFactory.Calibration0ValueForCurrent1_AddresInEEprom, CalibrationDataFactory.Calibration0ValueForCurrent1);
	flash_write( CalibrationDataFactory.CalibrationValueForCurrent_AddresInEEprom, CalibrationDataFactory.CalibrationValueForCurrent);
	flash_write( CalibrationDataFactory.CalibrationValueForCurrent1_AddresInEEprom, CalibrationDataFactory.CalibrationValueForCurrent1);
	flash_write( CalibrationDataFactory.CalibrationValueForVoltage_AddresInEEprom, CalibrationDataFactory.CalibrationValueForVoltage);
	flash_write( CalibrationDataFactory.CalibrationValueForVoltage1_AddresInEEprom, CalibrationDataFactory.CalibrationValueForVoltage1);
	flash_write( CalibrationDataFactory.CalibrationValueForVoltage2_AddresInEEprom, CalibrationDataFactory.CalibrationValueForVoltage2);
	flash_write( CalibrationDataFactory.ResistanceComp_Ishunt_Wires_AddresInEEprom, CalibrationDataFactory.ResistanceComp_Ishunt_Wires);
	flash_write( CalibrationDataFactory.ResistanceComp_MOSFET_AddresInEEprom, CalibrationDataFactory.ResistanceComp_MOSFET);
	Print_to_USART1("CalibrationFactoryWriteToFlash ");
}
uint8_t CalibrationReadFromFlash(void)
{
	CalibrationData.CRC_data = flash_read(CalibrationData.CRC_AddresInEEprom,0,256);
	CalibrationData.Calibration0ValueForCurrent = flash_read(CalibrationData.Calibration0ValueForCurrent_AddresInEEprom,0,1000);
	CalibrationData.Calibration0ValueForCurrent1 = flash_read(CalibrationData.Calibration0ValueForCurrent1_AddresInEEprom,0,1000);
	CalibrationData.CalibrationValueForCurrent = flash_read(CalibrationData.CalibrationValueForCurrent_AddresInEEprom,10,100000);
	CalibrationData.CalibrationValueForCurrent1 = flash_read(CalibrationData.CalibrationValueForCurrent1_AddresInEEprom,10,100000);
	CalibrationData.CalibrationValueForVoltage = flash_read(CalibrationData.CalibrationValueForVoltage_AddresInEEprom,10,100000);
	CalibrationData.CalibrationValueForVoltage1 = flash_read(CalibrationData.CalibrationValueForVoltage1_AddresInEEprom,10,100000);
	CalibrationData.CalibrationValueForVoltage2 = flash_read(CalibrationData.CalibrationValueForVoltage2_AddresInEEprom,10,100000);
	CalibrationData.ResistanceComp_Ishunt_Wires = flash_read(CalibrationData.ResistanceComp_Ishunt_Wires_AddresInEEprom,70,200);
	CalibrationData.ResistanceComp_MOSFET = flash_read(CalibrationData.ResistanceComp_MOSFET_AddresInEEprom,10,200);
	Print_to_USART1("CalibrationReadFromFlash ");
}
uint8_t CalibrationReadFromFlashForVerify(void)
{
	CalibrationDataForVerify.CRC_data = flash_read(CalibrationDataForVerify.CRC_AddresInEEprom,0,256);
	CalibrationDataForVerify.Calibration0ValueForCurrent = flash_read(CalibrationDataForVerify.Calibration0ValueForCurrent_AddresInEEprom,0,1000);
	CalibrationDataForVerify.Calibration0ValueForCurrent1 = flash_read(CalibrationDataForVerify.Calibration0ValueForCurrent1_AddresInEEprom,0,1000);
	CalibrationDataForVerify.CalibrationValueForCurrent = flash_read(CalibrationDataForVerify.CalibrationValueForCurrent_AddresInEEprom,10,100000);
	CalibrationDataForVerify.CalibrationValueForCurrent1 = flash_read(CalibrationDataForVerify.CalibrationValueForCurrent1_AddresInEEprom,10,100000);
	CalibrationDataForVerify.CalibrationValueForVoltage = flash_read(CalibrationDataForVerify.CalibrationValueForVoltage_AddresInEEprom,10,100000);
	CalibrationDataForVerify.CalibrationValueForVoltage1 = flash_read(CalibrationDataForVerify.CalibrationValueForVoltage1_AddresInEEprom,10,100000);
	CalibrationDataForVerify.CalibrationValueForVoltage2 = flash_read(CalibrationDataForVerify.CalibrationValueForVoltage2_AddresInEEprom,10,100000);
	CalibrationDataForVerify.ResistanceComp_Ishunt_Wires = flash_read(CalibrationDataForVerify.ResistanceComp_Ishunt_Wires_AddresInEEprom,70,200);
	CalibrationDataForVerify.ResistanceComp_MOSFET = flash_read(CalibrationDataForVerify.ResistanceComp_MOSFET_AddresInEEprom,10,200);
	Print_to_USART1("CalibrationReadFromFlashForVerify ");
}
//==============
uint8_t SettingsWriteToFlash(void)
{
	flash_erase_page( SettingsData.CRC_AddresInEEprom);
	flash_write( SettingsData.CRC_AddresInEEprom, SettingsData.CRC_data);
	flash_write( SettingsData.Option1_AddresInEEprom, SettingsData.Option1);
	flash_write( SettingsData.ChargeTime_AddresInEEprom, SettingsData.ChargeTime);
	flash_write( SettingsData.LowVoltage_AddresInEEprom, SettingsData.LowVoltage);
	flash_write( SettingsData.MaxVoltage_AddresInEEprom, SettingsData.MaxVoltage);
	flash_write( SettingsData.Swing_Chrg_time_AddresInEEprom, SettingsData.Swing_Chrg_time);
	flash_write( SettingsData.Swing_DChrg_time_AddresInEEprom, SettingsData.Swing_DChrg_time);
	flash_write( SettingsData.ChargeAdapt_AddresInEEprom, SettingsData.ChargeAdapt);
	Print_to_USART1("SettingsWriteToFlash ");
}
uint8_t SettingsFactoryWriteToFlash(void)
{
	flash_erase_page( SettingsDataFactory.CRC_AddresInEEprom);
	flash_write( SettingsDataFactory.CRC_AddresInEEprom, SettingsDataFactory.CRC_data);
	flash_write( SettingsDataFactory.Option1_AddresInEEprom, SettingsDataFactory.Option1);
	flash_write( SettingsDataFactory.ChargeTime_AddresInEEprom, SettingsDataFactory.ChargeTime);
	flash_write( SettingsDataFactory.LowVoltage_AddresInEEprom, SettingsDataFactory.LowVoltage);
	flash_write( SettingsDataFactory.MaxVoltage_AddresInEEprom, SettingsDataFactory.MaxVoltage);
	flash_write( SettingsDataFactory.Swing_Chrg_time_AddresInEEprom, SettingsDataFactory.Swing_Chrg_time);
	flash_write( SettingsDataFactory.Swing_DChrg_time_AddresInEEprom, SettingsDataFactory.Swing_DChrg_time);
	flash_write( SettingsDataFactory.ChargeAdapt_AddresInEEprom, SettingsDataFactory.ChargeAdapt);
	Print_to_USART1("SettingsFactoryWriteToFlash ");
}
uint8_t SettingsReadFromFlash(void)
{
	SettingsData.CRC_data = flash_read(SettingsData.CRC_AddresInEEprom,0,256);
	SettingsData.Option1 =  flash_read(SettingsData.Option1_AddresInEEprom,1,10);
	SettingsData.ChargeTime =  flash_read(SettingsData.ChargeTime_AddresInEEprom,1,1000);
	SettingsData.LowVoltage =  flash_read(SettingsData.LowVoltage_AddresInEEprom,1,4000);
	SettingsData.MaxVoltage =  flash_read(SettingsData.MaxVoltage_AddresInEEprom,1,4000);
	SettingsData.Swing_Chrg_time =  flash_read(SettingsData.Swing_Chrg_time_AddresInEEprom,0,10000);
	SettingsData.Swing_DChrg_time =  flash_read(SettingsData.Swing_DChrg_time_AddresInEEprom,0,10000);
	SettingsData.ChargeAdapt = flash_read(SettingsData.ChargeAdapt_AddresInEEprom,0,100);
	Print_to_USART1("SettingsReadFromFlash ");
}
uint8_t SettingsReadFromFlashForVerify(void)
{
	SettingsDataForVerify.CRC_data = flash_read(SettingsDataForVerify.CRC_AddresInEEprom,0,256);
	SettingsDataForVerify.Option1 =  flash_read(SettingsDataForVerify.Option1_AddresInEEprom,1,10);
	SettingsDataForVerify.ChargeTime =  flash_read(SettingsDataForVerify.ChargeTime_AddresInEEprom,1,1000);
	SettingsDataForVerify.LowVoltage =  flash_read(SettingsDataForVerify.LowVoltage_AddresInEEprom,1,4000);
	SettingsDataForVerify.MaxVoltage =  flash_read(SettingsDataForVerify.MaxVoltage_AddresInEEprom,1,4000);
	SettingsDataForVerify.Swing_Chrg_time =  flash_read(SettingsDataForVerify.Swing_Chrg_time_AddresInEEprom,0,10000);
	SettingsDataForVerify.Swing_DChrg_time =  flash_read(SettingsDataForVerify.Swing_DChrg_time_AddresInEEprom,0,10000);
	SettingsDataForVerify.ChargeAdapt = flash_read(SettingsDataForVerify.ChargeAdapt_AddresInEEprom,0,100);
	Print_to_USART1("SettingsReadFromFlashForVerify ");
}
//===============
uint8_t DataWhenPowerOffWriteToFlash(void)
{
	flash_erase_page( SaveDataWhenPowerOff.CRC_AddresInEEprom);
	flash_write( SaveDataWhenPowerOff.CRC_AddresInEEprom, SaveDataWhenPowerOff.CRC_data);
	flash_write( SaveDataWhenPowerOff.BatteryCapacityDischargePrevious_AddresInEEprom, SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue);
	flash_write( SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent_AddresInEEprom, SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent);
	Print_to_USART1("DataWhenPowerOffWriteToFlash ");
}
uint8_t DataWhenPowerOff_Factory_WriteToFlash(void)
{
	flash_erase_page( SaveDataWhenPowerOffFactory.CRC_AddresInEEprom);
	flash_write( SaveDataWhenPowerOffFactory.CRC_AddresInEEprom, SaveDataWhenPowerOffFactory.CRC_data);
	flash_write( SaveDataWhenPowerOffFactory.BatteryCapacityDischargePrevious_AddresInEEprom, SaveDataWhenPowerOffFactory.BatteryCapacityDischargePreviousValue);
	flash_write( SaveDataWhenPowerOffFactory.BatteryCapacityDischargeCurrent_AddresInEEprom, SaveDataWhenPowerOffFactory.BatteryCapacityDischargeCurrent);
	Print_to_USART1("DataWhenPowerOff_Factory_WriteToFlash ");
}
uint8_t DataWhenPowerOffReadFromFlash(void)
{
	SaveDataWhenPowerOff.CRC_data = flash_read(SaveDataWhenPowerOff.CRC_AddresInEEprom,0,256);
	SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue =  flash_read(SaveDataWhenPowerOff.BatteryCapacityDischargePrevious_AddresInEEprom,0,1000000);
	SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent = flash_read(SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent_AddresInEEprom,0,1000000);
	Print_to_USART1("CalibrationDataWhenPowerOffReadFromFlash ");
}
uint8_t DataWhenPowerOffReadFromFlashForVerify(void)
{
	SaveDataWhenPowerOffForVerify.CRC_data = flash_read(SaveDataWhenPowerOffForVerify.CRC_AddresInEEprom,0,256);
	SaveDataWhenPowerOffForVerify.BatteryCapacityDischargePreviousValue =  flash_read(SaveDataWhenPowerOffForVerify.BatteryCapacityDischargePrevious_AddresInEEprom,0,1000000);
	SaveDataWhenPowerOffForVerify.BatteryCapacityDischargeCurrent = flash_read(SaveDataWhenPowerOffForVerify.BatteryCapacityDischargeCurrent_AddresInEEprom,0,1000000);
	Print_to_USART1("DataWhenPowerOffReadFromFlashForVerify ");
}




uint8_t FactoryEEpromWrite(void)
{
	__disable_irq();
	char Data_crc=0;
	char *Data = (unsigned char *) &SaveData;
	Data = Data+4;
	Data_crc = CalcCRC8(Data,((unsigned char)sizeof(SaveData))-4,0);
	Print_to_USART1_d(Data_crc,"CRC is : ",0);
	Print_to_USART1_d(((unsigned char)sizeof(SaveData))-4,"Size is : ",0);
	SaveData.CRC_data = Data_crc;
	flash_unlock();
	flash_erase_page( FactorySaveData.CRC_AddresInEEprom);
	flash_write( FactorySaveData.CRC_AddresInEEprom, FactorySaveData.CRC_data);
	flash_write( FactorySettingsData.Option1_AddresInEEprom, FactorySettingsData.Option1);
	flash_write( FactorySettingsData.ChargeTime_AddresInEEprom, FactorySettingsData.ChargeTime);
	flash_write( FactorySaveData.BatteryCapacityDischargePrevious_AddresInEEprom, FactorySaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue);
	flash_write( FactorySettingsData.LowVoltage_AddresInEEprom, FactorySettingsData.LowVoltage);
	flash_write( FactorySettingsData.MaxVoltage_AddresInEEprom, FactorySettingsData.MaxVoltage);
	flash_write( FactorySettingsData.Swing_Chrg_time_AddresInEEprom, FactorySettingsData.Swing_Chrg_time);
	flash_write( FactorySettingsData.Swing_DChrg_time_AddresInEEprom, FactorySettingsData.Swing_DChrg_time);
	flash_write( FactorySaveDataWhenPowerOff.BatteryCapacityDischargeCurrent_AddresInEEprom, FactorySaveDataWhenPowerOff.BatteryCapacityDischargeCurrent);

	flash_write( FactoryCalibrationData.Calibration0ValueForCurrent_AddresInEEprom, FactoryCalibrationData.Calibration0ValueForCurrent);
	flash_write( FactoryCalibrationData.Calibration0ValueForCurrent1_AddresInEEprom, FactoryCalibrationData.Calibration0ValueForCurrent1);
	flash_write( FactoryCalibrationData.CalibrationValueForCurrent_AddresInEEprom, FactoryCalibrationData.CalibrationValueForCurrent);
	flash_write( FactoryCalibrationData.CalibrationValueForCurrent1_AddresInEEprom, FactoryCalibrationData.CalibrationValueForCurrent1);
	flash_write( FactoryCalibrationData.CalibrationValueForVoltage_AddresInEEprom, FactoryCalibrationData.CalibrationValueForVoltage);
	flash_write( FactoryCalibrationData.CalibrationValueForVoltage1_AddresInEEprom, FactoryCalibrationData.CalibrationValueForVoltage1);
	flash_write( FactoryCalibrationData.CalibrationValueForVoltage2_AddresInEEprom, FactoryCalibrationData.CalibrationValueForVoltage2);
	flash_write( FactorySettingsData.ChargeAdapt_AddresInEEprom, FactorySettingsData.ChargeAdapt);
	flash_write( FactoryCalibrationData.ResistanceComp_Ishunt_Wires_AddresInEEprom, FactoryCalibrationData.ResistanceComp_Ishunt_Wires);
	flash_write( FactoryCalibrationData.ResistanceComp_MOSFET_AddresInEEprom, FactoryCalibrationData.ResistanceComp_MOSFET);

	flash_lock();
	__enable_irq();

	Print_to_USART1("FactoryEEpromWrite write ");
	return 1;
}


uint8_t ReadFromEEprom(void)
{
	    __disable_irq();
	    SaveData.CRC_data = flash_read(SaveData.CRC_AddresInEEprom,0,256);
		SettingsData.Option1 =  flash_read(SettingsData.Option1_AddresInEEprom,1,10);
		SettingsData.ChargeTime =  flash_read(SettingsData.ChargeTime_AddresInEEprom,1,1000);
		SaveDataWhenPowerOff.BatteryCapacityDischargePreviousValue =  flash_read(SaveData.BatteryCapacityDischargePrevious_AddresInEEprom,0,1000000);
		SettingsData.LowVoltage =  flash_read(SettingsData.LowVoltage_AddresInEEprom,1,4000);
		SettingsData.MaxVoltage =  flash_read(SettingsData.MaxVoltage_AddresInEEprom,1,4000);
		SettingsData.Swing_Chrg_time =  flash_read(SettingsData.Swing_Chrg_time_AddresInEEprom,0,10000);
		SettingsData.Swing_DChrg_time =  flash_read(SettingsData.Swing_DChrg_time_AddresInEEprom,0,10000);
		SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent = flash_read(SaveDataWhenPowerOff.BatteryCapacityDischargeCurrent_AddresInEEprom,0,1000000);
		CalibrationData.Calibration0ValueForCurrent = flash_read(CalibrationData.Calibration0ValueForCurrent_AddresInEEprom,0,1000);
		CalibrationData.Calibration0ValueForCurrent1 = flash_read(CalibrationData.Calibration0ValueForCurrent1_AddresInEEprom,0,1000);
		CalibrationData.CalibrationValueForCurrent = flash_read(CalibrationData.CalibrationValueForCurrent_AddresInEEprom,10,100000);
		CalibrationData.CalibrationValueForCurrent1 = flash_read(CalibrationData.CalibrationValueForCurrent1_AddresInEEprom,10,100000);
		CalibrationData.CalibrationValueForVoltage = flash_read(CalibrationData.CalibrationValueForVoltage_AddresInEEprom,10,100000);
		CalibrationData.CalibrationValueForVoltage1 = flash_read(CalibrationData.CalibrationValueForVoltage1_AddresInEEprom,10,100000);
		CalibrationData.CalibrationValueForVoltage2 = flash_read(CalibrationData.CalibrationValueForVoltage2_AddresInEEprom,10,100000);
		SettingsData.ChargeAdapt = flash_read(SettingsData.ChargeAdapt_AddresInEEprom,0,100);
		CalibrationData.ResistanceComp_Ishunt_Wires = flash_read(CalibrationData.ResistanceComp_Ishunt_Wires_AddresInEEprom,70,200);
		CalibrationData.ResistanceComp_MOSFET = flash_read(CalibrationData.ResistanceComp_MOSFET_AddresInEEprom,10,200);
		char Data_crc=0;
		char *Data = (unsigned char *) &SaveData;
		Data = Data+4;
		Data_crc = CalcCRC8(Data,((unsigned char)sizeof(SaveData))-4,0);
		Print_to_USART1_d(Data_crc,"CRC is : ",0);
		Print_to_USART1_d(((unsigned char)sizeof(SaveData))-4,"Size is : ",0);


		Print_to_USART1_d(Data_crc,"CRC is : ",0);


		__enable_irq();
		if (EEpromReadStatus == 0)
		{
			Print_to_USART1("Read from EEprom - FAIL ");
			EEpromReadStatus = 1;
			return 0;
		}
		else
		{
			Print_to_USART1("Read from EEprom - SUCCESS");
			EEpromReadStatus = 1;
			return 1;
		}
}

uint8_t ReadFromEEpromVerify(void)
{

	    SaveDataForVerify.CRC_data = flash_read(SaveDataForVerify.CRC_AddresInEEprom,0,256);
		SaveDataForVerify.Option1 =  flash_read(SaveDataForVerify.Option1_AddresInEEprom,1,10);
		SaveDataForVerify.Value =  flash_read(SaveDataForVerify.Value_AddresInEEprom,1,1000);
		SaveDataForVerify.BatteryCapacityDischargePreviousValue =  flash_read(SaveDataForVerify.BatteryCapacityDischargePrevious_AddresInEEprom,0,1000000);
		SaveDataForVerify.LowVoltage =  flash_read(SaveDataForVerify.LowVoltage_AddresInEEprom,1,4000);
		SaveDataForVerify.MaxVoltage =  flash_read(SaveDataForVerify.MaxVoltage_AddresInEEprom,1,4000);
		SaveDataForVerify.Swing_Chrg_time =  flash_read(SaveDataForVerify.Swing_Chrg_time_AddresInEEprom,0,10000);
		SaveDataForVerify.Swing_DChrg_time =  flash_read(SaveDataForVerify.Swing_DChrg_time_AddresInEEprom,0,10000);
		SaveDataForVerify.BatteryCapacityDischargeCurrent = flash_read(SaveDataForVerify.BatteryCapacityDischargeCurrent_AddresInEEprom,0,1000000);
		SaveDataForVerify.Calibration0ValueForCurrent = flash_read(SaveDataForVerify.Calibration0ValueForCurrent_AddresInEEprom,0,1000);
		SaveDataForVerify.Calibration0ValueForCurrent1 = flash_read(SaveDataForVerify.Calibration0ValueForCurrent1_AddresInEEprom,0,1000);
		SaveDataForVerify.CalibrationValueForCurrent = flash_read(SaveDataForVerify.CalibrationValueForCurrent_AddresInEEprom,10,100000);
		SaveDataForVerify.CalibrationValueForCurrent1 = flash_read(SaveDataForVerify.CalibrationValueForCurrent1_AddresInEEprom,10,100000);
		SaveDataForVerify.CalibrationValueForVoltage = flash_read(SaveDataForVerify.CalibrationValueForVoltage_AddresInEEprom,10,100000);
		SaveDataForVerify.CalibrationValueForVoltage1 = flash_read(SaveDataForVerify.CalibrationValueForVoltage1_AddresInEEprom,10,100000);
		SaveDataForVerify.CalibrationValueForVoltage2 = flash_read(SaveDataForVerify.CalibrationValueForVoltage2_AddresInEEprom,10,100000);
		SaveDataForVerify.ChargeAdapt = flash_read(SaveDataForVerify.ChargeAdapt_AddresInEEprom,0,100);
		SaveDataForVerify.ResistanceComp_Ishunt_Wires = flash_read(SaveDataForVerify.ResistanceComp_Ishunt_Wires_AddresInEEprom,70,200);
		SaveDataForVerify.ResistanceComp_MOSFET = flash_read(SaveDataForVerify.ResistanceComp_MOSFET_AddresInEEprom,10,200);
}

char CalcCRC8(char *Ptr, char Num, char CRC1)
{
   const char CrcTable[256] = {
    0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, // 00
    0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d, // 08
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, // 10
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D, // 18
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, // 20
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD, // 28
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,  // 30
    0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,  // 38
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,  // 40
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,  // 48
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,  // 50
    0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,  // 58
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,  // 60
    0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,  // 68
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,  // 70
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,  // 78
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,  // 80
    0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,  // 88
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,  // 90
    0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,  // 98
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,  // a0
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,  // a8
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,  // b0
    0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,  // b8
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,  // c0
    0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,  // c8
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,  // d0
    0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13, // d8
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, // e0
    0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83, // e8
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, // f0
    0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3 // f8
  };
  do  CRC1 = CrcTable[CRC1 ^ *Ptr++]; while (--Num);
  return CRC1;
}
