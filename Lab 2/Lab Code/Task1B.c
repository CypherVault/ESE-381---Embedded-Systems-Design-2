/*
 * parallel_in_parallel_out_struct.c
 *
 * Created: 2/3/2024 6:58:25 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>


int main(void)
{
 
		//INPUT Pin Configuration for Dip Switches
		
		
		//structural
		PORTA.PIN7CTRL = 0x08;  //enable pullup resistor for PA7
		PORTA.PIN6CTRL = 0x08;  //enable pullup resistor for PA6
		PORTA.PIN5CTRL = 0x08;  //enable pullup resistor for PA5
		PORTA.PIN4CTRL = 0x08;  //enable pullup resistor for PA4
		PORTA.PIN3CTRL = 0x08;  //enable pullup resistor for PA3
		PORTA.PIN2CTRL = 0x08;  //enable pullup resistor for PA2
		PORTC_PIN1CTRL = 0x08;  //enable pullup resistor for PC1
		PORTC_PIN0CTRL = 0x08;  //enable pullup resistor for PC0
		
		//OUTPUT Port config
		
		
		//structural	
		PORTD.DIR = 0xFF;    
		
    while (1) 
    {
		//structural	
		PORTD.OUT = ((VPORTA.IN & 0b11111100) | (VPORTC.IN & 0b00000011));
		
    }
	
	
	
}

