/*
 * read_modify_write_sftw_sw0.c
 *
 * Created: 2/3/2024 7:00:29 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>

int main(void)
{
	
	//INPUT Pin Configuration for Dip Switches
	
	//flat
	PORTA_PIN7CTRL = 0x08;  //enable pullup resistor for PA7
	PORTA_PIN6CTRL = 0x08;  //enable pullup resistor for PA6
	PORTA_PIN5CTRL = 0x08;  //enable pullup resistor for PA5
	PORTA_PIN4CTRL = 0x08;  //enable pullup resistor for PA4
	PORTA_PIN3CTRL = 0x08;  //enable pullup resistor for PA3
	PORTA_PIN2CTRL = 0x08;  //enable pullup resistor for PA2
	//PORTA_PIN1CTRL = 0x08;  //enable pullup resistor for PA1 // I DONT WANT THIS TO FLOAT AND CAUSE BAD BEHAVIOR
	//PORTA_PIN0CTRL = 0x08;  //enable pullup resistor for PA0 // I DONT WANT THIS TO FLOAT AND CAUSE BAD BEHAVIOR

	PORTC_PIN1CTRL = 0x08;  //enable pullup resistor for PC1
	PORTC_PIN0CTRL = 0x08;  //enable pullup resistor for PC0
	
	//OUTPUT Port config
	
	//flat
	PORTD_DIR = 0xFF;      //set Port D as an output for led bar graph
	
	//pushbutton code
	VPORTB_DIR = 0b11111011;      //set everything but B2 as an output so they stay zero.
	PORTB_PIN2CTRL = 0x08;  //enable pullup resistor for SW0 for pushbutton

		//flat
	PORTD_OUT = ((VPORTA_IN & 0b11111100) | (VPORTC_IN & 0b00000011));

	
	while (1)
	{
		
		VPORTB_OUT = VPORTB_IN << 1;
		
		if	( (VPORTB_IN & 0b00000100) == 0b00000000) { // if the whole port is ZERO, then the pushbutton is pressed which means OUTPUT.
			
			
			VPORTD_OUT = (VPORTD_OUT&0b11000111)|((((VPORTA_IN & 0b00000100) | (VPORTC_IN & 0b00000011)) << 3) & (0b00111000));
		}
		
		else {
			
		}
		
		
		
	}
	
}

