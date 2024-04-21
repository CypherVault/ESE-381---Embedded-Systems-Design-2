/*
 * Lab 1 - Task 1.c
 *
 * Created: 1/27/2024 3:41:30 PM
 * Author : cnielsen
 */ 

#include <avr/io.h>


int main(void)
{
    
	VPORTB_DIR = 0x08;      //set PB3 as an output
	PORTB_PIN2CTRL = 0x08;  //enable pullup resistor for SW0 

    while (1) 
    {
		
		VPORTB_OUT = VPORTB_IN << 1;
		
    }
}

