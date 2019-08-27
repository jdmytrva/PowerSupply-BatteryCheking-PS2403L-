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
	flash_write( SaveData.ResistanceComp_Ishunt_Wires_AddresInEEprom, SaveData.ResistanceComp_Ishunt_Wires);
	flash_write( SaveData.ResistanceComp_MOSFET_AddresInEEprom, SaveData.ResistanceComp_MOSFET);

	flash_lock();
	__enable_irq();
	Print_to_USART1("EEprom write ");
	return 1;
}
uint8_t FactoryEEpromWrite(void)
{
	__disable_irq();
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
	flash_write( FactorySaveData.ResistanceComp_Ishunt_Wires_AddresInEEprom, FactorySaveData.ResistanceComp_Ishunt_Wires);
	flash_write( FactorySaveData.ResistanceComp_MOSFET_AddresInEEprom, FactorySaveData.ResistanceComp_MOSFET);

	flash_lock();
	__enable_irq();
	Print_to_USART1("FactoryEEpromWrite write ");
	return 1;
}


uint8_t ReadFromEEprom(void)
{
	    __disable_irq();
		SaveData.Option1 =  flash_read(SaveData.Option1_AddresInEEprom,1,10);
		SaveData.Value =  flash_read(SaveData.Value_AddresInEEprom,1,1000);
		SaveData.BatteryCapacityDischargePreviousValue =  flash_read(SaveData.BatteryCapacityDischargePrevious_AddresInEEprom,0,1000000);
		SaveData.LowVoltage =  flash_read(SaveData.LowVoltage_AddresInEEprom,1,4000);
		SaveData.MaxVoltage =  flash_read(SaveData.MaxVoltage_AddresInEEprom,1,4000);
		SaveData.Swing_Chrg_time =  flash_read(SaveData.Swing_Chrg_time_AddresInEEprom,0,10000);
		SaveData.Swing_DChrg_time =  flash_read(SaveData.Swing_DChrg_time_AddresInEEprom,0,10000);
		SaveData.BatteryCapacityDischargeCurrent = flash_read(SaveData.BatteryCapacityDischargeCurrent_AddresInEEprom,0,1000000);
		SaveData.Calibration0ValueForCurrent = flash_read(SaveData.Calibration0ValueForCurrent_AddresInEEprom,0,1000);
		SaveData.Calibration0ValueForCurrent1 = flash_read(SaveData.Calibration0ValueForCurrent1_AddresInEEprom,0,1000);
		SaveData.CalibrationValueForCurrent = flash_read(SaveData.CalibrationValueForCurrent_AddresInEEprom,10,100000);
		SaveData.CalibrationValueForCurrent1 = flash_read(SaveData.CalibrationValueForCurrent1_AddresInEEprom,10,100000);
		SaveData.CalibrationValueForVoltage = flash_read(SaveData.CalibrationValueForVoltage_AddresInEEprom,10,100000);
		SaveData.CalibrationValueForVoltage1 = flash_read(SaveData.CalibrationValueForVoltage1_AddresInEEprom,10,100000);
		SaveData.CalibrationValueForVoltage2 = flash_read(SaveData.CalibrationValueForVoltage2_AddresInEEprom,10,100000);
		SaveData.ChargeAdapt = flash_read(SaveData.ChargeAdapt_AddresInEEprom,0,100);
		SaveData.ResistanceComp_Ishunt_Wires = flash_read(SaveData.ResistanceComp_Ishunt_Wires_AddresInEEprom,70,200);
		SaveData.ResistanceComp_MOSFET = flash_read(SaveData.ResistanceComp_MOSFET_AddresInEEprom,10,200);
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

