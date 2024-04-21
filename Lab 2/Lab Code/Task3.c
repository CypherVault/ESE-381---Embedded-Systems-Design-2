/*
 * simple_comb_function.c
 *
 * Created: 2/3/2024 7:03:14 PM
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
	PORTA_PIN1CTRL = 0x08;  //enable pullup resistor for PA1 // I DONT WANT THIS TO FLOAT AND CAUSE BAD BEHAVIOR
	PORTA_PIN0CTRL = 0x08;  //enable pullup resistor for PA0 // I DONT WANT THIS TO FLOAT AND CAUSE BAD BEHAVIOR

	//OUTPUT Port config
	
	//flat
	PORTD_DIR = 0xFF;      //set Port D as an output for led bar graph
	
	
	while (1){
										//~A B ~C									//~A B C										//A ~B C									//A B C
		if ( ((VPORTA_IN | 0b00011111) == 0b01011111)  | ((VPORTA_IN | 0b00011111) == 0b01111111) | 	((VPORTA_IN | 0b00011111) == 0b10111111) | ((VPORTA_IN | 0b00011111) == 0b11111111)  ) {
			
			VPORTD_OUT = 0b01111111;
			
		}
		else {
			
			VPORTD_OUT = 0xFF ;
			
		}
		
		
		
	}
	
	
	
}

