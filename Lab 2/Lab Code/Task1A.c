/*
 * parallel_in_parallel_out_flat.c
 *
 * Created: 2/3/2024 6:58:25 PM
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
		
    while (1) 
    {
		//flat
		PORTD_OUT = ((VPORTA_IN & 0b11111100) | (VPORTC_IN & 0b00000011));
		
    }
	
	
	
}

