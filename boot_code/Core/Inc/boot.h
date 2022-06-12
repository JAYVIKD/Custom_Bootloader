#ifndef __BOOT_H_
#define __BOOT_H_

// Version Info

#define Version_BLC  "Jayvik Desai's Bootloader V1.0"


//Command Defination

#define CMD_VER_USE											"%s: Command to read the Version info \r\n", CMD_VER
#define CMD_VER 												0x51
#define CMD_GET_HELP_USE								"%s: Command to print the help menu\r\n", CMD_GET_HELP
#define CMD_GET_HELP 										0x52
#define CMD_GET_CID_USE 								"%s: Command to print the MCU chipID\r\n", CMD_GET_CID
#define CMD_GET_CID 										0x53
#define CMD_GET_RDP_STATUS_USE					"%s: Command to read the FLASH read write protection level\r\n", CMD_GET_RDP_STATUS
#define CMD_GET_RDP_STATUS 							0x54
#define CMD_GO_TO_ADDR_USE							"%s: Command to jump bootloader to a particular address\r\n", CMD_GO_TO_ADDR
#define CMD_GO_TO_ADDR 									0x55
#define CMD_FLASH_ERASE_USE							"%s: Command to mass erase a sector of the user flash\r\n", CMD_FLASH_ERASE
#define CMD_FLASH_ERASE 								0x56
#define CMD_MEM_WRITE_USE								"%s: Command to write Data in to different memories of the MCU\r\n", CMD_MEM_WRITE
#define CMD_MEM_WRITE 									0x57
#define CMD_ENDIS_RW_PROTECT_USE				"%s: Command to enable or disable read write protection\r\n", CMD_ENDIS_RW_PROTECT
#define CMD_ENDIS_RW_PROTECT 						0x58
#define CMD_MEM_READ_USE								"%s: Command to read data from different memories of MCU\r\n", CMD_MEM_READ
#define CMD_MEM_READ 										0x59
#define CMD_READ_SECTOR_STATUS_USE			"%s: Command to read all the flash sector protection status\r\n", CMD_READ_SECTOR_STATUS
#define CMD_READ_SECTOR_STATUS 					0x5a
#define CMD_OTP_READ_USE								"%s: Command to read all the OTP contents\r\n", CMD_OTP_READ
#define CMD_OTP_READ 										0x5b



#include "stm32f7xx_hal.h"
#include "main.h"
#define DEBUG_EN 
#define FLASH_SECTOR2_BASE_ADDRESS 0x08010000u
#define VEC_OFFSET 0x10000

#define REC_Port &huart3
#define versio

void bootloader_jump_user_app(void);
void bootloader_boot_code(void);
void bootloader_init(void);
void bootloader_Get_Help(void);
void bootloader_Get_Cid(uint8_t* buffer);
void bootloader_Get_RDP_Status(uint8_t* buffer);
void bootloader_Go_To_ADDR(uint8_t* buffer);
void bootloader_Flash_Erase(uint8_t* buffer);
void bootloader_Mem_Write(uint8_t* buffer);
void bootloader_Endis_RW_Protect(uint8_t* buffer);
void bootloader_Mem_Read(uint8_t* buffer);
void bootloader_Read_Sector_Status(uint8_t* buffer);
void bootloader_OTP_Read(uint8_t* buffer);
#endif 