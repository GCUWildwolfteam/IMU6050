/*
 * C8T6的Flash为64k，中容量产品每一页1K(1024)=8196位
 * 即有0-63页
 * 写入步骤：解锁，页擦除，写入，锁定
 * 读出步骤，指针读取
*/

#include "Flash.h"

void FLASH_Erase(uint16_t page){
	//解锁Flash
	FLASH_Unlock();
	//页擦除
	FLASH_ErasePage(FLASH_BASEADDR+1024*page);
	//锁Flash
	FLASH_Lock();
}

	
void Flash_Write16(uint16_t page, uint16_t Address_offset, uint16_t data){
	//解锁Flash
	FLASH_Unlock(); 
	//写入数据
	FLASH_ProgramHalfWord( (FLASH_BASEADDR+1024*page)+Address_offset, data);
	//锁Flash
	FLASH_Lock();
}

void Flash_Write32(uint16_t page, uint16_t Address_offset, uint32_t data){
	//解锁Flash
	FLASH_Unlock(); 
	//写入数据
	FLASH_ProgramWord((FLASH_BASEADDR+1024*page)+Address_offset, data);
	//锁Flash
	FLASH_Lock();
}

void Flash_Read16(uint16_t page, uint16_t Address_offset, uint16_t* data){
	uint16_t* p = (uint16_t*)( (FLASH_BASEADDR+1024*page)+Address_offset);
	*data = *p;
}

void Flash_Read32(uint16_t page, uint16_t Address_offset, uint32_t* data){
	uint32_t* p = (uint32_t*)( (FLASH_BASEADDR+1024*page)+Address_offset);
	*data = *p;
}
