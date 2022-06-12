#include"boot.h"


uint8_t buffer[200];
// checks for 
void bootloader_init(){
	
	
	
	if(HAL_GPIO_ReadPin(but_GPIO_Port, but_Pin) == GPIO_PIN_SET){
				// Bootloader code if button is pressed
				
				st_printf("BOOTLOADER ACTIVE\r\n");
		st_printf("Press '0R' for command menu\r\n:");
				bootloader_boot_code();
		}
	else{
				// jump to app code if the button is not pressed
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
				st_printf("JUMPING TO APP CODE\r\n");
				//read Uart data
				bootloader_jump_user_app();
	}
}
/*
 Jump to the User Application.
** Change the Vector Table Offset register
** assuming flash sector 2 is used
** make sure the MSP is proper
** jump to app code from baseaddr
*/
void bootloader_jump_user_app(){
	//pointerr to the app reset handler
	void (*app_reset_handler)(void);
	st_printf("Jumping to user app\r\n" , FLASH_SECTOR_2);
	// set the MSP value
	uint32_t MSP_val= *(volatile uint32_t *) FLASH_SECTOR2_BASE_ADDRESS;
	__set_MSP(MSP_val);
	SCB->VTOR = FLASHAXI_BASE | VEC_OFFSET;
	// Jump to the reset handler
	uint32_t app_reset_handler_address= *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS+4);
	app_reset_handler = (void*) app_reset_handler_address; 
	app_reset_handler();
}

//BootLoader Main code 
//take commands from the UARTport 
void bootloader_boot_code(){
	uint32_t rec_len;
	while(1){
		memset(buffer,0,200);
		HAL_UART_Receive(REC_Port, buffer , 1, HAL_MAX_DELAY);
		rec_len = buffer[0];
		HAL_UART_Receive(REC_Port, &buffer[1] , 1, HAL_MAX_DELAY);
		
		switch(buffer[1]){
		
			case CMD_VER: 
										st_printf("\r\n");
										st_printf(Version_BLC);
										break;
			case CMD_GET_HELP: 
										bootloader_Get_Help();
										break;
			case CMD_GET_CID: 
										bootloader_Get_Cid(buffer);
										break;
			case CMD_GET_RDP_STATUS: 
										bootloader_Get_RDP_Status(buffer);
										break;
			case CMD_GO_TO_ADDR: 
										bootloader_Go_To_ADDR(buffer);
										break;
			case CMD_FLASH_ERASE: 
										bootloader_Flash_Erase(buffer);
										break;
			case CMD_MEM_WRITE: 
										bootloader_Mem_Write(buffer);
										break;
			case CMD_ENDIS_RW_PROTECT: 
										bootloader_Endis_RW_Protect(buffer);
										break;
			case CMD_MEM_READ: 
										bootloader_Mem_Read(buffer);
										break;
			case CMD_READ_SECTOR_STATUS: 
										bootloader_Read_Sector_Status(buffer);
										break;
			case CMD_OTP_READ: 
										bootloader_OTP_Read(buffer);
										break;
			
			
			default : 		
										st_printf("Invalid Command\r\n:");
										break;
		}
	
	}
}

//Function to print the Supported Bootloader Command
void bootloader_Get_Help(){
		
		
		st_printf("\r\nList of Commands:\r\n");
		st_printf(CMD_VER_USE);
		st_printf(CMD_GET_HELP_USE);
		st_printf(CMD_GET_CID_USE);
		st_printf(CMD_GET_RDP_STATUS_USE);
		st_printf(CMD_GO_TO_ADDR_USE);
		st_printf(CMD_FLASH_ERASE_USE);
		st_printf(CMD_MEM_WRITE_USE);
		st_printf(CMD_ENDIS_RW_PROTECT_USE);
		st_printf(CMD_MEM_READ_USE);
		st_printf(CMD_READ_SECTOR_STATUS_USE);
		st_printf(CMD_OTP_READ_USE);
		st_printf("\r\n:");
			
}

void bootloader_Get_Cid(uint8_t* buffer){

}

void bootloader_Get_RDP_Status(uint8_t* buffer){

}

void bootloader_Go_To_ADDR(uint8_t* buffer){

}

void bootloader_Flash_Erase(uint8_t* buffer){

}

void bootloader_Mem_Write(uint8_t* buffer){

}

void bootloader_Endis_RW_Protect(uint8_t* buffer){

}

void bootloader_Mem_Read(uint8_t* buffer){

}

void bootloader_Read_Sector_Status(uint8_t* buffer){

}

void bootloader_OTP_Read(uint8_t* buffer){

}
