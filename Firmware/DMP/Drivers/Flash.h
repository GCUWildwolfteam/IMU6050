#ifndef __FLASH_H
#define __FLASH_H

#include "user_common.h"

#define FLASH_BASEADDR 		0x08000000

void FLASH_Erase(uint16_t page);

void Flash_Write16(uint16_t page, uint16_t Address_offset, uint16_t data);
void Flash_Write32(uint16_t page, uint16_t Address_offset, uint32_t data);

void Flash_Read16(uint16_t page, uint16_t Address_offset, uint16_t* data);
void Flash_Read32(uint16_t page, uint16_t Address_offset, uint32_t* data);


#endif /* __FLASH_H */
