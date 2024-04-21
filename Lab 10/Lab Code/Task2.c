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
// Function: void write_RTC (uint8_t slave, unit8_t reg_RTC, uint8_t data_RTC)
//
// Target MCU : AVR128DB48 @ 4MHz
// Target Hardware ;
// Author : Ken Short
// DESCRIPTION
// This function writes data to a register in the RTC. To accomplish this, it
// must first write the DS3231’s slave address, then the register’s pointer
// address, and finally the data.
//**************************************************************************

void write_RTC (uint8_t slave, uint8_t reg_RTC, uint8_t data_RTC){
	
		while((TWI0.MSTATUS & 0x03) != 0x01) ; /* wait until idle */
		
		TWI0.MADDR = slave << 1;         /* send address for write */
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until saddr sent */
		
		//The next write clears the WIF flag
		TWI0.MDATA = reg_RTC;              /* send memory address */
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until raddr sent */
		
		//The next write clears the WIF flag
		TWI0.MDATA = data_RTC;               /* send data */
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until data sent */
		
		//The next write clears the WIF flag
		TWI0.MCTRLB |= 0x03;          /* issue a stop */

		return;
	
}


//***************************************************************************
// Function: uint8_t read_RTC (uint8_t slave, uint8_t reg_RTC)
// Target MCU : AVR128DB48 @ 4MHz
// Author : Ken Short
// DESCRIPTION
// This function reads data from a register in the RTC. To accomplish this, it
// must first write the DS3231’s slave address, then its pointer address, and
// finally read the data.
//**************************************************************************

uint8_t read_RTC (uint8_t slave, uint8_t reg_RTC){
	
	
	volatile	uint8_t data;
		
		TWI0.MADDR = slave << 1;         /* send address for write */ // 0 VERSION
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until saddr sent */
		
		//The next write clears the WIF flag
		TWI0.MDATA = reg_RTC;              /* send memory address    WRITE COMMAND  */ 
		while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until raddr sent */
	
		TWI0.MADDR = (slave << 1) | (0b00000001) ;         /* send address for write */ // 1 VERSION
	//	while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until saddr sent */
		
		while((TWI0.MSTATUS & 0x80) == 0);		// RIF flag, wait until data received
		
		// WIF flag does not work here
		data = TWI0.MDATA;				//clears the RIF flag
		TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;	//MCMD issue nack followed by a stop
		
	
		return (data);
}

uint8_t toWrite;
uint8_t justRead;


int main(void)
{
	uint8_t DS3231addr = 0x68;
	uint8_t REGaddr = 0x0E; //CONTROL reg ADDRESS
	uint8_t configdata = 0x00;
	uint8_t statusAddr = 0x0F;
	uint8_t statusData = 0x08;
	
	
	I2C_rtc_DS3231_config();
	write_RTC(DS3231addr, statusAddr, statusData);
	
    /* Replace with your application code */
    while (1) 
    {
		while((TWI0.MSTATUS & 0x03) != 0x01) ; /* wait until I2C bus idle */
			
		toWrite = 0x23;	
			
		//write_RTC(DS3231addr, REGaddr, configdata);
		write_RTC(DS3231addr, REGaddr, configdata);
		
		
		justRead = read_RTC(DS3231addr, 0x0A);
		
    }
}

