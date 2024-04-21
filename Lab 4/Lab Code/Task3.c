/*
 * asynch_sw_read_interrupt.c
 *
 * Created: 2/18/2024 7:09:24 PM
 * Author : MysticOwl
 */ 



#include <avr/io.h>
#define F_CPU 4000000 //freq
#include <util/delay.h> 
#define BAUD_RATE 9600
volatile uint8_t data;
#include <avr/interrupt.h>
volatile uint8_t received_char = 0;

ISR(PORTB_PORT_vect) {

	
	_delay_us(((1000000/BAUD_RATE))/2);  // Adjust delay based on baud rate and system clock

	if((PORTB.IN & 0b00000010) != 0b00000000) { //false start check
		PORTB.INTFLAGS = 0x02;
		return 'P';
		
	}

	

	// Receive 8 data bits (LSB first)
	for (int8_t i = 0; i < 8; ++i) {
		_delay_us((1000000/BAUD_RATE-5));
		received_char = received_char >> 1;
		received_char |= (((PORTB.IN & 0b00000010) << 6));
		// Adjust delay based on baud rate and system clock
		
	};
	_delay_us((1000000/BAUD_RATE));
	// Wait for the stop bit one bit later
	PORTB.INTFLAGS = 0x02;
	
	if ((PORTB.IN & 0b00000010) == 0b00000010) {// there is a 1 at the pin ;
		
		return (char)received_char;
		
	}
	else {
		PORTB.INTFLAGS = 0x02;
		return 'A'; // framing error
	}

// Clear the interrupt flag for the next go-around


};


int main(void)
{
	
    /* Replace with your application code */
	
	// SREG = 0b10000000; // disable global interrupts
	
    SREG |=  CPU_I_bm;
	
	PORTB.DIR = 0x00; // make the whole port an input.
	PORTB.PIN1CTRL = 0x03;
//	PORTB.PIN1CTRL |= 0x08;
	
	
	
	
//	USART3.BAUD = 0b10010110000000;      //baud rate
	
//	USART3.CTRLC =  0x03 ;    //frame format
	
//	USART3.CTRLA =  0b00010000 ;    //enable recieve interrupt 
	
//	USART3.CTRLB =  0b11000000;    //transmitter and receiver enabling  as output
	
//	USART3.CTRLB |=  0b00010000;     // enable the false start check 
	
//	USART3.STATUS(6) = 1 means the data has been sent.
	
	
	
    while (1) 
    {
		asm volatile ("nop");
		
		
    }
}

