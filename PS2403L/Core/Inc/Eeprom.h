#ifndef _EE_
#define _EE_
#include <stm32f1xx.h>
#include "General.h"


uint8_t flash_ready(void);
void flash_erase_page(uint32_t address);
void flash_unlock(void);
void flash_lock();
void flash_write(uint32_t address,uint32_t data);
uint32_t flash_read(uint32_t address, uint32_t MinValue, uint32_t MaxValue);

uint8_t ReadFromFlash(void);
uint8_t DataWhenPowerOffWriteToFlash_CRC(void);
uint8_t SettingsWriteToFlash_CRC(void);
uint8_t CalibrationWriteToFlash_CRC(void);
uint8_t FactoryWriteToFlash_CRC(void);
char CalcCRC8(char *Ptr, char Num, char CRC1);




uint8_t CalibrationWriteToFlash(void);
uint8_t CalibrationFactoryWriteToFlash(void);
uint8_t CalibrationReadFromFlash(void);
uint8_t CalibrationReadFromFlashForVerify(void);
uint8_t SettingsWriteToFlash(void);
uint8_t SettingsFactoryWriteToFlash(void);
uint8_t SettingsReadFromFlash(void);
uint8_t SettingsReadFromFlashForVerify(void);
uint8_t DataWhenPowerOffWriteToFlash(void);
uint8_t DataWhenPowerOff_Factory_WriteToFlash(void);
uint8_t DataWhenPowerOffReadFromFlash(void);
uint8_t DataWhenPowerOffReadFromFlashForVerify(void);

void flash_write_block();
uint8_t flash_read_block();









#endif
