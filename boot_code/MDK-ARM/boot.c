#include"boot.h"
#include"main.h"
// checks for 
void bootloader_init(){
	
	
	
	if(HAL_GPIO_ReadPin(but_GPIO_Port, but_Pin) == GPIO_PIN_SET){
				// Bootloader code if button is pressed
				
				st_printf("BOOTLOADER ACTIVE\r\n");
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


void bootloader_boot_code(){
// boot code
	
}
