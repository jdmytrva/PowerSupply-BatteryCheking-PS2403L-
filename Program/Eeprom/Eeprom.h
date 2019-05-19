#include <stm32f10x.h>
#ifndef _EE_
#define _EE_

uint8_t flash_ready(void);
void flash_erase_page(uint32_t address);
void flash_unlock(void);
void flash_lock();
void flash_write(uint32_t address,uint32_t data);
uint32_t flash_read(uint32_t address);



#endif
