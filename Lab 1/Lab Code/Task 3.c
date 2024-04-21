/*
 * lab 1 Task 3.c
 *
 * Created: 1/27/2024 4:28:28 PM
 * Author : MysticOwl
 */ 

#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h> 


int main(void)
{
    /* Replace with your application code */
    PORTB.DIR |= PIN3_bm;
	
	
	
	while (1) 
    {
		
		PORTB.OUT |= PIN3_bm;
		_delay_ms(500);
		PORTB.OUT &= ~PIN3_bm;
		_delay_ms(500);
		
    }
}

