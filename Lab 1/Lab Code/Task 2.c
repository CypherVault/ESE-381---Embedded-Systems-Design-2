/*
 * Lab 1 - Task 2.c
 *
 * Created: 1/27/2024 3:45:15 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>


int main(void)
{
   //INPUTS
 PORTA_PIN2CTRL = 0x08;  //enable pullup resistor for SW1
 PORTA_PIN5CTRL = 0x08;  //enable pullup resistor for SW2
  //OUTPUTS
 PORTB.DIR = 0x20;      //set PB5 as an output	for LED1
//bool Led_on = false;
 
    while (1)
    {
	if ( PORTA.IN == 0b00000011 )
	{
		
		 PORTB.OUT = 0b00000000;
	}
	else
	{
	 PORTB.OUT = 0b00100000;
	}
    }
	
	
}

