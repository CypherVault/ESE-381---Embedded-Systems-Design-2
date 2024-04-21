/*
 * TWI0_LM75_init.c
 *
 * Created: 4/7/2024 2:14:35 PM
 * Author : MysticOwl
 *//*
 * LM75_I2C_tempSensor.c
 *
 * Created: 4/6/2024 1:30:49 PM
 * Author : kshort
 */ 
 

#include <avr/io.h>
#define F_CPU 4000000	//Frequency used by delay macros.
#include <util/delay.h>	//Header for delay macros and functions.
#include <string.h>
#include <stdio.h>


/* This program configures and reads an LM75. It computes the
* measured temperature in tenths of a degree and puts it in a
* string which it stores in a buffer.
*
* This program is essentially Task 2 and part of Task 3 of Lab 09 s24.
*
* Note that this program is in a development form. It contains
* code which is not necessary for the program's minimal function
* but could be useful if the program is expanded. It also contains
* comments that are more extensive than necessary, but are good
* for latter reference.
*/

/*
*
* AVR128DB48 TWI0 is configured as I2C master.
* System clock rate is set to approx. 4 MHz.
* AVR128DB48 is connected to LM75 Temperature Sensor.
* System implements test of reading, computing, and formatting for 
* display the LM75's temperature.
*
* Tested with 10 kohm pullups on SDA and SCL.
*
* PA2  I2C SDA	serial data
* PA3  I2C SCL	serial clock
*
* Tested with Atmel Studio 7
*/

                                                                                               
#define LM75_ADDR 0x48     // LM75 address
#define TEMPERATURE_ADDR 0x00	//slave address of temperature register

uint8_t temp_reg_high;	//high byte of LM75 Temperature register
uint8_t temp_reg_low;	//low byte of LM75 Temperature register	
uint16_t LM75_temp_reg = 0;	//LM75 Temperature register contents
int16_t LM75_temp = 0;	//right adjusted 2's complement, resolution 0.1C
char dsp_buff1[17];		//buffer for line 1 of LCD image

//Function prototypes
void TWI0_LM75_init(void);	//Initialize TWI0 module to talk to LM75

//Write a byte to the specified I2C slave. Parameters are slave address,
//address of register in slave to be written, and data to be written.
int TWI0_LM75_write(unsigned char saddr, unsigned char raddr, unsigned char data);
//void LM75_TWI0_init(void) ; //Initialize LM75
uint16_t TWI0_LM75_read(unsigned char saddr);


int main (void) {
	TWI0_LM75_init();	//Initialize TWI0 to talk LM75 slave.
	

	while (1) {
		while((TWI0.MSTATUS & 0x03) != 0x01) ; /* wait until I2C bus idle */
		LM75_temp_reg = TWI0_LM75_read(LM75_ADDR);
		LM75_temp = ((int16_t)LM75_temp_reg) >> 7;
//		sprintf(dsp_buff1, "Temp = %4d", (LM75_temp >> 1)); //integer result
//		sprintf(dsp_buff1, "Temp = %4d.%d", (LM75_temp >> 1),((LM75_temp%2) ? 5 : 0) ); //only for pos temps
		sprintf(dsp_buff1, "Temp = %.1f", ((float)(LM75_temp)/2.0)); //requires vprintf library
//		_delay_ms(1000);						
	}
}

void TWI0_LM75_init(void) {
	
	//29.3.2.1 Initialization
	//If used, the following bits must be configured before enabling the TWI device:
	//• The SDA Setup Time (SDASETUP) bit from the Control A (TWIn.CTRLA) register
	//• The SDA Hold Time SDAHOLD) bit field from the Control A (TWIn.CTRLA) register
	//• The FM Plus Enable (FMPEN) bit from the Control A (TWIn.CTRLA) register

	//Default values work for this Laboratory

	//29.3.2.1.1 Master Initialization
	//The Master Baud Rate (TWIn.MBAUD) register must be written to a value that will result in a valid TWI bus clock
	//frequency. Writing a ‘1’ to the Enable TWI Master (ENABLE) bit in the Master Control A (TWIn.MCTRLA) register will
	//enable the TWI master. The Bus State (BUSSTATE) bit field from the Master Status (TWIn.MSTATUS) register must
	//be set to 0x1, to force the bus state to Idle.
	
	TWI0.MBAUD = 0x01;	//Want 400kHz, but to get it BAUD value would be negative. 4MHz main clock -> ~400KHz I2C clock
	TWI0.MCTRLA = 0x01;	//Enable TWI master bit0
	//Smart mode enable SMEN is bit1, it is 0, so smart mode not enabled.
	//Since SMEN = 0, MCMD field in MCTRLB must be written for each byte
	//received by master to create an acknowledge action followed by an operation.
	TWI0.DBGCTRL = 0x01;
	TWI0.MSTATUS = 0x01; //Force bus state to idle


	//29.3.2.2 TWI Master Operation
	//The TWI master is byte-oriented, with an optional interrupt after each byte. There are separate interrupt flags for the
	//master write and read operation. Interrupt flags can also be used for polled operation. There are dedicated status
	//flags for indicating ACK/NACK received, bus error, arbitration lost, clock hold, and bus state.
	//When an interrupt flag is set to ‘1’, the SCL is forced low. This will give the master time to respond or handle any
	//data, and will, in most cases, require software interaction. Clearing the interrupt flags releases the SCL. The number
	//of interrupts generated is kept to a minimum by an automatic handling of most conditions.
	

}


int TWI0_LM75_write(uint8_t saddr, uint8_t raddr, uint8_t data) {
	while((TWI0.MSTATUS & 0x03) != 0x01) ; /* wait until idle */
	
	TWI0.MADDR = saddr << 1;         /* send address for write */
	while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until saddr sent */
	
	//The next write clears the WIF flag
	TWI0.MDATA = raddr;              /* send memory address */
	while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until raddr sent */
	
	//The next write clears the WIF flag
	TWI0.MDATA = data;               /* send data */
	while((TWI0.MSTATUS & 0x40) == 0); /* WIF flag, wait until data sent */
	
	//The next write clears the WIF flag
	TWI0.MCTRLB |= 0x03;          /* issue a stop */

	return 0;
}




uint16_t TWI0_LM75_read(uint8_t saddr)
{	
	while((TWI0.MSTATUS & 0x03) != 0x01) ;	// wait until idle 
	
	TWI0.MADDR = ((saddr << 1) | 0x01);		// send slave address and read command
	
	while((TWI0.MSTATUS & 0x80) == 0);		// RIF flag, wait until byte is received
											// WIF flag does not work here	
	temp_reg_high = TWI0.MDATA;				//clears the RIF flag
	
	TWI0.MCTRLB = 0x02;					//MCMD - issue ack followed by a byte read operation
	
	while((TWI0.MSTATUS & 0x80) == 0);		// RIF flag, wait until data received
	
	temp_reg_low = TWI0.MDATA;				//clears the RIF flag

	TWI0.MCTRLB = 0x07;					//MCMD issue nack followed by a stop
	
	return (uint16_t)((temp_reg_high << 8) | (temp_reg_low & 0x80));	//read data from received data buffer
}

