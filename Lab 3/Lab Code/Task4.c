/*
 * toggle_every_xxx_us.c
 *
 * Created: 2/11/2024 1:50:32 PM
 * Author : Chris
 */ 

#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h>


int main(void)
{
	
			
			
			//flat
			PORTA_PIN7CTRL = 0x08;  //enable pullup resistor for PA7
			PORTA_PIN6CTRL = 0x08;  //enable pullup resistor for PA6
		PORTB.DIR |= PIN3_bm;

    /* Replace with your application code */
    while (1) 
    {
		
		
		
		if ((VPORTA_IN | 0b00111111) == 0b00111111) {
				
				//DIP CODE 00
				///52.08333
				PORTB.OUT |= PIN3_bm;
				_delay_ms(52);
				PORTB.OUT &= ~PIN3_bm;
				_delay_ms(52);
				
		}
	else if  ((VPORTA_IN | 0b00111111) == 0b01111111)  {
		
		
		//DIP CODE 01
		///104.1666
		PORTB.OUT |= PIN3_bm;
		_delay_ms(104);
		PORTB.OUT &= ~PIN3_bm;
		_delay_ms(104);
		
	}
	else if  ((VPORTA_IN | 0b00111111) == 0b10111111) {
		
		//DIP CODE 10
		///208.333
		PORTB.OUT |= PIN3_bm;
		_delay_ms(208);
		PORTB.OUT &= ~PIN3_bm;
		_delay_ms(208);
		
	}
	
	else {
}
		
    }
}


		