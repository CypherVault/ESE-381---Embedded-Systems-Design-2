/*
 * simple_comb_functon_nb.c
 *
 * Created: 2/3/2024 7:05:04 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>

typedef union{
	
	uint8_t byte;
	
	struct {
		
		uint8_t bit0 : 1;
		uint8_t bit1 : 1;
		uint8_t bit2 : 1;
		uint8_t bit3 : 1;
		uint8_t bit4 : 1;
		uint8_t bit5 : 1;
		uint8_t bit6 : 1;
		uint8_t bit7 : 1;
		
	}bvals;
} Named_bits;

int main(void)
{
    
	volatile Named_bits data;
//	volatile uint8_t  
//	volatile uint8_t 

	//flat
	PORTA_PIN7CTRL = 0x08;  //enable pullup resistor for PA7
	PORTA_PIN6CTRL = 0x08;  //enable pullup resistor for PA6
	PORTA_PIN5CTRL = 0x08;  //enable pullup resistor for PA5
	PORTA_PIN4CTRL = 0x08;  //enable pullup resistor for PA4
	PORTA_PIN3CTRL = 0x08;  //enable pullup resistor for PA3
	PORTA_PIN2CTRL = 0x08;  //enable pullup resistor for PA2
	PORTA_PIN1CTRL = 0x08;  //enable pullup resistor for PA1 // I DONT WANT THIS TO FLOAT AND CAUSE BAD BEHAVIOR
	PORTA_PIN0CTRL = 0x08;  //enable pullup resistor for PA0 // I DONT WANT THIS TO FLOAT AND CAUSE BAD BEHAVIOR

	//OUTPUT Port config
	
	//flat
	PORTD_DIR = 0xFF;      //set Port D as an output for led bar graph
	
	
	
    while (1) 
    {
		
		data.byte = VPORTA_IN;
		data.bvals.bit7 = ( (!data.bvals.bit7&data.bvals.bit6&!data.bvals.bit5) | (!data.bvals.bit7&data.bvals.bit6&data.bvals.bit5) | (data.bvals.bit7&!data.bvals.bit6&data.bvals.bit5) | (data.bvals.bit7&data.bvals.bit6&data.bvals.bit5));
		data.byte &= 0b10000000;
		VPORTD_OUT = ~data.byte;
		
		
    }
}

