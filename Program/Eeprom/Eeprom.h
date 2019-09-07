#include <stm32f10x.h>
#include "General/General.h"
#ifndef _EE_
#define _EE_

uint8_t flash_ready(void);
void flash_erase_page(uint32_t address);
void flash_unlock(void);
void flash_lock();
void flash_write(uint32_t address,uint32_t data);
uint32_t flash_read(uint32_t address, uint32_t MinValue, uint32_t MaxValue);

uint8_t ReadFromEEprom(void);
uint8_t EEpromWrite(void);
uint8_t FactoryEEpromWrite(void);
char CalcCRC8(char *Ptr, char Num, char CRC1);
uint8_t ReadFromEEpromVerify(void);

#endif
