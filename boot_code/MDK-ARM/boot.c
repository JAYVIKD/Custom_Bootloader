#include"boot.h"

//global 
uint8_t buffer[200];
uint8_t Version_BLC  = 0x1;
volatile MetaData MD;
//check for GPIO pin status 
uint8_t supported_command[] = {
														CMD_VER,
														CMD_GET_HELP,
														CMD_GET_CID,
														CMD_GET_RDP_STATUS,
														CMD_GO_TO_ADDR,
														CMD_FLASH_ERASE,
														CMD_MEM_WRITE,
														CMD_ENDIS_RW_PROTECT,
														CMD_MEM_READ,
														CMD_READ_SECTOR_STATUS,
														CMD_OTP_READ
};





void bl_printf(char *format,...){
	#ifdef DEBUG_EN 
		char str[80];
		memset(str , 0 , 80); 
		va_list args;
		va_start(args, format);
		//strcat(format, "\r\n");
		sprintf(str, format ,args);
		HAL_UART_Transmit(REC_Port , (uint8_t*)str , strlen(str), HAL_MAX_DELAY);
		va_end(args);
	#endif
}

void bootloader_init(){
	
	
	
	if(HAL_GPIO_ReadPin(but_GPIO_Port, but_Pin) == GPIO_PIN_SET){
				// Bootloader code if button is pressed
				
				bl_printf("BOOTLOADER ACTIVE\r\n");
				bl_printf("Press '0R' for command menu\r\n:");
				bootloader_boot_code();
		}
	else{
				// jump to app code if the button is not pressed
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
				bl_printf("JUMPING TO APP CODE%d\r\n");
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
	char str[80];
	MD.data1= *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS+ 0x1d0) ;
	MD.data2= *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS+ 0x1d4) ;
	MD.data3= *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS+ 0x1d8) ;
	MD.data4= *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS+ 0x1dC) ;
	sprintf(str, "META:%x,%x,%x,%x",MD.data1,MD.data2,MD.data3,MD.data4);
	bl_printf(str);
	void (*app_reset_handler)(void);
	bl_printf("Jumping to user app\r\n" , FLASH_SECTOR_2);
	// set the MSP value
	uint32_t MSP_val= *(volatile uint32_t *) FLASH_SECTOR2_BASE_ADDRESS;
	__set_MSP(MSP_val);
	SCB->VTOR = FLASH_SECTOR2_BASE_ADDRESS+16;
	// Jump to the reset handler
	uint32_t app_reset_handler_address= *(volatile uint32_t *) (FLASH_SECTOR2_BASE_ADDRESS+4);
	app_reset_handler =  (void*)app_reset_handler_address; 
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
		HAL_UART_Receive(REC_Port, &buffer[1] , rec_len, HAL_MAX_DELAY);
		
		switch(buffer[1]){
		
			case CMD_VER: 
										bootloader_Get_Version();
										
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
										bl_printf("\r\nInvalid Command\r\n:");
										break;
		}
	
	}
}

void bootloader_Get_Version(void){
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
		//bootloader_ACK(buffer[0]);
		//HAL_UART_Transmit(REC_Port, &Version_BLC, 1, HAL_MAX_DELAY);

		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				bootloader_ACK(1);
				HAL_UART_Transmit(REC_Port, &Version_BLC, 1, HAL_MAX_DELAY);

		}
		
		else {
			bootloader_NACK();
		}
		
	
		
		
}
//Function to print the Supported Bootloader Command
void bootloader_Get_Help(){
		
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
		//bootloader_ACK(buffer[0]);
		//HAL_UART_Transmit(REC_Port, &Version_BLC, 1, HAL_MAX_DELAY);

		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				bootloader_ACK(sizeof(supported_command));
				HAL_UART_Transmit(REC_Port, supported_command, sizeof(supported_command), HAL_MAX_DELAY);

		}
		
		else {
			bootloader_NACK();
		}
		bl_printf("\r\nList of Commands:\r\n");
		bl_printf(CMD_VER_USE);
		bl_printf(CMD_GET_HELP_USE);
		bl_printf(CMD_GET_CID_USE);
		bl_printf(CMD_GET_RDP_STATUS_USE);
		bl_printf(CMD_GO_TO_ADDR_USE);
		bl_printf(CMD_FLASH_ERASE_USE);
		bl_printf(CMD_MEM_WRITE_USE);
		bl_printf(CMD_ENDIS_RW_PROTECT_USE);
		bl_printf(CMD_MEM_READ_USE);
		bl_printf(CMD_READ_SECTOR_STATUS_USE);
		bl_printf(CMD_OTP_READ_USE);
		bl_printf("\r\n:");
			
}

void bootloader_Get_Cid(uint8_t* buffer){
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
		uint16_t cid = get_MCU_chip_ID();
	

		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				
				bootloader_ACK(sizeof(cid));
				HAL_UART_Transmit(REC_Port, (uint8_t *)&cid, sizeof(cid), HAL_MAX_DELAY);

		}
		
		else {
			bootloader_NACK();
		}
}

void bootloader_Get_RDP_Status(uint8_t* buffer){
		uint8_t RDP_stat = get_RDP_status();
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
	
		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				
				bootloader_ACK(sizeof(RDP_stat));
				HAL_UART_Transmit(REC_Port, (uint8_t *)&RDP_stat, sizeof(RDP_stat), HAL_MAX_DELAY);

		}
		
		else {
			bootloader_NACK();
		}
}

void bootloader_Go_To_ADDR(uint8_t* buffer){
		
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
		uint8_t addr_val = ADDR_VALID;
		uint8_t addr_inval = ADDR_INVALID;
		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				uint32_t jump_address = *((volatile uint32_t *) &buffer[2]);
				jump_address+=1;
					bootloader_ACK(1);
				if( check_valid_address(jump_address)){
						HAL_UART_Transmit(REC_Port, &addr_val, 1, HAL_MAX_DELAY);
						void (*jump_to_address) (void) = (void *)jump_address;
						jump_to_address();
						
				}
				else{
						
						HAL_UART_Transmit(REC_Port, &addr_inval, 1, HAL_MAX_DELAY);
				}

		}
		
		else {
			bootloader_NACK();
		}
}

void bootloader_Flash_Erase(uint8_t* buffer){
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
		uint8_t success = SUCCESS;
		uint8_t fail = ERROR;
		uint8_t sector_no = buffer[2];
		uint8_t no_of_sector = buffer[3];
		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				bootloader_ACK(1);
				uint8_t ret = erase_flash_sector( buffer[2] , buffer[3]);
				
				if( ret == SUCCESS) {
						HAL_UART_Transmit(REC_Port, &success , 1, HAL_MAX_DELAY);
						
				}
				else {
						HAL_UART_Transmit(REC_Port, &fail , 1, HAL_MAX_DELAY);
				}
						
		}
		
		else {
			bootloader_NACK();
		}
}

void bootloader_Mem_Write(uint8_t* buffer){
		uint32_t command_packet_length = buffer[0]+1;
		uint32_t host_crc = *((uint32_t * ) (buffer+command_packet_length - 4) ) ;
		uint32_t base_add = *((uint32_t * ) (&buffer[2])) ; 
		uint32_t payload_len = buffer[6];
		uint8_t success = SUCCESS;
		uint8_t fail = ERROR;
		HAL_StatusTypeDef ret; 
		if( ! bootloader_verify_CRC( &buffer[0] , command_packet_length - 4 , host_crc)){
				bootloader_ACK(1);
				if( check_valid_address(base_add)){
								HAL_FLASH_Unlock();
								for( uint32_t i = 0 ; i < payload_len ; i++){
									ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,base_add+i,buffer[i+7]);
								}
								HAL_FLASH_Lock();
								
										if(ret == SUCCESS){
												HAL_UART_Transmit(REC_Port, &success , 1, HAL_MAX_DELAY);
										}
										else {
											HAL_UART_Transmit(REC_Port, &fail , 1, HAL_MAX_DELAY);
										}
								
				}
				else{
							HAL_UART_Transmit(REC_Port, &fail , 1, HAL_MAX_DELAY);
				}
					
		}
		
		else {
			bootloader_NACK();
		}
}

void bootloader_Endis_RW_Protect(uint8_t* buffer){

}

void bootloader_Mem_Read(uint8_t* buffer){

}

void bootloader_Read_Sector_Status(uint8_t* buffer){

}

void bootloader_OTP_Read(uint8_t* buffer){

}

void bootloader_ACK(uint8_t follow_len){
			uint8_t ack_buf[2];
			ack_buf[0] = BL_ACK;
			ack_buf[1] = follow_len;
			HAL_UART_Transmit(REC_Port, ack_buf, 2, HAL_MAX_DELAY);
}

void bootloader_NACK(void){

		HAL_UART_Transmit(REC_Port, (uint8_t *)BL_NACK , 1, HAL_MAX_DELAY);
}

uint8_t bootloader_verify_CRC(uint8_t *pdata , uint32_t len , uint32_t CRC_host){
		uint32_t CRC_val = 0xff;
	
		for (uint32_t i=0 ; i<len ; i++){
			uint32_t idata = pdata[i];
			CRC_val = HAL_CRC_Accumulate(&hcrc, &idata , 1);
		}
		 __HAL_CRC_DR_RESET(&hcrc);
		if(CRC_val == CRC_host){
				return CRC_Success;
		}
		
		return CRC_Fail;
}

uint16_t get_MCU_chip_ID(){
			uint16_t cid ;
			cid = ((uint16_t) DBGMCU->IDCODE) & 0x0fff ;
			
		return cid;
}

uint8_t get_RDP_status(){
			uint8_t RDP = ((FLASH->OPTCR >> 8)  & 0xff);
			
			return RDP;
}
// add to check the validity of the address not added anything yet so all addresses are valid address 
uint8_t check_valid_address(uint32_t address){
			return 1;

}

uint8_t erase_flash_sector( uint8_t sector , uint8_t no_of_sector){
		HAL_FLASH_Unlock();
		while(FLASH->SR & (1<<16));
		FLASH->CR |= 0x2;
		FLASH->CR |= ( sector << 3);
		FLASH->CR |= (1 << 16);
		while(FLASH->SR & (1<<16));
		HAL_FLASH_Lock();
		
		return SUCCESS; 
}
