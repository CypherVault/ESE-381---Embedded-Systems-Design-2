/*
 * DS3231_block_write_read_test.c
 *
 * Created: 4/14/2024 4:09:00 PM
 * Author : MysticOwl
 */ 
/*
 * DS3231_write_read_test.c
 *
 * Created: 4/14/2024 4:08:22 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

volatile uint8_t RTC_time_date_write[7];
volatile uint8_t RTC_time_date_read[7];


//******************************************************************************
// Function : void I2C_rtc_DS3231_config(void)
// Date and version : 041024, version 1.0
// Target MCU : AVR128 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function configures an AVR128DB48 operated at
// 4 MHz to communicate with the DS323
// SCL must be operated at the maximum possible frequency for
// the DS3231.
//******************************************************************************

void I2C_rtc_DS3231_config(void){
	
	TWI0.MBAUD = 0x01;	//Want 400kHz, but to get it BAUD value would be negative. 4MHz main clock -> ~400KHz I2C clock
	TWI0.MCTRLA = 0x01;	//Enable TWI master bit0
	//Smart mode enable SMEN is bit1, it is 0, so smart mode not enabled.
	//Since SMEN = 0, MCMD field in MCTRLB must be written for each byte
	//received by master to create an acknowledge action followed by an operation.
	TWI0.DBGCTRL = 0x01;
	TWI0.MSTATUS = 0x01; //Force bus state to idle
	
	
	
}

//***************************************************************************
// Function Name : "block_write_RTC"
// void block_write_RTC (uint8_t slave, volatile uint8_t *array_ptr,
// uint8_t strt_addr, uint8_t count)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function writes a block of data from an array to the DS3231. strt_addr
// is the starting address in the DS3231. count is the number of data bytes to
// be transferred and array_ptr is the address of the source array in the AVR128.
//**************************************************************************void block_write_RTC(uint8_t slave, volatile uint8_t *array_ptr, uint8_t strt_addr, uint8_t count) {
	while((TWI0.MSTATUS & 0x03) != 0x01); // wait until idle

		TWI0.MADDR = slave << 1;         // send base address for write
		while((TWI0.MSTATUS & 0x40) == 0); // WIF flag, wait until saddr sent

		TWI0.MDATA = (strt_addr);    // send memory address
		while((TWI0.MSTATUS & 0x40) == 0); // WIF flag, wait until raddr sent

for (uint8_t i = 0; i < count; i++) {
		TWI0.MDATA = array_ptr[i];       // send data
		while((TWI0.MSTATUS & 0x40) == 0); // WIF flag, wait until data sent
	}

	TWI0.MCTRLB |= 0x03;                // issue a stop
	return;
}

//***************************************************************************
// Function Name : "block_read_RTC"
// void block_read_RTC (uint8_t slave, volatile uint8_t *array_ptr,
// uint8_t strt_addr, uint8_t count)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function reads a block of data from the DS3231 and transfers it to an
// array. strt_addr is the starting address in the DS3231. count is the number
// of data bytes to be transferred and array_ptr is the address of the
// destination array in the AVR128.
//**************************************************************************
void block_read_RTC(uint8_t slave, volatile uint8_t *array_ptr, uint8_t strt_addr, uint8_t count) {
	// Store the data in this: volatile uint8_t RTC_time_date_read[7];

	while((TWI0.MSTATUS & 0x03) != 0x01); // wait until idle

	
		TWI0.MADDR = slave << 1;         /* send address for write */ // 0 VERSION
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until saddr sent */
		
		//The next write clears the WIF flag
		TWI0.MDATA = strt_addr;              /* send memory address    WRITE COMMAND  */
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until raddr sent */
		
		TWI0.MADDR = (slave << 1) | (0b00000001) ;         /* send address for write */ // 1 VERSION
		
	for (uint8_t i = 0; i < count-1; i++) {
	
		while((TWI0.MSTATUS & 0x80) == 0); // RIF flag, wait until byte is received
		array_ptr[i] = TWI0.MDATA; // Store the received byte in the destination array
		
		TWI0.MCTRLB = 0x02; // MCMD - issue ack followed by a byte read operation
	
	
	}
	
	
	while((TWI0.MSTATUS & 0x80) == 0); // RIF flag, wait until byte is received
		array_ptr[count-1] = TWI0.MDATA; // Store the received byte in the destination array
		
		TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;	//MCMD issue nack followed by a stop

		
	return; // read data from received data buffer
}


int main(void)
{
	uint8_t DS3231addr = 0x68;
	uint8_t REGaddr = 0x00; //time reg ADDRESS
	uint8_t configdata = 0xFF;
	
	// Variables for the read function
	volatile uint8_t RTC_time_date_read[7];
	uint8_t slave_address = 0x68; // Slave address of the DS3231 device
	uint8_t start_address = 0x00; // Starting address in the DS3231 to read from
	uint8_t read_count = 7; // Number of bytes to read

	// Variables for the write function
	volatile uint8_t RTC_time_date_write[7] = {0x55, 0x59, 0x23, 0x14, 0x04, 0x20, 0x24}; // Data to be written to the DS3231
	uint8_t write_start_address = 0x00; // Starting address in the DS3231 to write to
	uint8_t write_count = 7; // Number of bytes to write
	
	I2C_rtc_DS3231_config();
	
    /* Replace with your application code */
    while (1) 
    {
		while((TWI0.MSTATUS & 0x03) != 0x01) ; /* wait until I2C bus idle */
		
		block_write_RTC(slave_address, RTC_time_date_write, write_start_address, write_count);
    	block_read_RTC(slave_address, RTC_time_date_read, start_address, read_count);
		
    }
}

