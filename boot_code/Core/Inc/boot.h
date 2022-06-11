#ifndef __BOOT_H_
#define __BOOT_H_

 
#include "stm32f7xx_hal.h"
#define DEBUG_EN 
#define FLASH_SECTOR2_BASE_ADDRESS 0x08010000u
#define VEC_OFFSET 0x10000

void bootloader_jump_user_app(void);
void bootloader_boot_code(void);
void bootloader_init(void);
#endif 